#include "domain/story_database.hpp"

#include "domain/story_group.hpp"
#include "domain/story_object.hpp"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QUuid>


using domain::StoryDatabase;


struct StoryDatabase::Impl
{
    QSqlDatabase db;
};


StoryDatabase::StoryDatabase(const QString& dbPath, QObject* parent):
    QObject(parent),
    d(new Impl)
{
    QFile f(dbPath);
    bool exists = f.exists();
    d->db = QSqlDatabase::addDatabase("QSQLITE");
    d->db.setDatabaseName(dbPath);
    if (!d->db.open()) 
    {
        qDebug() << "Failed to open database: " << dbPath;
    }
        
    if (!exists && !this->createTables())
    {
        f.remove();
    }
}

StoryDatabase::~StoryDatabase()
{
    delete d;
}

bool StoryDatabase::isOpen() const
{
    return d->db.isOpen();
}

bool StoryDatabase::addStoryGroup(domain::StoryGroup* group)
{
    QStringList queries;
    QFile groupImage;
    if (group->preview().isLocalFile())
    {
        groupImage.setFileName(group->preview().toLocalFile());
        groupImage.open(QIODevice::ReadOnly);
    }

    QString groupId = group->id().toString();
    queries << QString("INSERT INTO story_groups(id, tittle, preview, preview_url) "
        "VALUES ('%1', '%2', %3, %4);")
        .arg(groupId)
        .arg(group->tittle())
        .arg(groupImage.isOpen() && groupImage.isReadable() ? QString("'%1'").arg(QString(groupImage.readAll())) : "NULL")
        .arg(groupImage.isOpen() && groupImage.isReadable() ? "NULL" : QString("'%1'").arg(group->preview().toString()));

    for(auto storyObj: group->stories())
    {
        auto story = dynamic_cast< domain::StoryObject* >(storyObj);
        if (!story) return false;

        QFile storyImage;
        if (story->preview().isLocalFile())
        {
            storyImage.setFileName(story->preview().toLocalFile());
            storyImage.open(QIODevice::ReadOnly);
        }
        queries << QString("INSERT INTO stories(id, story_group_id, tittle, url, preview, preview_url, duration) "
            "VALUES ('%1', '%2', '%3', '%4', %5, %6, '%7');")
            .arg(story->id().toString())
            .arg(groupId)
            .arg(story->tittle())
            .arg(story->url().toString())
            .arg(storyImage.isOpen() && storyImage.isReadable() ? QString("'%1'").arg(QString(storyImage.readAll())) : "NULL")
            .arg(storyImage.isOpen() && storyImage.isReadable() ? "NULL" : QString("'%1'").arg(story->preview().toString()))
            .arg(story->duration());
    }
    if (!d->db.transaction()) return false;

    QSqlQuery query(d->db);
    for (auto queryStr: queries)
    {
        if (!query.exec(queryStr)) 
        {
            qDebug() << query.lastQuery() << query.lastError();
            d->db.rollback();
            return false;
        }
    }
    qDebug() << "Saved!";
    d->db.commit();
    return true;
}

QList< domain::StoryGroup* > StoryDatabase::loadAll(bool* ok) const
{
    if (ok) *ok = false;
    QList< domain::StoryGroup* > list;

    QSqlQuery query(d->db);
    QSqlQuery storyQuery(d->db);
    QString str = "SELECT id, tittle, preview, preview_url FROM story_groups;";
    if (!query.exec(str))
    {
        qDebug() << query.lastQuery() << query.lastError();
        return list;
    }
    while (query.next()) 
    {
        QUuid id = query.value(0).toUuid();
        QString tittle = query.value(1).toString();
        QByteArray preview = query.value(2).toByteArray();
        QUrl previewUrl = query.value(3).toUrl();
        // TODO - preview image
        auto group = new domain::StoryGroup(tittle, previewUrl, id);

        QString storyQuerystr = QString("SELECT id, tittle, url, preview, preview_url, duration "
            "FROM stories WHERE story_group_id = '%1';")
            .arg(id.toString());
        if (!storyQuery.exec(storyQuerystr)) 
        {
            qDebug() << storyQuery.lastQuery() << storyQuery.lastError();
            qDeleteAll(list);
            return list;
        }

        while (storyQuery.next()) 
        {
            QUuid storyId = storyQuery.value(0).toUuid();
            QString storyTittle = storyQuery.value(1).toString();
            QUrl storyUrl = storyQuery.value(2).toUrl();
            QByteArray storyPreview = storyQuery.value(3).toByteArray();
            QUrl storyPreviewUrl = storyQuery.value(4).toUrl();
            double storyDuration = storyQuery.value(5).toDouble();

            // qDebug() << "Loaded: " << storyTittle << storyUrl << storyDuration << storyPreviewUrl << storyId;
            auto story = new domain::StoryObject(storyTittle, storyUrl, storyDuration, storyPreviewUrl, storyId, group);
            group->addStory(story);
        }
        list.append(group);
    }

    if (ok) *ok = true;
    return list;
}

bool StoryDatabase::removeStoryGroup(const QUuid& id)
{
    QSqlQuery query(d->db);
    QString str = QString("DELETE FROM story_groups WHERE id = '%1';").arg(id.toString());
    if (!query.exec(str))
    {
        qDebug() << query.lastQuery() << query.lastError();
        return false;
    }
    return true;
}

bool StoryDatabase::createTables()
{
    QStringList queries;
    queries << "CREATE TABLE story_groups ("
            "id	TEXT NOT NULL UNIQUE, "
            "tittle	TEXT NOT NULL, "
            "preview	BLOB, "
            "preview_url	INTEGER, "
            "PRIMARY KEY(id));";
    queries <<  "CREATE TABLE stories ( "
        "id	TEXT NOT NULL, "
        "story_group_id	TEXT NOT NULL, "
        "tittle	TEXT NOT NULL, "
        "url	TEXT NOT NULL, "
        "preview	BLOB, "
        "preview_url	TEXT, "
        "duration	REAL DEFAULT 0.0, "
        "PRIMARY KEY(id), "
        "FOREIGN KEY(story_group_id) REFERENCES story_groups(id) ON DELETE CASCADE);";

    if (!d->db.transaction()) return false;

    QSqlQuery query(d->db);
    for (auto queryStr: queries)
    {
        if (!query.exec(queryStr)) 
        {
            qDebug() << query.lastQuery() << query.lastError();
            d->db.rollback();
            return false;
        }
    }
    d->db.commit();
    return true;
}
