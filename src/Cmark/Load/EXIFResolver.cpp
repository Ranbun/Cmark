#include "EXIFResolver.h"

namespace CM
{
    [[maybe_unused]] int EXIFResolver::resolver(const std::vector<unsigned char> &pictureData)
    {
        return m_EXIFResolver.parseFrom(pictureData.data(), pictureData.size());
    }

    ExifList EXIFResolver::resolverImageExif(const easyexif::EXIFInfo &result)
    {
        ExifList infoMaps;
        infoMaps.emplace_back(ExifPack{ExifKey::Camera_make,result.Make});
        infoMaps.emplace_back(ExifPack{ExifKey::Camera_model,result.Model});
        infoMaps.emplace_back(ExifPack{ExifKey::Image_width,std::to_string(result.ImageWidth)});
        infoMaps.emplace_back(ExifPack{ExifKey::Image_height,std::to_string(result.ImageHeight)});
        infoMaps.emplace_back(ExifPack{ExifKey::Image_date,result.DateTime});

        /// Exposure Time
        auto inExposureTime = static_cast<unsigned int>(1.0 / result.ExposureTime);
        infoMaps.emplace_back(ExifPack{ExifKey::Exposure_time,"1/" + std::to_string(inExposureTime) + " s"});

        std::string f_stop = std::to_string(static_cast<int>(result.FNumber)) + "." +
                             std::to_string(static_cast<int>(result.FNumber * 10) % 10) + "f";
        infoMaps.emplace_back(ExifPack{ExifKey::F_stop,f_stop});

        infoMaps.emplace_back(ExifPack{ExifKey::ISO_speed,std::to_string(result.ISOSpeedRatings)});

        /// TODO: we need resolver all info and write it to ExifMap and output it

        return std::move(infoMaps);
    }
} // CM
