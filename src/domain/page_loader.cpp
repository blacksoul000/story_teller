#include "domain/page_loader.hpp"
#include "domain/page_handler_factory.hpp"

#include <QtNetwork>


using domain::PageLoader;


PageLoader::PageLoader(QObject* parent): 
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    connect(&m_qnam, &QNetworkAccessManager::authenticationRequired,
            this, &PageLoader::slotAuthenticationRequired);
}

void PageLoader::startRequest(const QUrl& url)
{
    qDebug() << Q_FUNC_INFO;
    if (m_reply) 
    {
        qDebug() << "Already busy";
        return;
    }
    if (!url.isValid())
    {
        qDebug() << "Url is not valid";
        return;
    }
    m_url = url;
    m_httpRequestAborted = false;
    m_handler = domain::PageHandlerFactory::create(m_url);

    m_reply = m_qnam.get(QNetworkRequest(url));
    m_reply->ignoreSslErrors();
    connect(m_reply, &QNetworkReply::finished, this, &PageLoader::httpFinished);
    connect(m_reply, &QNetworkReply::finished, this, &PageLoader::finished);
    connect(m_reply, &QIODevice::readyRead, this, &PageLoader::httpReadyRead);
}

void PageLoader::cancel()
{
    qDebug() << Q_FUNC_INFO;
    m_httpRequestAborted = true;
    m_reply->abort();
}

void PageLoader::httpFinished()
{
    qDebug() << Q_FUNC_INFO;
    if (m_httpRequestAborted) 
    {
        m_reply->deleteLater();
        m_reply = nullptr;
        return;
    }

    if (m_reply->error()) 
    {
        m_reply->deleteLater();
        m_reply = nullptr;
        return;
    }

    const QVariant redirectionTarget = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    m_reply->deleteLater();
    m_reply = nullptr;

    if (!redirectionTarget.isNull()) {
        const QUrl redirectedUrl = m_url.resolved(redirectionTarget.toUrl());
        this->startRequest(redirectedUrl);
        return;
    }
}

void PageLoader::httpReadyRead()
{
    m_buffer.append(m_reply->readAll());
}

QList< domain::MetaInfo > PageLoader::media() const
{
    return m_handler ? m_handler->parseMedia(m_buffer) : QList< MetaInfo >();
}

QUrl PageLoader::preview() const
{
    return m_handler ? m_handler->parsePreview(m_buffer) : QUrl();
}

QString PageLoader::tittle() const
{
    return m_handler ? m_handler->parseTittle(m_buffer) : QString();
}


void PageLoader::slotAuthenticationRequired(QNetworkReply* ,QAuthenticator*)
{}
