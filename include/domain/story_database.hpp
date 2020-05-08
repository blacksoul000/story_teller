#ifndef STORY_DATABASE_HPP
#define STORY_DATABASE_HPP


#include <QObject>


namespace domain
{
    class StoryGroup;

    class StoryDatabase : public QObject
    {
        // Q_OBJECT

    public:
        StoryDatabase(const QString& dbPath, QObject* parent = nullptr);
        ~StoryDatabase() override;

        bool isOpen() const;

        bool addStoryGroup(domain::StoryGroup* group);
        bool removeStoryGroup(const QUuid& id);
        QList< domain::StoryGroup* > loadAll(bool* ok = nullptr) const;

    private:
        bool createTables();

        struct Impl;
        Impl* d;
    };
}  // namespace domain

#endif  // STORY_DATABASE_HPP