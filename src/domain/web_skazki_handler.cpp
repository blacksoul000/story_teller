#include "domain/web_skazki_handler.hpp"

#include <QtNetwork>
#include <QRegExp>


using domain::WebSkazkiHandler;


struct WebSkazkiHandler::Impl
{
    QList< domain::MetaInfo > media;
    QUrl preview;
    QString tittle;

    void parseMedia(const QString& buffer);
    void parseTittle(const QString& buffer);
    void parsePreview(const QString& buffer);
};


WebSkazkiHandler::WebSkazkiHandler(QObject* parent): 
    domain::AbstractPageHandler(parent),
    d(new Impl)
{}

WebSkazkiHandler::~WebSkazkiHandler()
{
    delete d;
}

void WebSkazkiHandler::httpReadyRead()
{
    QString buff = m_reply->readAll();
    d->parseMedia(buff);
    d->parseTittle(buff);
    d->parsePreview(buff);
}

QList< domain::MetaInfo > WebSkazkiHandler::media() const
{
    return d->media;
}

QUrl WebSkazkiHandler::preview() const
{
    return d->preview;
}

QString WebSkazkiHandler::tittle() const 
{
    return d->tittle;
}

void WebSkazkiHandler::Impl::parseMedia(const QString& buffer)
{
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
}

void WebSkazkiHandler::Impl::parseTittle(const QString& buffer)
{
    QRegExp regexp("<title>(Аудио\\s?сказк[иа]|Аудио\\s?книга)\\s«([А-ЯЁа-яё\\s-,]+)» слушать онлайн<\\/title>");
    
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        tittle = regexp.cap(2);
    }
}

void WebSkazkiHandler::Impl::parsePreview(const QString& buffer)
{
    QRegExp regexp("src=\"(https:\\/\\/web-skazki\\.ru\\/preview-files\\/[a-zA-Z0-9\\\\/-]+\\-x\\-[0-9]+\\.(jpe?g|png))\"");
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        preview = regexp.cap(1);
    }
}
