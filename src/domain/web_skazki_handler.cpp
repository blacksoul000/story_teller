#include "domain/web_skazki_handler.hpp"

#include <QRegExp>


using domain::WebSkazkiHandler;


QList< domain::MetaInfo > WebSkazkiHandler::parseMedia(const QString& buffer) const
{
    QList< domain::MetaInfo > media;

    QRegExp regexp("\\{ name: '([А-ЯЁа-яё0-9:,\\/\\.\\s\\-–_\\(\\)\?!]+)', "
        "artist: '[а-яёА-Яёa-zA-Z\\s\\-–]+', url: '([a-zA-Z0-9\\/\\-_]+\\.mp3)' \\}");
    regexp.setMinimal(true);
    int lastPos = 0;
    while ((lastPos = regexp.indexIn(buffer, lastPos)) != -1) 
    {
        lastPos += regexp.matchedLength();
        media.append({regexp.cap(1), 
                      QUrl::fromUserInput("https://web-skazki.ru" + regexp.cap(2)), 
                      0.0, 
                      QUrl()});
    }
    return media;
}

QString WebSkazkiHandler::parseTittle(const QString& buffer) const
{
    QRegExp regexp("<title>(Аудио\\s?сказк[иа]|Аудио\\s?книга)\\s«([А-ЯЁа-яё\\s-,]+)» слушать онлайн<\\/title>");
    
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        return regexp.cap(2);
    }
}

QUrl WebSkazkiHandler::parsePreview(const QString& buffer) const
{
    QRegExp regexp("src=\"(https:\\/\\/web-skazki\\.ru\\/preview-files\\/[a-zA-Z0-9\\\\/-]+\\-x\\-[0-9]+\\.(jpe?g|png))\"");
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        return regexp.cap(1);
    }
}
