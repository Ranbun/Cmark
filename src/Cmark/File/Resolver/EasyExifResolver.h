#ifndef CAMERAMARK_EASYEXIFRESOLVER_H
#define CAMERAMARK_EASYEXIFRESOLVER_H

#include "Resolver.h"

namespace CM
{
    class EasyExifResolver final : public Resolver
    {
    public:
        EasyExifResolver();
        ~EasyExifResolver() override;

        EasyExifResolver(const EasyExifResolver&) = delete;
        EasyExifResolver(const EasyExifResolver&&) = delete;

        EasyExifResolver& operator=(const EasyExifResolver&) = delete;
        EasyExifResolver& operator=(const EasyExifResolver&&) = delete;


        std::shared_ptr<EXIFInfo> resolver(const ImagePack& imagePack) override;
    };

} // CM

#endif //CAMERAMARK_EASYEXIFRESOLVER_H
