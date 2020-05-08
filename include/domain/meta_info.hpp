#ifndef META_INFO_HPP
#define META_INFO_HPP


#include <QUrl>


namespace domain
{
    struct MetaInfo
    {
        QString tittle;
        QUrl url;
        double duration;
        QUrl preview;
    };
}  // namespace domain

#endif  // META_INFO_HPP