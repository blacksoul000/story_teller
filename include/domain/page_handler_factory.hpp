#ifndef PAGE_HANDLER_FACTORY_HPP
#define PAGE_HANDLER_FACTORY_HPP


#include <QUrl>
#include <QSharedPointer>


namespace domain
{
    class IPageHandler;
    class PageHandlerFactory
    {
    public:
        virtual ~PageHandlerFactory() = default;

        static QSharedPointer< IPageHandler > create(const QUrl& url);

    private:
        PageHandlerFactory() = delete;
    };
}  // namespace domain

#endif  // PAGE_HANDLER_FACTORY_HPP