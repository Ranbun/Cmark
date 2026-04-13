#ifndef CAMERAMARK_EASYEXIFRESOLVER_H
#define CAMERAMARK_EASYEXIFRESOLVER_H

/**
 * @file EasyExifResolver.h
 * @brief 基于 easyexif 的 EXIF 解析器：从 ImagePack 的内存像素数据解析 EXIF，并填充项目内 EXIFInfo。
 */

#include "Resolver.h"

namespace CM
{
    /**
     * @brief Resolver 的具体实现；解析过程会按 ImagePack 约定加锁，避免与图像数据并发访问冲突。
     */
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
