#include "EasyExifResolver.h"
#include "exif.h"

namespace CM
{
    EasyExifResolver::EasyExifResolver()  = default;
    EasyExifResolver::~EasyExifResolver()  = default;

    std::shared_ptr<EXIFInfo> EasyExifResolver::resolver(const ImagePack& imagePack)
    {
        easyexif::EXIFInfo exifResolver;
        const auto imagePixels = imagePack.m_ImageData;
        const auto data = imagePixels->constData();

        std::unique_lock localMutex(*imagePack.m_LocalMutex);

        const auto exifCheckCode = exifResolver.parseFrom(reinterpret_cast<const unsigned char*>(data),
                                                          imagePixels->size());
        localMutex.unlock();


        auto outputExIfInfos = std::make_shared<EXIFInfo>();
        outputExIfInfos->m_LoadedCheckCode = exifCheckCode;
        {
            const auto& in = outputExIfInfos;
            const easyexif::EXIFInfo& out = exifResolver;

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

            in->FocalLength = out.FocalLength; ////< 焦距

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

        return outputExIfInfos;
    }
} // CM
