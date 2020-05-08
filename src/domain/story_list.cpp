#include "domain/story_list.hpp"

#include "domain/story_group.hpp"


using domain::StoryList;


struct StoryList::Impl
{
    QList< QObject* > stories;
};


StoryList::StoryList(QObject* parent):
    QObject(parent),
    d(new Impl)
{
}

StoryList::~StoryList()
{
    delete d;
}

QList< QObject* > StoryList::stories() const
{
    return d->stories;
}

void StoryList::addStoryGroup(StoryGroup* story, int pos)
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

void StoryList::removeStoryGroup(const QUuid& id)
{
    auto it = std::find_if(d->stories.begin(), d->stories.end(), [id](QObject* obj)->bool{
        auto group = reinterpret_cast< domain::StoryGroup* >(obj);
        return group && group->id() == id;
    });
    if (it == d->stories.end()) return;
    d->stories.erase(it);
    emit storiesChanged();
}
