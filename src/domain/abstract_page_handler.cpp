#include "domain/abstract_page_handler.hpp"

#include <QtNetwork>


using domain::AbstractPageHandler;


AbstractPageHandler::AbstractPageHandler(QObject* parent): 
    QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
    connect(&m_qnam, &QNetworkAccessManager::authenticationRequired,
            this, &AbstractPageHandler::slotAuthenticationRequired);
}

void AbstractPageHandler::startRequest(const QUrl& url)
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

    m_reply = m_qnam.get(QNetworkRequest(url));
    m_reply->ignoreSslErrors();
    connect(m_reply, &QNetworkReply::finished, this, &AbstractPageHandler::httpFinished);
    connect(m_reply, &QNetworkReply::finished, this, &AbstractPageHandler::finished);
    connect(m_reply, &QIODevice::readyRead, this, &AbstractPageHandler::httpReadyRead);
}

void AbstractPageHandler::cancel()
{
    qDebug() << Q_FUNC_INFO;
    m_httpRequestAborted = true;
    m_reply->abort();
}

void AbstractPageHandler::httpFinished()
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

void AbstractPageHandler::httpReadyRead()
{}

void AbstractPageHandler::slotAuthenticationRequired(QNetworkReply* ,QAuthenticator*)
{}
