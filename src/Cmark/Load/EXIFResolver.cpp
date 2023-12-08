#include "EXIFResolver.h"
#include "FileLoad.h"

#include <UI/StatusBar.h>

#include <tuple>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <future>

namespace CM
{
    static std::unordered_map<size_t,std::shared_ptr<EXIFInfo>> loadedInfos;
    static std::unordered_map<size_t,std::promise<void>> threadFinishSignals;
    static std::unordered_map<size_t,int> loadImageCheckCode;

    static std::mutex infoMutex;

    ExifInfoMap EXIFResolver::resolverImageExif(const std::weak_ptr<CM::EXIFInfo>& infoPtr)
    {
        auto result = *infoPtr.lock();

        ExifInfoMap infoMaps;
        infoMaps.insert({ExifKey::Camera_make,result.Make});
        infoMaps.insert({ExifKey::Camera_model,result.Model});
        infoMaps.insert({ExifKey::Image_width,std::to_string(result.ImageWidth)});
        infoMaps.insert({ExifKey::Image_height,std::to_string(result.ImageHeight)});
        infoMaps.insert({ExifKey::Image_date,result.DateTime});

        /// Exposure Time
        auto inExposureTime = static_cast<unsigned int>(1.0 / result.ExposureTime);
        infoMaps.insert({ExifKey::Exposure_time,std::string("1/") + std::to_string(inExposureTime)});

        std::string f_stop = std::string("f/") + std::to_string(static_cast<int>(result.FNumber)) + std::string(".") +
                             std::to_string(static_cast<int>(result.FNumber * 10) % 10);
        infoMaps.insert({ExifKey::F_stop,f_stop});

        infoMaps.insert({ExifKey::ISO_speed,std::string("ISO") + std::to_string(result.ISOSpeedRatings)});
        infoMaps.insert({ExifKey::Lens_Model,result.LensInfo.Model});

        /// TODO: we need resolver all info and write it to ExifMap and output it
        infoMaps.insert({ExifKey::Shutter_speed,std::to_string((int)(1.0 / result.ExposureTime))});

        return std::move(infoMaps);
    }


    std::tuple<bool, std::string> EXIFResolver::check(int resolverCode)
    {
        bool status = false;
        std::string outputInfos{"Resolver Picture Info Success!"};
        switch(resolverCode)
        {
            case PARSE_EXIF_SUCCESS:
                status = true;
                outputInfos = "Resolver Picture Info Success!";
                std::cout<<outputInfos<<std::endl;
                break;
            case PARSE_EXIF_ERROR_NO_JPEG:
                status = false;
                outputInfos = "No JPEG markers found in buffer, possibly invalid JPEG file!";
                std::cout<<outputInfos<<std::endl;
                break;
            case PARSE_EXIF_ERROR_NO_EXIF:
                status = false;
                outputInfos = "No EXIF header found in JPEG file.";
                std::cout<<outputInfos<<std::endl;
                break;
            case PARSE_EXIF_ERROR_UNKNOWN_BYTEALIGN:
                status = false;
                outputInfos = "Byte alignment specified in EXIF file was unknown (not Motorola or Intel).";
                std::cout<<outputInfos<<std::endl;
                break;
            case PARSE_EXIF_ERROR_CORRUPT:
                status = false;
                outputInfos = "EXIF header was found, but data was corrupted.";
                std::cout<<outputInfos<<std::endl;
                break;
            default:
                break;
        }

        CM::StatusBar::showMessage(outputInfos.c_str());
        return {status,outputInfos};
    }

    size_t EXIFResolver::resolver(const std::filesystem::path &path)
    {
        assert(this);

        std::hash<std::filesystem::path> Hasher;
        size_t hashValue = Hasher(path);

        /// load file
        auto loadImageFile = [](std::promise<void> & exitSignal, const std::filesystem::path & path, size_t fileHashValue){
            auto res = FileLoad::Load(path);

            easyexif::EXIFInfo EXIFResolver;
            auto exifCheckCode = EXIFResolver.parseFrom(res.data(),res.size());

            auto outputExIFInfos = std::make_shared<EXIFInfo>();
            {
                const auto & in = outputExIFInfos;
                const easyexif::EXIFInfo & out = EXIFResolver;

                in->ImageDescription = out.ImageDescription;
                in->Make = out.Make;
                in->Model = out.Model;
                in->Orientation = out.Orientation;
                in->BitsPerSample = out.BitsPerSample;

                in->DateTime = out.DateTime;
                in->DateTimeOriginal = out.DateTimeOriginal;
                in->DateTimeDigitized = out.DateTimeDigitized;
                in->SubSecTimeOriginal = out.SubSecTimeOriginal;
                in->Copyright = out.Copyright;

                in->ExposureTime = out.ExposureTime;
                in->FNumber = out.FNumber;
                in->ExposureProgram = out.ExposureProgram;

                in->ISOSpeedRatings = out.ISOSpeedRatings;
                in->ShutterSpeedValue = out.ShutterSpeedValue;

                in->ImageWidth = out.ImageWidth;
                in->ImageHeight = out.ImageHeight;

                in->LensInfo.Make = out.LensInfo.Make;
                in->LensInfo.Model = out.LensInfo.Model;
                /// TODO need add others
            }

            std::lock_guard<std::mutex> local(infoMutex);
            loadedInfos.insert({fileHashValue,outputExIFInfos});
            loadImageCheckCode.insert({fileHashValue,exifCheckCode});
            exitSignal.set_value();
        };

        std::promise<void> exitSignal;
        threadFinishSignals.insert({hashValue,std::move(exitSignal)});
        std::thread loading(loadImageFile,std::ref(threadFinishSignals.at(hashValue)),std::ref(path),hashValue);
        loading.detach();

        return hashValue;
    }

    std::weak_ptr<EXIFInfo> EXIFResolver::getExifInfo(size_t index)
    {
        assert(this);  /// TODO: maybe remove it

        if(threadFinishSignals.count(index))
        {
            auto & exitSignal = threadFinishSignals.at(index);
            exitSignal.get_future().wait();   ///< 等待线程结束
            threadFinishSignals.erase(index);
        }
        /// 获取图片结果
        return loadedInfos.at(index);
    }

    int EXIFResolver::checkCode(size_t index)
    {
        assert(this);   /// TODO: maybe remove it

        if(threadFinishSignals.count(index))
        {
            auto & exitSignal = threadFinishSignals.at(index);
            exitSignal.get_future().wait();   ///< 等待线程结束
            threadFinishSignals.erase(index);
        }

        if(loadImageCheckCode.count(index))
        {
            auto Code = loadImageCheckCode.at(index);
            loadImageCheckCode.erase(index);
            return Code;
        }

        return PARSE_EXIF_SUCCESS;
    }

} // CM
