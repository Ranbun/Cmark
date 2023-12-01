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
        infoMaps.emplace_back(ExifPack{ExifKey::Lens_Model,result.LensInfo.Model});

        /// TODO: we need resolver all info and write it to ExifMap and output it
        infoMaps.emplace_back(ExifPack{ExifKey::Shutter_speed,std::to_string((int)(1.0 / result.ExposureTime))});


        return std::move(infoMaps);
    }

    void EXIFResolver::check(int resolverCode)
    {
        switch(resolverCode)
        {
            case PARSE_EXIF_SUCCESS:
                std::cout<<"Resolver Picture Info Success!"<<std::endl;
                break;
            case PARSE_EXIF_ERROR_NO_JPEG:
                std::cout<<"No JPEG markers found in buffer, possibly invalid JPEG file!"<<std::endl;
                break;
            case PARSE_EXIF_ERROR_NO_EXIF:
                std::cout<<"No EXIF header found in JPEG file."<<std::endl;
                break;
            case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
                std::cout<<"Byte alignment specified in EXIF file was unknown (not Motorola or Intel)."<<std::endl;
                break;
            case PARSE_EXIF_ERROR_CORRUPT:
                std::cout<<"EXIF header was found, but data was corrupted."<<std::endl;
                break;
            default:
                break;
        }
    }
} // CM
