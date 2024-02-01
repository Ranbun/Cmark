#include "CMark.h"
#include <Window/StatusBar.h>
#include <File/ImageProcess/ImageProcess.h>
#include "EXIFResolver.h"
#include "Base/ImagePack.h"
#include "Log/CLog.h"

#include <QBuffer>
#include <QString>

namespace CM
{
    namespace
    {
        std::unordered_map<size_t, std::shared_ptr<EXIFInfo>> g_LoadedInfos;
        std::unordered_map<size_t, int> g_LoadImageCheckCode;
        std::mutex g_InfoMutex;
    }

    std::shared_ptr<Resolver> EXIFResolver::m_ExifInfoResolver = std::make_shared<EasyExifResolver>();

    ExifInfoMap EXIFResolver::resolverImageExif(const std::weak_ptr<EXIFInfo>& infoPtr)
    {
        auto result = *infoPtr.lock();

        ExifInfoMap infoMaps;
        if (!result.Make.empty())
        {
            infoMaps.insert({ExifKey::CameraMake, result.Make});
        }

        if (!result.Model.empty())
        {
            infoMaps.insert({ExifKey::CameraModel, result.Model});
        }

        if (result.ImageWidth)
        {
            infoMaps.insert({ExifKey::ImageWidth, std::to_string(result.ImageWidth)});
        }

        if (result.ImageHeight)
        {
            infoMaps.insert({ExifKey::ImageHeight, std::to_string(result.ImageHeight)});
        }

        if (!result.DateTime.empty())
        {
            infoMaps.insert({ExifKey::ImageDate, result.DateTime});
        }

        /// Exposure Time
        if (const auto inExposureTime = static_cast<unsigned int>(1.0 / result.ExposureTime);
            result.ExposureTime > 1e-5 && inExposureTime)
        {
            infoMaps.insert({ExifKey::ExposureTime, std::string("1/") + std::to_string(inExposureTime)});
        }

        if (const auto fNumber = static_cast<int>(result.FNumber);
            fNumber)
        {
            std::string fStop = std::string("f/") + std::to_string(fNumber) + std::string(".") +
                std::to_string(static_cast<int>(result.FNumber * 10) % 10);


            infoMaps.insert({ExifKey::FStop, fStop});
        }


        if (result.ISOSpeedRatings)
        {
            infoMaps.insert({ExifKey::ISOSpeed, std::string("ISO") + std::to_string(result.ISOSpeedRatings)});
        }

        if (!result.LensInfo.Model.empty())
        {
            infoMaps.insert({ExifKey::LensModel, result.LensInfo.Model});
        }

        /// TODO: we need resolver all info and write it to ExifMap and output it
        if (const auto shutterSpeed = static_cast<int>(1.0 / result.ExposureTime);
            result.ExposureTime > 1e-5 && shutterSpeed)
        {
            infoMaps.insert({ExifKey::ShutterSpeed, std::to_string(shutterSpeed)});
        }

        /// Focal Length
        if (const auto focalLength = static_cast<int>(result.FocalLength); focalLength)
        {
            infoMaps.insert({ExifKey::FocalLength, std::to_string(focalLength) + "mm"});
        }
        return std::move(infoMaps);
    }

    void EXIFResolver::destroyCache()
    {
        g_LoadedInfos.clear();
        g_LoadImageCheckCode.clear();
    }

    std::tuple<bool, std::string> EXIFResolver::check(const int checkCode)
    {
        bool status = true;
        std::string outputInfos{"Resolver Picture Info Success!"};
        switch (checkCode)
        {
        case 0:
            status = true;
            outputInfos = "Resolver Picture Info Success!";
            break;
        case 1982:
            status = false;
            outputInfos = "No JPEG markers found in buffer, possibly invalid JPEG file!";
            break;
        case 1983:
            status = false;
            outputInfos = "No EXIF header found in JPEG file.";
            break;
        case 1985:
            status = false;
            outputInfos = "Byte alignment specified in EXIF file was unknown (not Motorola or Intel).";
            break;
        case 1986:
            status = false;
            outputInfos = "EXIF header was found, but data was corrupted.";
            break;
        default:
            break;
        }
        // CM::CLog::Info(QString::fromStdString(outputInfos));
        // StatusBar::showMessage(QString(outputInfos.c_str()));
        return {status,outputInfos};
    }

    void EXIFResolver::resolver(const ImagePack &pack, bool synchronization)
    {
        {
            std::lock_guard<std::mutex> local(g_InfoMutex);
            if(g_LoadedInfos.count(pack.m_FileIndexCode))
            {
                return;
            }
        }

        auto loadImageFile = [](const ImagePack& imagePack)
        {
            auto outputExIfInfos = m_ExifInfoResolver->resolver(imagePack);
            std::lock_guard<std::mutex> local(g_InfoMutex);
            g_LoadedInfos.insert({imagePack.m_FileIndexCode, outputExIfInfos});
            g_LoadImageCheckCode.insert({imagePack.m_FileIndexCode, outputExIfInfos->m_LoadedCheckCode});
        };

        if(!synchronization)
        {
            std::thread loading(loadImageFile, std::ref(pack));
            loading.join();
        }
        else
        {
            loadImageFile(pack);
        }

        auto loadStatusCode = 0;
        {
            std::lock_guard local(g_InfoMutex);
            if (g_LoadImageCheckCode.count(pack.m_FileIndexCode))
            {
                loadStatusCode = g_LoadImageCheckCode.at(pack.m_FileIndexCode);
            }
        }

        auto [status,infos] = check(loadStatusCode);
        if(status)
        {
            CLog::Info(QString::fromStdString(infos));
            return;
        }
        CLog::Warning(QString::fromStdString(infos));
    }

    ExifInfoMap EXIFResolver::getExifInfo(const size_t index)
    {
        if(g_LoadedInfos.count(index))
        {
            return resolverImageExif(g_LoadedInfos.at(index));
        }

        return {};
    }

    std::string EXIFResolver::ExifItem(size_t fileIndexCode, ExifKey key)
    {
        const auto exifInfos = getExifInfo(fileIndexCode);
        auto res = exifInfos.count(ExifKey::CameraMake)? exifInfos.at(key):"";
        return res;
    }

} // CM
