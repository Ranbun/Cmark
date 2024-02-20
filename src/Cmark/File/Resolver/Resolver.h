#ifndef CMARK_RESOLVER_H_
#define CMARK_RESOLVER_H_

#include "Base/ExifInfo.h"
#include "Base/ImagePack.h"
#include <memory>

namespace CM
{
    class Resolver
    {
    public:
        Resolver() = default;
        virtual ~Resolver() = default;

        Resolver(const Resolver&) = delete;
        Resolver(const Resolver&&) = delete;

        Resolver& operator=(const Resolver&) = delete;
        Resolver& operator=(const Resolver&&) = delete;

        virtual  std::shared_ptr<EXIFInfo> resolver(const ImagePack & imagePack) = 0;
    };
}


#endif // !CMARK_RESOLVER_H_
