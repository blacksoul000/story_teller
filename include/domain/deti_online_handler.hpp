#ifndef DETI_ONLINE_HANDLER_HPP
#define DETI_ONLINE_HANDLER_HPP


#include "domain/i_page_handler.hpp"


namespace domain
{
    class DetiOnlineHandler : public IPageHandler
    {
    public:
        QList< MetaInfo > parseMedia(const QString& buffer) const override;
        QUrl parsePreview(const QString& buffer) const override;
        QString parseTittle(const QString& buffer) const override;
    };
}  // namespace domain

#endif  // DETI_ONLINE_HANDLER_HPP
