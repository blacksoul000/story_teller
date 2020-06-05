#include "domain/page_handler_factory.hpp"

#include "domain/deti_online_handler.hpp"
#include "domain/web_skazki_handler.hpp"
#include "domain/ozornik_handler.hpp"


using namespace domain;


QSharedPointer< IPageHandler > PageHandlerFactory::create(const QUrl& url)
{
    if (!url.isValid()) return nullptr;

    using PageHandlerPtr = QSharedPointer< IPageHandler >;

    const QString host = url.host();
    if (host == "deti-online.com") return PageHandlerPtr(new domain::DetiOnlineHandler());
    if (host == "web-skazki.ru") return PageHandlerPtr(new domain::WebSkazkiHandler());
    if (host == "ozornik.net") return PageHandlerPtr(new domain::OzornikHandler());
    return nullptr;
}
