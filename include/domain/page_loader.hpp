#ifndef PAGE_LOADER_HPP
#define PAGE_LOADER_HPP

#include "domain/meta_info.hpp"
#include "domain/i_page_handler.hpp"

#include <QNetworkAccessManager>
#include <QUrl>


namespace domain
{
    class PageLoader : public QObject
    {
        Q_OBJECT
    public:
        PageLoader(QObject* parent = nullptr);
        virtual ~PageLoader(){}
        void startRequest(const QUrl& url);

        virtual QList< MetaInfo > media() const;
        virtual QUrl preview() const;
        virtual QString tittle() const;

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
        QString m_buffer;
        QUrl m_url;
        QNetworkAccessManager m_qnam;
        QNetworkReply* m_reply = nullptr;
        bool m_httpRequestAborted = false;
        QSharedPointer< domain::IPageHandler > m_handler;
    };
}  // namespace domain

#endif  // PAGE_LOADER_HPP