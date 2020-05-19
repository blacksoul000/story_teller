#include "domain/deti_online_handler.hpp"

#include <QtNetwork>
#include <QRegExp>


using domain::DetiOnlineHandler;


struct DetiOnlineHandler::Impl
{
    QList< domain::MetaInfo > media;
    QUrl preview;
    QString tittle;

    void parseMedia(const QString& buffer);
    void parseTittle(const QString& buffer);
    void parsePreview(const QString& buffer);
};


DetiOnlineHandler::DetiOnlineHandler(QObject* parent): 
    domain::AbstractPageHandler(parent),
    d(new Impl)
{}

DetiOnlineHandler::~DetiOnlineHandler()
{
    delete d;
}

void DetiOnlineHandler::httpReadyRead()
{
    QString buff = m_reply->readAll();
    d->parseMedia(buff);
    d->parseTittle(buff);
    d->parsePreview(buff);
}

QList< domain::MetaInfo > DetiOnlineHandler::media() const
{
    return d->media;
}

QUrl DetiOnlineHandler::preview() const
{
    return d->preview;
}

QString DetiOnlineHandler::tittle() const 
{
    return d->tittle;
}

void DetiOnlineHandler::Impl::parseMedia(const QString& buffer)
{
    QRegExp regexp("window\\.pl\\.push\\(\\{duration:'([0-9:]+)',file:'(https:\\/\\/stat3\\.deti-online\\.com[a-zA-Z0-9\\/\\-_]+\\.mp3)'"
        ",title:'([А-ЯЁа-яё0-9:;,\\/\\.\\s\\-—_\\(\\)\\?\\!]+)'");
    regexp.setMinimal(true);
    int lastPos = 0;
    while ((lastPos = regexp.indexIn(buffer, lastPos)) != -1) 
    {
        lastPos += regexp.matchedLength();
        media.append({regexp.cap(3), QUrl(regexp.cap(2)), 0.0/*std::(regexp.cap(1)*/, QUrl()}); // TODO
    }
}

void DetiOnlineHandler::Impl::parseTittle(const QString& buffer)
{
    QRegExp regexp("<head><title>(Аудио\\s?сказки|Аудио\\s?сказка|Аудио\\s?книга)?\\s?([А-ЯЁа-яё\\s-,]+)\\.? [сС]лушать");

    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        tittle = regexp.cap(2);
    }
}

void DetiOnlineHandler::Impl::parsePreview(const QString& buffer)
{
    QRegExp regexp("srcset=\"([a-zA-Z0-9\\/-]+\\.jpg)\\s2x\"");
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        preview = QUrl::fromUserInput("https://deti-online.com" + regexp.cap(1));
    }
}
