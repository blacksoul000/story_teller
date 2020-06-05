#include "domain/deti_online_handler.hpp"

#include <QRegExp>


using domain::DetiOnlineHandler;


QList< domain::MetaInfo > DetiOnlineHandler::parseMedia(const QString& buffer) const
{
    QList< domain::MetaInfo > media;

    QRegExp regexp("window\\.pl\\.push\\(\\{duration:'([0-9:]+)',file:'(https:\\/\\/stat3\\.deti-online\\.com[a-zA-Z0-9\\/\\-_]+\\.mp3)'"
        ",title:'([А-ЯЁа-яё0-9a-zA-Z:;,\\/\\.\\s\\-—_\\(\\)\\?\\!«»]+)'");
    regexp.setMinimal(true);
    int lastPos = 0;
    while ((lastPos = regexp.indexIn(buffer, lastPos)) != -1) 
    {
        lastPos += regexp.matchedLength();
        media.append({regexp.cap(3), QUrl(regexp.cap(2)), 0.0, QUrl()});
    }
    return media;
}

QString DetiOnlineHandler::parseTittle(const QString& buffer) const
{
    QRegExp regexp("<head><title>(Аудио\\s?сказки|Аудио\\s?сказка|Аудио\\s?книга)?\\s?"
        "([А-ЯЁа-яё0-9a-zA-Z:;\\s\\-—_,\\(\\)\\?\\!«»]+)\\.?\\s?\\|?\\s?([сС]лушать)?");

    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        return regexp.cap(2);
    }
}

QUrl DetiOnlineHandler::parsePreview(const QString& buffer) const
{
    QRegExp regexp("srcset=\"([a-zA-Z0-9\\/-]+\\.jpg)\\s2x\"");
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        return QUrl::fromUserInput("https://deti-online.com" + regexp.cap(1));
    }
}
