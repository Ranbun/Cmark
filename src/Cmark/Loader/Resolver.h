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

        virtual  std::shared_ptr<EXIFInfo> resolver(const ImagePack & imagePack) = 0;
    };
}


#endif // !CMARK_RESOLVER_H_
