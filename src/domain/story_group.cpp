#include "domain/story_group.hpp"

#include "domain/story_object.hpp"
#include "domain/story_database.hpp"


using domain::StoryGroup;
using domain::StoryDatabase;


struct StoryGroup::Impl
{
    QUuid id;
    QString tittle;
    QUrl preview;
    QList< StoryObject* > stories;
    const StoryDatabase* db = nullptr; 
};


StoryGroup::StoryGroup(const StoryDatabase* db, const QString& tittle, const QUrl& preview, 
                       const QUuid& id, QObject* parent):
    QObject(parent),
    d(new Impl)
{
    d->id = id;
    d->tittle = tittle;
    d->preview = preview;
    d->db = db;
}

StoryGroup::~StoryGroup()
{
    delete d;
}

QUuid StoryGroup::id() const
{
    return d->id;
}

QString StoryGroup::tittle() const
{
    return d->tittle.trimmed();
}

void StoryGroup::setTittle(const QString& tittle)
{
    if (tittle == d->tittle) return;

    d->tittle = tittle;
    d->db->updateStoryGroup(this);
    emit tittleChanged();
}

QUrl StoryGroup::preview() const
{
    return d->preview;
}

QList< QObject* > StoryGroup::stories() const
{
    QList< QObject* > list;
    for (auto obj: d->stories)
    {
        if (!obj->isDeleted()) list.append(obj);
    }
    return list;
}

QList< QObject* > StoryGroup::storiesAll() const
{
    QList< QObject* > list;
    for (auto obj: d->stories)
    {
        list.append(obj);
    }
    return list;
}

void StoryGroup::addStory(StoryObject* story, int pos)
{
    if (pos < 0 || pos > d->stories.size()) 
    {
        d->stories.append(story);
    }
    else
    {
        d->stories.insert(pos, story);
    }

    emit storiesChanged();
    emit storiesAllChanged();
}
