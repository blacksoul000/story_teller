#include "domain/ozornik_handler.hpp"

#include <QTextDocument>
#include <QtNetwork>
#include <QRegExp>


using domain::OzornikHandler;


struct OzornikHandler::Impl
{
    QList< domain::MetaInfo > media;
    QUrl preview;
    QString tittle;
    QString buffer;

    void parseMedia(const QString& buffer);
    void parseTittle(const QString& buffer);
    void parsePreview(const QString& buffer);
};


OzornikHandler::OzornikHandler(QObject* parent): 
    domain::AbstractPageHandler(parent),
    d(new Impl)
{}

OzornikHandler::~OzornikHandler()
{
    delete d;
}

void OzornikHandler::httpReadyRead()
{
    d->buffer.append(m_reply->readAll());
    d->parseMedia(d->buffer);
    d->parseTittle(d->buffer);
    d->parsePreview(d->buffer);
}

QList< domain::MetaInfo > OzornikHandler::media() const
{
    return d->media;
}

QUrl OzornikHandler::preview() const
{
    return d->preview;
}

QString OzornikHandler::tittle() const 
{
    return d->tittle;
}

void OzornikHandler::Impl::parseMedia(const QString& buffer)
{
    media.clear();

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
}

void OzornikHandler::Impl::parseTittle(const QString& buffer)
{
    QRegExp regexp("<title>(Аудио\\s?сказки|Аудио\\s?сказка|Аудио\\s?книга)?"
        "\\s?([А-ЯЁа-яё0-9a-zA-Z:;\\s\\-—_,\\(\\)\\?\\!«»]+)[\\.\\s\\|]*([сС]лушать)");

    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        tittle = regexp.cap(2);
    }
}

void OzornikHandler::Impl::parsePreview(const QString& buffer)
{
    QRegExp regexp("<img src=\"(https:\\/\\/ozornik\\.net\\/wp-content\\/uploads\\/[a-zA-Z0-9\\\\/-]+\\.jpe?g)\"");
    regexp.setMinimal(true);
    if (regexp.indexIn(buffer) != -1)
    {
        preview = regexp.cap(1);
    }
}
