#include "domain/page_handler_factory.hpp"

#include "domain/deti_online_handler.hpp"


using namespace domain;


AbstractPageHandler* PageHandlerFactory::create(const QUrl& url, QObject* parent)
{
    if (!url.isValid()) return nullptr;

    const QString host = url.host();
    if (host == "deti-online.com") return new domain::DetiOnlineHandler(parent);
    return nullptr;
}
