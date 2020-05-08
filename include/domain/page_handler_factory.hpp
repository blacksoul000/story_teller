#ifndef PAGE_HANDLER_FACTORY_HPP
#define PAGE_HANDLER_FACTORY_HPP


#include <QUrl>


namespace domain
{
    class AbstractPageHandler;
    class PageHandlerFactory
    {
    public:
        virtual ~PageHandlerFactory() = default;

        static AbstractPageHandler* create(const QUrl& url, QObject* parent = nullptr);

    private:
        PageHandlerFactory() = delete;
    };
}  // namespace domain

#endif  // PAGE_HANDLER_FACTORY_HPP