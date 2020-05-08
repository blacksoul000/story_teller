#ifndef STORY_LIST_HPP
#define STORY_LIST_HPP


#include "domain/story_object.hpp"

#include <QObject>
#include <QUuid>
#include <QUrl>


namespace domain
{
    class StoryGroup;
    
    class StoryList : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QList< QObject* > stories READ stories NOTIFY storiesChanged)

    public:
        StoryList(QObject* parent = nullptr);
        ~StoryList() override;

        QList< QObject* > stories() const;

        void addStoryGroup(StoryGroup* story, int pos = -1);
        void removeStoryGroup(const QUuid& id);

    signals:
        void storiesChanged();

    private:
        struct Impl;
        Impl* d;
    };
}  // namespace domain

#endif  // STORY_LIST_HPP