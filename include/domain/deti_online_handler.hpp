#ifndef DETI_ONLINE_HANDLER_HPP
#define DETI_ONLINE_HANDLER_HPP


#include "domain/abstract_page_handler.hpp"


namespace domain
{
    class DetiOnlineHandler : public AbstractPageHandler
    {
        Q_OBJECT
    public:
        DetiOnlineHandler(QObject* parent = nullptr);
        virtual ~DetiOnlineHandler();

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

#endif  // DETI_ONLINE_HANDLER_HPP
