#ifndef STORY_GROUP_HPP
#define STORY_GROUP_HPP


#include "domain/story_object.hpp"

#include <QObject>
#include <QUrl>


namespace domain
{
    class StoryObject;
    
    class StoryGroup : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QUuid id READ id CONSTANT)
        Q_PROPERTY(QString tittle READ tittle CONSTANT)
        Q_PROPERTY(QUrl preview READ preview CONSTANT)
        Q_PROPERTY(QList< QObject* > stories READ stories NOTIFY storiesChanged)

    public:
        StoryGroup(const QString& tittle, const QUrl& preview = QUrl(), 
                   const QUuid& id = QUuid::createUuid(), QObject* parent = nullptr);
        ~StoryGroup() override;

        QUuid id() const;
        QString tittle() const;
        QUrl preview() const;
        QList< QObject* > stories() const;

        void addStory(StoryObject* story, int pos = -1);

    signals:
        void storiesChanged();

    private:
        struct Impl;
        Impl* d;
    };
}  // namespace domain

#endif  // STORY_GROUP_HPP