#include <CMark.h>
#include <UI/StatusBar.h>
#include <ImageProcess/ImageProcess.h>
#include "EXIFResolver.h"
#include "Base/ImagePack.h"

#include <QBuffer>
#include <QString>

namespace CM
{
    namespace
    {
        std::unordered_map<size_t, std::shared_ptr<EXIFInfo>> g_LoadedInfos;

        [[deprecated]] std::unordered_map<size_t, std::promise<void>> g_ThreadFinishSignals;
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
        g_ThreadFinishSignals.clear();
        g_LoadImageCheckCode.clear();
    }

    std::tuple<bool, std::string> EXIFResolver::check(const size_t fileIndexCode)
    {
        const auto code = checkCode(fileIndexCode);

        bool status = true;
        std::string outputInfos{"Resolver Picture Info Success!"};
        switch (code)
        {
        case 0:
            status = true;
            outputInfos = "Resolver Picture Info Success!";
            std::cout << outputInfos << std::endl;
            break;
        case 1982:
            status = false;
            outputInfos = "No JPEG markers found in buffer, possibly invalid JPEG file!";
            std::cout << outputInfos << std::endl;
            break;
        case 1983:
            status = false;
            outputInfos = "No EXIF header found in JPEG file.";
            std::cout << outputInfos << std::endl;
            break;
        case 1985:
            status = false;
            outputInfos = "Byte alignment specified in EXIF file was unknown (not Motorola or Intel).";
            std::cout << outputInfos << std::endl;
            break;
        case 1986:
            status = false;
            outputInfos = "EXIF header was found, but data was corrupted.";
            std::cout << outputInfos << std::endl;
            break;
        default:
            break;
        }

        StatusBar::showMessage(QString(outputInfos.c_str()));
        return {status, outputInfos};
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
    }

    ExifInfoMap EXIFResolver::getExifInfo(const size_t index)
    {
        if (g_ThreadFinishSignals.count(index))
        {
            auto& exitSignal = g_ThreadFinishSignals.at(index);
            exitSignal.get_future().wait(); ///< 等待线程结束
            g_ThreadFinishSignals.erase(index);
            g_LoadImageCheckCode.erase(index);
        }

        if(g_LoadedInfos.count(index))
        {
            return resolverImageExif(g_LoadedInfos.at(index));
        }

        return {};
    }

    int EXIFResolver::checkCode(const size_t index)
    {
        if (g_ThreadFinishSignals.count(index))
        {
            auto& exitSignal = g_ThreadFinishSignals.at(index);
            exitSignal.get_future().wait(); ///< 等待线程结束
            g_ThreadFinishSignals.erase(index);
        }

        if (g_LoadImageCheckCode.count(index))
        {
            const auto code = g_LoadImageCheckCode.at(index);
            g_LoadImageCheckCode.erase(index);
            return code;
        }

        return 0;
    }
} // CM
