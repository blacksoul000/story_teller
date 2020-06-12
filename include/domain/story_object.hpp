#ifndef STORY_OBJECT_HPP
#define STORY_OBJECT_HPP


#include "domain/meta_info.hpp"

#include <QObject>
#include <QUuid>
#include <QUrl>


namespace domain
{
    class StoryDatabase;

    class StoryObject : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QUuid id READ id CONSTANT)
        Q_PROPERTY(QString tittle READ tittle CONSTANT)
        Q_PROPERTY(QUrl url READ url CONSTANT)
        Q_PROPERTY(double duration READ duration CONSTANT)
        Q_PROPERTY(QUrl preview READ preview CONSTANT)
        Q_PROPERTY(bool isDeleted READ isDeleted WRITE setDeleted NOTIFY storyChanged)

    public:
        StoryObject(const StoryDatabase* db, const QString& tittle, const QUrl& url, double duration, 
                    const QUrl& preview = QUrl(), bool isDeleted = false,
                    const QUuid& id = QUuid::createUuid(), QObject* parent = nullptr);
        StoryObject(const StoryDatabase* db, const MetaInfo metaInfo, QObject* parent = nullptr);
        ~StoryObject() override;

        QUuid id() const;
        QString tittle() const;
        QUrl preview() const;
        double duration() const;
        QUrl url() const;

        bool isDeleted() const;
        void setDeleted(bool set);

    signals:
        void storyChanged();

    private:
        struct Impl;
        Impl* d;
    };
}  // namespace domain

#endif  // STORY_OBJECT_HPP