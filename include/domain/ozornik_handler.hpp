#ifndef OZORNIK_HANDLER_HPP
#define OZORNIK_HANDLER_HPP


#include "domain/abstract_page_handler.hpp"


namespace domain
{
    class OzornikHandler : public AbstractPageHandler
    {
        Q_OBJECT
    public:
        OzornikHandler(QObject* parent = nullptr);
        virtual ~OzornikHandler();

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

#endif  // OZORNIK_HANDLER_HPP
