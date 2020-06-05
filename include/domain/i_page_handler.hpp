#ifndef I_PAGE_HANDLER_HPP
#define I_PAGE_HANDLER_HPP

#include "domain/meta_info.hpp"

#include <QString>
#include <QUrl>

namespace domain
{
    class IPageHandler
    {
    public:
        IPageHandler() = default;
        ~IPageHandler() = default;

        virtual QList< MetaInfo > parseMedia(const QString& buffer) const = 0;
        virtual QString parseTittle(const QString& buffer) const = 0;
        virtual QUrl parsePreview(const QString& buffer) const = 0;
    };
} // namespace domain

#endif // I_PAGE_HANDLER_HPP