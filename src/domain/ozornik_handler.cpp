#include "domain/ozornik_handler.hpp"

#include <QTextDocument>
#include <QRegExp>


using domain::OzornikHandler;


QList< domain::MetaInfo > OzornikHandler::parseMedia(const QString& buffer) const
{
    QList< domain::MetaInfo > media;

    QRegExp regexp("data-audiourl=\"(https:\\/\\/ozornik\\.net\\/wp-content\\/uploads\\/"
        "[a-zA-Z0-9\\/\\-_]+\\.mp3)\" data-artist=\"\">\\s([0-9a-zA-Z:;\\s\\-—_,\\(\\)\\?\\!\\.&#«»]*)\\s<span");
    // regexp.setMinimal(true);
    QTextDocument text;
    int lastPos = 0;
    while ((lastPos = regexp.indexIn(buffer, lastPos)) != -1) 
    {
        lastPos += regexp.matchedLength();
        text.setHtml(regexp.cap(2));
        media.append({text.toPlainText(), 
                      regexp.cap(1), 
                      0.0, 
                      QUrl()});
    }
    return media;
}

QString OzornikHandler::parseTittle(const QString& buffer) const
{
    QRegExp regexp("<title>(Аудио\\s?сказки|Аудио\\s?сказка|Аудио\\s?книга)?"
        "\\s?([А-ЯЁа-яё0-9a-zA-Z:;\\s\\-—_,\\(\\)\\?\\!«»]+)[\\.\\s\\|]*([сС]лушать)");

    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        return regexp.cap(2);
    }
    return QString();
}

QUrl OzornikHandler::parsePreview(const QString& buffer) const
{
    QRegExp regexp("<img src=\"(https:\\/\\/ozornik\\.net\\/wp-content\\/uploads\\/[a-zA-Z0-9\\\\/-]+\\.jpe?g)\"");
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        return regexp.cap(1);
    }
    return QUrl();
}
