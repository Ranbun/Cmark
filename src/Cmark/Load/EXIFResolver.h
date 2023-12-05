#ifndef CAMERAMARK_EXIFRESOLVER_H
#define CAMERAMARK_EXIFRESOLVER_H

#include "exif.h"

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include "Base/Type.h"

namespace CM
{
    class EXIFResolver
    {
    public:
        EXIFResolver() =default;
        /**
         * @brief 解析图片中的信息
         * @param pictureData 从文件中读取的数据
         * @return 解析码 用于判断是否错误以及错误的信息
         */
        [[maybe_unused]] int resolver(const std::vector<unsigned  char > & pictureData);

        /**
         * @brief 获取解析完成的exif信息
         * @return exif 信息
         */
        [[nodiscard]] const easyexif::EXIFInfo & getInfos() const {return m_EXIFResolver;}

        /**
         * @brief 解析错误码
         * @param resolverCode resolver函数的返回值
         * @return 结果和错误信息
         */
        static std::tuple<bool, std::string> check(int resolverCode);

        /**
         * @brief 转换解析的信息为内部可用信息
         * @param result 解析的图片的exif信息
         * @return 转换完成的结果
         */
        [[maybe_unused]] static ExifList resolverImageExif(const easyexif::EXIFInfo & result);

    private:
        easyexif::EXIFInfo m_EXIFResolver;
    };


} // CM

#endif //CAMERAMARK_EXIFRESOLVER_H
