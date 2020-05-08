#ifndef ABSTRACT_PAGE_HANDLER_HPP
#define ABSTRACT_PAGE_HANDLER_HPP


#include "domain/meta_info.hpp"

#include <QNetworkAccessManager>
#include <QUrl>


namespace domain
{
    class AbstractPageHandler : public QObject
    {
        Q_OBJECT
    public:
        AbstractPageHandler(QObject* parent = nullptr);
        virtual ~AbstractPageHandler(){}
        void startRequest(const QUrl& url);

        virtual QList< MetaInfo > media() const = 0;
        virtual QUrl preview() const = 0;
        virtual QString tittle() const = 0;

    public slots:
        void cancel();

    signals:
        void finished();

    protected slots:
        virtual void slotAuthenticationRequired(QNetworkReply*, QAuthenticator*);
        virtual void httpReadyRead();

    protected slots:
        void httpFinished();

    protected:
        QUrl m_url;
        QNetworkAccessManager m_qnam;
        QNetworkReply* m_reply = nullptr;
        bool m_httpRequestAborted = false;
    };
}  // namespace domain

#endif  // ABSTRACT_PAGE_HANDLER_HPP