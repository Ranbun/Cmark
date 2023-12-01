#ifndef CAMERAMARK_EXIFRESOLVER_H
#define CAMERAMARK_EXIFRESOLVER_H

#include "exif.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

#include "../Base/Type.h"

namespace CM
{
    class EXIFResolver
    {
    public:
        EXIFResolver() =default;
        [[maybe_unused]] int resolver(const std::vector<unsigned  char > & pictureData);
        [[nodiscard]] const easyexif::EXIFInfo & getInfos() const {return m_EXIFResolver;}

        static void check(int resolverCode);

        [[maybe_unused]] static ExifList resolverImageExif(const easyexif::EXIFInfo & result);

    private:
        easyexif::EXIFInfo m_EXIFResolver;
    };


} // CM

#endif //CAMERAMARK_EXIFRESOLVER_H
