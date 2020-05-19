#ifndef WEB_SKAZKI_HANDLER_HPP
#define WEB_SKAZKI_HANDLER_HPP


#include "domain/abstract_page_handler.hpp"


namespace domain
{
    class WebSkazkiHandler : public AbstractPageHandler
    {
        Q_OBJECT
    public:
        WebSkazkiHandler(QObject* parent = nullptr);
        virtual ~WebSkazkiHandler();

        QList< MetaInfo > media() const override;
        QUrl preview() const override;
        QString tittle() const  override;

    protected slots:
        void httpReadyRead() override;

    private:
        struct Impl;
        Impl* d;
    };
    
}  // namespace domain

#endif  // WEB_SKAZKI_HANDLER_HPP
