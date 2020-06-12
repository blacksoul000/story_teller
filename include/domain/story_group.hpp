#ifndef STORY_GROUP_HPP
#define STORY_GROUP_HPP


#include "domain/story_object.hpp"

#include <QObject>
#include <QUrl>


namespace domain
{
    class StoryObject;
    class StoryDatabase;
    
    class StoryGroup : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QUuid id READ id CONSTANT)
        Q_PROPERTY(QString tittle READ tittle WRITE setTittle NOTIFY tittleChanged)
        Q_PROPERTY(QUrl preview READ preview CONSTANT)
        Q_PROPERTY(QList< QObject* > stories READ stories NOTIFY storiesChanged)
        Q_PROPERTY(QList< QObject* > storiesAll READ storiesAll NOTIFY storiesAllChanged)

    public:
        StoryGroup(const StoryDatabase* db, const QString& tittle, const QUrl& preview = QUrl(), 
                   const QUuid& id = QUuid::createUuid(), QObject* parent = nullptr);
        ~StoryGroup() override;

        QUuid id() const;
        QString tittle() const;
        void setTittle(const QString& tittle);
        QUrl preview() const;
        QList< QObject* > stories() const;
        QList< QObject* > storiesAll() const;

        void addStory(StoryObject* story, int pos = -1);

    signals:
        void storiesChanged();
        void storiesAllChanged();
        void tittleChanged();

    private:
        struct Impl;
        Impl* d;
    };
}  // namespace domain

#endif  // STORY_GROUP_HPP