#ifndef CAMERAMARK_EXIFRESOLVER_H
#define CAMERAMARK_EXIFRESOLVER_H

#include <QObject>

#include "Base/ExifInfo.h"
#include "Base/Type.h"
#include "Base/ImagePack.h"

#include "EasyExifResolver.h"


namespace CM
{
    namespace Tools
    {
        class ResourcesTools;
    }

    class EXIFResolver : public QObject
    {
        friend  class Tools::ResourcesTools;
    public:
        EXIFResolver() =default;

        /**
         * @brief 解析错误码
         * @param checkCode checkCode函数的返回值
         * @return 结果和错误信息
         */
        static std::tuple<bool, std::string> check(int checkCode);

        /// ---------------------------------new resolver interface --------------------------------------------------

        /**
         * @brief 加载图片并解析相应的exif信息
         * @param pack 图片路径
         * @return 加载的图片的索引 可以通过此索引获取加载的exif信息
         */
        static void resolver(const ImagePack &pack, bool synchronization = true);


        // static void resolver(const ImagePack &pack);

        /**
         * @brief 通过加载时候返回的索引获取加载完成的图片数据
         * @param index 加载时候返回的索引
         * @return 图片数据Exif信息
         */
        static ExifInfoMap getExifInfo(size_t index);

    public:

        /**
         * @brief 解析加载完成的exif信息 返回可用的信息
         * @param infoPtr 加载的对应的文件的exif信息
         * @return
         */
        static ExifInfoMap resolverImageExif(const std::weak_ptr<CM::EXIFInfo>& infoPtr);

        static void destroyCache();

    private:

        static std::shared_ptr<Resolver> m_ExifInfoResolver;

    };

} // CM

#endif //CAMERAMARK_EXIFRESOLVER_H
