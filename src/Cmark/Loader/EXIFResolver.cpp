#include <CMark.h>

#include <UI/StatusBar.h>

#include "EXIFResolver.h"

#include <QBuffer>
#include <QImageReader>

#include "FileLoad.h"

#include <QString>

#include "QExifImageHeader"

namespace CM
{
    namespace 
    {
        std::unordered_map<size_t, std::shared_ptr<EXIFInfo>> g_LoadedInfos;
        std::unordered_map<size_t, std::promise<void>> g_ThreadFinishSignals;
        std::unordered_map<size_t, int> g_LoadImageCheckCode;

        std::mutex g_InfoMutex;

    }

    ExifInfoMap EXIFResolver::resolverImageExif(const std::weak_ptr<CM::EXIFInfo>& infoPtr)
    {
        auto result = *infoPtr.lock();

        ExifInfoMap infoMaps;
        if (!result.Make.empty())
        {
            infoMaps.insert({ ExifKey::CameraMake,result.Make });
        }

        if (!result.Model.empty())
        {
            infoMaps.insert({ ExifKey::CameraModel,result.Model });
        }

        if (result.ImageWidth)
        {
            infoMaps.insert({ ExifKey::ImageWidth,std::to_string(result.ImageWidth) });
        }

        if (result.ImageHeight)
        {
            infoMaps.insert({ ExifKey::ImageHeight,std::to_string(result.ImageHeight) });
        }

        if (!result.DateTime.empty())
        {
            infoMaps.insert({ ExifKey::ImageDate,result.DateTime });
        }

        /// Exposure Time
        if (const auto inExposureTime = static_cast<unsigned int>(1.0 / result.ExposureTime);
            result.ExposureTime > 1e-5 && inExposureTime)
        {
            infoMaps.insert({ ExifKey::ExposureTime,std::string("1/") + std::to_string(inExposureTime) });
        }

        if (const auto fNumber = static_cast<int>(result.FNumber);
            fNumber)
        {
            std::string fStop = std::string("f/") + std::to_string(fNumber) + std::string(".") +
                std::to_string(static_cast<int>(result.FNumber * 10) % 10);


            infoMaps.insert({ ExifKey::FStop,fStop });
        }


        if (result.ISOSpeedRatings)
        {
            infoMaps.insert({ ExifKey::ISOSpeed,std::string("ISO") + std::to_string(result.ISOSpeedRatings) });
        }

        if (!result.LensInfo.Model.empty())
        {
            infoMaps.insert({ ExifKey::LensModel,result.LensInfo.Model });
        }

        /// TODO: we need resolver all info and write it to ExifMap and output it
        if (const auto shutterSpeed = static_cast<int>(1.0 / result.ExposureTime);
            result.ExposureTime > 1e-5 && shutterSpeed)
        {
            infoMaps.insert({ ExifKey::ShutterSpeed,std::to_string(shutterSpeed)});
        }

        /// Focal Length
        if (const auto focalLength = static_cast<int>(result.FocalLength); focalLength)
        {
            infoMaps.insert({ ExifKey::FocalLength,std::to_string(focalLength) + "mm" });
        }
        return infoMaps;
    }

    template <>
    size_t EXIFResolver::hash<std::string>(const std::string& path)
    {
        constexpr std::hash<std::string> hasher;
        const size_t hashValue = hasher(path);
        return hashValue;
    }

    void EXIFResolver::destory()
    {
        g_LoadedInfos.clear();
        g_ThreadFinishSignals.clear();
        g_LoadImageCheckCode.clear();
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

        StatusBar::showMessage(QString(outputInfos.c_str()));
        return {status,outputInfos};
    }

    size_t EXIFResolver::resolver(const std::filesystem::path &path)
    {
        assert(this);

        auto hashValue = this->hash(path.string());
        /// load file
        auto loadImageFile = [](std::promise<void> & exitSignal, const std::filesystem::path & path, size_t fileHashValue)
        {
            auto loadDataPtr = FileLoad::load(path);

#if _DEBUG >> 1

            QByteArray imageData(reinterpret_cast<const char*>(loadDataPtr->data()),
                static_cast<int>(loadDataPtr->size()));

            // Create a QBuffer to treat the QByteArray as a QIODevice
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::ReadOnly);

            // Create a QImageReader and set the device to the QBuffer
            QImageReader imageReader(&buffer);

            // Read the image from the QImageReader
            QImage image = imageReader.read();

            QExifImageHeader readerExifHeader;

            // readerExifHeader.loadFromJpeg(path.string().c_str());
            readerExifHeader.loadFromJpeg(&buffer);

            // *readerFile = readerExifHeader.thumbnail();
            auto imagetags = readerExifHeader.imageTags();
            auto gpstags = readerExifHeader.gpsTags();
            auto extendedtags = readerExifHeader.extendedTags();

            auto width = readerExifHeader.value(QExifImageHeader::Make).toString();

            auto ers = image.save("test.png", "png", 1);


#endif 


            easyexif::EXIFInfo EXIFResolver;
            auto exifCheckCode = EXIFResolver.parseFrom(loadDataPtr->data(),loadDataPtr->size());
            loadDataPtr->clear();
            loadDataPtr.reset();

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

                in->FocalLength = out.FocalLength;  ////< 焦距

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

            std::lock_guard<std::mutex> local(g_InfoMutex);
            g_LoadedInfos.insert({fileHashValue,outputExIFInfos});
            g_LoadImageCheckCode.insert({fileHashValue,exifCheckCode});

            exitSignal.set_value();
            {
                g_ThreadFinishSignals.erase(fileHashValue);
            }
        };

        std::promise<void> exitSignal;
        g_ThreadFinishSignals.insert({hashValue,std::move(exitSignal)});
        std::thread loading(loadImageFile,std::ref(g_ThreadFinishSignals.at(hashValue)),std::ref(path),hashValue);
        loading.detach();

        return hashValue;
    }

    std::weak_ptr<EXIFInfo> EXIFResolver::getExifInfo(const size_t index) const
    {
        assert(this);  /// TODO: maybe remove it

        if(g_ThreadFinishSignals.count(index))
        {
            auto & exitSignal = g_ThreadFinishSignals.at(index);
            exitSignal.get_future().wait();   ///< 等待线程结束
            g_ThreadFinishSignals.erase(index);
            g_LoadImageCheckCode.erase(index);
        }
        /// 获取图片结果
        return g_LoadedInfos.at(index);
    }

    int EXIFResolver::checkCode(const size_t index) const
    {
        assert(this);   /// TODO: maybe remove it

        if(g_ThreadFinishSignals.count(index))
        {
            auto & exitSignal = g_ThreadFinishSignals.at(index);
            exitSignal.get_future().wait();   ///< 等待线程结束
            g_ThreadFinishSignals.erase(index);
        }

        if(g_LoadImageCheckCode.count(index))
        {
            const auto code = g_LoadImageCheckCode.at(index);
            g_LoadImageCheckCode.erase(index);
            return code;
        }

        return PARSE_EXIF_SUCCESS;
    }

} // CM
