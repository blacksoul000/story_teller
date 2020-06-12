#include "domain/story_object.hpp"

#include "domain/story_database.hpp"

#include <QDebug>


using domain::StoryObject;
using domain::StoryDatabase;


struct StoryObject::Impl
{
    QUuid id;
    QString tittle;
    QUrl url;
    QUrl preview;
    double duration;
    bool isDeleted = false;
    const StoryDatabase* db = nullptr;
};


StoryObject::StoryObject(const StoryDatabase* db, const QString& tittle, const QUrl& url, double duration, 
                         const QUrl& preview, bool isDeleted, const QUuid& id, QObject* parent):
    QObject(parent),
    d(new Impl)
{
    d->id = id;
    d->tittle = tittle;
    d->url = url;
    d->preview = preview;
    d->duration = duration;
    d->isDeleted = isDeleted;
    d->db = db;
}

StoryObject::StoryObject(const StoryDatabase* db, const MetaInfo metaInfo, QObject* parent):
    StoryObject(db, metaInfo.tittle, metaInfo.url, metaInfo.duration, 
                metaInfo.preview, false, QUuid::createUuid(), parent)
{}

StoryObject::~StoryObject()
{
    delete d;
}

QUuid StoryObject::id() const
{
    return d->id;
}

QString StoryObject::tittle() const
{
    return d->tittle.trimmed();
}

QUrl StoryObject::preview() const
{
    return d->preview;
}

double StoryObject::duration() const
{
    return d->duration;
}

QUrl StoryObject::url() const
{
    return d->url;
}

bool StoryObject::isDeleted() const
{
    return d->isDeleted;
}

void StoryObject::setDeleted(bool set)
{
    qDebug() << Q_FUNC_INFO << set;
    d->isDeleted = set;
    d->db->updateStory(this);
    emit storyChanged();
}
