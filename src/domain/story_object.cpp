#include "domain/story_object.hpp"


using domain::StoryObject;


struct StoryObject::Impl
{
    QUuid id;
    QString tittle;
    QUrl url;
    QUrl preview;
    double duration;
};


StoryObject::StoryObject(const QString& tittle, const QUrl& url, double duration, 
                         const QUrl& preview, const QUuid& id, QObject* parent):
    QObject(parent),
    d(new Impl)
{
    d->id = id;
    d->tittle = tittle;
    d->url = url;
    d->preview = preview;
    d->duration = duration;
}

StoryObject::StoryObject(const MetaInfo metaInfo, QObject* parent):
    StoryObject(metaInfo.tittle, metaInfo.url, metaInfo.duration, 
                metaInfo.preview, QUuid::createUuid(), parent)
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
    return d->tittle;
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
