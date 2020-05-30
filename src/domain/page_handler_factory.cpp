#include "domain/page_handler_factory.hpp"

#include "domain/deti_online_handler.hpp"
#include "domain/web_skazki_handler.hpp"
#include "domain/ozornik_handler.hpp"


using namespace domain;


AbstractPageHandler* PageHandlerFactory::create(const QUrl& url, QObject* parent)
{
    if (!url.isValid()) return nullptr;

    const QString host = url.host();
    if (host == "deti-online.com") return new domain::DetiOnlineHandler(parent);
    if (host == "web-skazki.ru") return new domain::WebSkazkiHandler(parent);
    if (host == "ozornik.net") return new domain::OzornikHandler(parent);
    return nullptr;
}
