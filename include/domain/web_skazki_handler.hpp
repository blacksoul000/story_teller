#ifndef WEB_SKAZKI_HANDLER_HPP
#define WEB_SKAZKI_HANDLER_HPP


#include "domain/i_page_handler.hpp"


namespace domain
{
    class WebSkazkiHandler : public IPageHandler
    {
    public:
        QList< MetaInfo > parseMedia(const QString& buffer) const override;
        QUrl parsePreview(const QString& buffer) const override;
        QString parseTittle(const QString& buffer) const override;
    };
    
}  // namespace domain

#endif  // WEB_SKAZKI_HANDLER_HPP
