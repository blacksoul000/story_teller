#include "domain/story_group.hpp"

#include "domain/story_object.hpp"


using domain::StoryGroup;


struct StoryGroup::Impl
{
    QUuid id;
    QString tittle;
    QUrl preview;
    QList< QObject* > stories;
};


StoryGroup::StoryGroup(const QString& tittle, const QUrl& preview, const QUuid& id, QObject* parent):
    QObject(parent),
    d(new Impl)
{
    d->id = id;
    d->tittle = tittle;
    d->preview = preview;
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

QUrl StoryGroup::preview() const
{
    return d->preview;
}

QList< QObject* > StoryGroup::stories() const
{
    return d->stories;
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
}
