#include <CMark.h>

#include <UI/StatusBar.h>
#include <ImageProcess/ImageProcess.h>

#include "EXIFResolver.h"

#include <QBuffer>
#include <QImageReader>

#include "FileLoad.h"

#include <QString>

#include "QExifImageHeader"
#include "qexifimageheader.h"

namespace CM
{
    namespace 
    {
        std::unordered_map<size_t, std::shared_ptr<EXIFInfo>> g_LoadedInfos;
        std::unordered_map<size_t, std::promise<void>> g_ThreadFinishSignals;
        std::unordered_map<size_t, int> g_LoadImageCheckCode;

        std::mutex g_InfoMutex;

        QList<QString> toQString(QExifValue & value)
        {
            switch (value.type())
            {
                case QExifValue::Type::Byte:
                {
                    const auto byte = value.toByte();
                    return { QString::number(byte) };
                }
                case QExifValue::Type::Ascii:
                {
                    return {value.toString()};
                }
                case QExifValue::Type::Short:
                {
                    const auto shortData = value.toShort();
                    return {QString::number(shortData)};
                }
                case QExifValue::Type::Long:
                {
                    const auto longData = value.toLong();
                    return {QString::number(longData)};
                }
                case QExifValue::Type::Rational:
                {
                    const auto data = value.toRational();
                    auto [f, s] = data;
                    return { QString::number(f),QString::number(s)};
                }
                case QExifValue::Type::Undefined:
                {
                    if(value.encoding() == QExifValue::TextEncoding::NoEncoding)
                    {
                        const auto array = value.toByteArray();
                        auto data = QString::fromUtf8(array);
                        return {data};
                    }
                    const auto array = value.toString();
                    return {array};
                }
                case QExifValue::Type::SignedLong:
                {
                    const auto data = value.toSignedLong();
                    return {QString::number(data)};
                }
                case QExifValue::Type::SignedRational:
                {
                    const auto map = value.toSignedRational();
                    const auto [f,s] = map;
                    return {QString::number(s),QString::number(s)};
                }
            }
        }

        std::string ImageTag2String(const QExifImageHeader & readerExifHeader,QExifImageHeader::ImageTag tag)
        {
            switch (tag)
            {
                case QExifImageHeader::ImageTag::Make:
                {
                    return readerExifHeader.value(tag).toString().toStdString();
                }
                // break;
                case QExifImageHeader::ImageTag::ImageWidth:
                    return {};
                case QExifImageHeader::ImageTag::ImageLength:
                case QExifImageHeader::ImageTag::BitsPerSample:
                case QExifImageHeader::ImageTag::Compression:
                case QExifImageHeader::ImageTag::PhotometricInterpretation:
                case QExifImageHeader::ImageTag::Orientation:
                case QExifImageHeader::ImageTag::SamplesPerPixel:
                case QExifImageHeader::ImageTag::PlanarConfiguration:
                case QExifImageHeader::ImageTag::YCbCrSubSampling:
                case QExifImageHeader::ImageTag::XResolution:
                case QExifImageHeader::ImageTag::YResolution:
                case QExifImageHeader::ImageTag::ResolutionUnit:
                case QExifImageHeader::ImageTag::StripOffsets:
                case QExifImageHeader::ImageTag::RowsPerStrip:
                case QExifImageHeader::ImageTag::StripByteCounts:
                case QExifImageHeader::ImageTag::TransferFunction:
                case QExifImageHeader::ImageTag::WhitePoint:
                case QExifImageHeader::ImageTag::PrimaryChromaciticies:
                case QExifImageHeader::ImageTag::YCbCrCoefficients:
                case QExifImageHeader::ImageTag::ReferenceBlackWhite:
                case QExifImageHeader::ImageTag::DateTime:
                case QExifImageHeader::ImageTag::ImageDescription:
                case QExifImageHeader::ImageTag::Model:
                case QExifImageHeader::ImageTag::Software:
                case QExifImageHeader::ImageTag::Artist:
                case QExifImageHeader::ImageTag::Copyright:
                    return toQString(readerExifHeader.value(tag))[0].toStdString();
                    break;
            }


            return "null";
        }

        std::string ExifExtendTagToString(const QExifImageHeader& readerExifHeader, QExifImageHeader::ExifExtendedTag tag)
        {
            switch (tag)
            {
            case QExifImageHeader::ExifExtendedTag::ExifVersion:
            case QExifImageHeader::ExifExtendedTag::FlashPixVersion:
            case QExifImageHeader::ExifExtendedTag::ColorSpace:
            case QExifImageHeader::ExifExtendedTag::ComponentsConfiguration:
            case QExifImageHeader::ExifExtendedTag::CompressedBitsPerPixel:
            case QExifImageHeader::ExifExtendedTag::PixelXDimension:
            case QExifImageHeader::ExifExtendedTag::PixelYDimension:
            case QExifImageHeader::ExifExtendedTag::UserComment:
            case QExifImageHeader::ExifExtendedTag::RelatedSoundFile:
            case QExifImageHeader::ExifExtendedTag::DateTimeOriginal:
            case QExifImageHeader::ExifExtendedTag::DateTimeDigitized:
            case QExifImageHeader::ExifExtendedTag::SubSecTime:
            case QExifImageHeader::ExifExtendedTag::SubSecTimeOriginal:
            case QExifImageHeader::ExifExtendedTag::SubSecTimeDigitized:
            case QExifImageHeader::ExifExtendedTag::ImageUniqueId:

            case QExifImageHeader::ExifExtendedTag::ExposureTime:
            case QExifImageHeader::ExifExtendedTag::FNumber:
            case QExifImageHeader::ExifExtendedTag::ExposureProgram:
            case QExifImageHeader::ExifExtendedTag::SpectralSensitivity:
            case QExifImageHeader::ExifExtendedTag::ISOSpeedRatings:
            case QExifImageHeader::ExifExtendedTag::Oecf:
            case QExifImageHeader::ExifExtendedTag::ShutterSpeedValue:
            case QExifImageHeader::ExifExtendedTag::ApertureValue:
            case QExifImageHeader::ExifExtendedTag::BrightnessValue:
            case QExifImageHeader::ExifExtendedTag::ExposureBiasValue:
            case QExifImageHeader::ExifExtendedTag::MaxApertureValue:
            case QExifImageHeader::ExifExtendedTag::SubjectDistance:
            case QExifImageHeader::ExifExtendedTag::MeteringMode:
            case QExifImageHeader::ExifExtendedTag::LightSource:
            case QExifImageHeader::ExifExtendedTag::Flash:
            case QExifImageHeader::ExifExtendedTag::FocalLength:
            case QExifImageHeader::ExifExtendedTag::SubjectArea:
            case QExifImageHeader::ExifExtendedTag::FlashEnergy:

            case QExifImageHeader::ExifExtendedTag::SpatialFrequencyResponse:
            case QExifImageHeader::ExifExtendedTag::FocalPlaneXResolution   :
            case QExifImageHeader::ExifExtendedTag::FocalPlaneYResolution   :
            case QExifImageHeader::ExifExtendedTag::FocalPlaneResolutionUnit:
            case QExifImageHeader::ExifExtendedTag::SubjectLocation         :
            case QExifImageHeader::ExifExtendedTag::ExposureIndex           :
            case QExifImageHeader::ExifExtendedTag::SensingMethod           :
            case QExifImageHeader::ExifExtendedTag::FileSource              :
            case QExifImageHeader::ExifExtendedTag::SceneType               :
            case QExifImageHeader::ExifExtendedTag::CfaPattern              :
            case QExifImageHeader::ExifExtendedTag::CustomRendered          :
            case QExifImageHeader::ExifExtendedTag::ExposureMode            :
            case QExifImageHeader::ExifExtendedTag::WhiteBalance            :
            case QExifImageHeader::ExifExtendedTag::DigitalZoomRatio        :
            case QExifImageHeader::ExifExtendedTag::FocalLengthIn35mmFilm   :
            case QExifImageHeader::ExifExtendedTag::SceneCaptureType        :
            case QExifImageHeader::ExifExtendedTag::GainControl             :
            case QExifImageHeader::ExifExtendedTag::Contrast                :
            case QExifImageHeader::ExifExtendedTag::Saturation              :
            case QExifImageHeader::ExifExtendedTag::Sharpness               :
            case QExifImageHeader::ExifExtendedTag::DeviceSettingDescription:
            case QExifImageHeader::ExifExtendedTag::SubjectDistanceRange    :
            {
                qDebug() << tag;
                auto res = toQString(readerExifHeader.value(tag))[0].toStdString();
                return res.empty()? "":res;
            }
            break;
            default:
                return "";
            }
        }

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

    size_t EXIFResolver::resolver(const std::string& path) const
    {
        assert(this);

        auto hashValue = ImageProcess::generateFileIndexCode(path);

        /// load file
        auto loadImageFile = [](std::promise<void> & exitSignal, const std::string & path, size_t fileHashValue)
        {
            auto loadDataPtr = FileLoad::load(path);

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
        std::thread loading(loadImageFile,std::ref(g_ThreadFinishSignals.at(hashValue)),path,hashValue);
        loading.detach();

        return hashValue;
    }

    void EXIFResolver::resolver(std::shared_ptr<QByteArray> imagePixels, size_t imageExifResolverCode)
    {
        // auto loadImageFile = [](std::promise<void>& exitSignal, std::shared_ptr<QByteArray> imagePixels, size_t fileHashValue)
        auto loadImageFile = [](std::shared_ptr<QByteArray> imagePixels)
        {

                QExifImageHeader readerExifHeader;
                QBuffer ReadImageExifBuffer(imagePixels.get());
                {
                    ReadImageExifBuffer.open(QIODevice::ReadOnly);
                    ReadImageExifBuffer.seek(0);
                }
                readerExifHeader.loadFromJpeg(&ReadImageExifBuffer);

                auto imageTags = readerExifHeader.imageTags();
                auto gpsTags = readerExifHeader.gpsTags();
                auto extendedTags = readerExifHeader.extendedTags();

                auto outputExIFInfos = std::make_shared<EXIFInfo>();
                {
                    const auto& in = outputExIFInfos;

                    if(readerExifHeader.contains(QExifImageHeader::ImageTag::Make))
                    {
                        in->Make = ImageTag2String(readerExifHeader, QExifImageHeader::ImageTag::Make);
                    }
                    if (readerExifHeader.contains(QExifImageHeader::ImageTag::Model))
                    {
                        in->Model = ImageTag2String(readerExifHeader, QExifImageHeader::ImageTag::Model);
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ImageTag::DateTime))
                    {
                        in->DateTime = ImageTag2String(readerExifHeader, QExifImageHeader::ImageTag::DateTime);
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ImageTag::Copyright))
                    {
                        in->Copyright = ImageTag2String(readerExifHeader, QExifImageHeader::ImageTag::Copyright);
                    }

                    if(readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::ExposureTime))
                    {
                        const auto res = ExifExtendTagToString(readerExifHeader, QExifImageHeader::ExifExtendedTag::ExposureTime);
                        in->ExposureTime = std::stod(res);
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::FocalLength))
                    {
                        auto res = ExifExtendTagToString(readerExifHeader, QExifImageHeader::ExifExtendedTag::FocalLength);

                        in->FocalLength = static_cast<double>(readerExifHeader.value(QExifImageHeader::ExifExtendedTag::FocalLength).toLong());
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::FocalLengthIn35mmFilm))
                    {

                        in->FocalLengthIn35mm = static_cast<double>(readerExifHeader.value(QExifImageHeader::ExifExtendedTag::FocalLengthIn35mmFilm).toLong());
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::FNumber))
                    {

                        auto res = ExifExtendTagToString(readerExifHeader, QExifImageHeader::ExifExtendedTag::FNumber);

                        in->FNumber = static_cast<double>(readerExifHeader.value(QExifImageHeader::ExifExtendedTag::FNumber).toLong());
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::ISOSpeedRatings))
                    {
                        auto res = ExifExtendTagToString(readerExifHeader, QExifImageHeader::ExifExtendedTag::ISOSpeedRatings);
                        in->ISOSpeedRatings = static_cast<double>(readerExifHeader.value(QExifImageHeader::ExifExtendedTag::ISOSpeedRatings).toLong());
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::ShutterSpeedValue))
                    {
                        auto res = ExifExtendTagToString(readerExifHeader, QExifImageHeader::ExifExtendedTag::ShutterSpeedValue);

                        in->ShutterSpeedValue = static_cast<double>(readerExifHeader.value(QExifImageHeader::ExifExtendedTag::ShutterSpeedValue).toLong());
                    }

                    if (readerExifHeader.contains(QExifImageHeader::ExifExtendedTag::MakerNote))
                    {
                        auto res = ExifExtendTagToString(readerExifHeader, QExifImageHeader::ExifExtendedTag::MakerNote);
                        in->LensInfo.Make = res;
                    }

                    for(auto i : extendedTags)
                    {
                        auto res = ExifExtendTagToString(readerExifHeader, i);
                        qDebug() << QString::fromStdString(res);
                    }



                    putchar(10);

                    // in->ImageDescription = out.ImageDescription;
                    // in->Make = out.Make;
                    // in->Model = out.Model;
                    // in->Orientation = out.Orientation;
                    // in->BitsPerSample = out.BitsPerSample;
                    //
                    // in->DateTime = out.DateTime;
                    // in->DateTimeOriginal = out.DateTimeOriginal;
                    // in->DateTimeDigitized = out.DateTimeDigitized;
                    // in->SubSecTimeOriginal = out.SubSecTimeOriginal;
                    // in->Copyright = out.Copyright;
                    //
                    // in->FocalLength = out.FocalLength;  ////< 焦距
                    //
                    // in->ExposureTime = out.ExposureTime;
                    // in->FNumber = out.FNumber;
                    // in->ExposureProgram = out.ExposureProgram;
                    //
                    // in->ISOSpeedRatings = out.ISOSpeedRatings;
                    // in->ShutterSpeedValue = out.ShutterSpeedValue;
                    //
                    // in->ImageWidth = out.ImageWidth;
                    // in->ImageHeight = out.ImageHeight;
                    //
                    // in->LensInfo.Make = out.LensInfo.Make;
                    // in->LensInfo.Model = out.LensInfo.Model;

                    /// TODO need add others
                }

#if  0
                std::lock_guard<std::mutex> local(g_InfoMutex);
                g_LoadedInfos.insert({ fileHashValue,outputExIFInfos });
                g_LoadImageCheckCode.insert({ fileHashValue,0 });

                exitSignal.set_value();
                {
                    g_ThreadFinishSignals.erase(fileHashValue);
                }
#endif 
        };

#if 0
        std::promise<void> exitSignal;
        g_ThreadFinishSignals.insert({ imageExifResolverCode,std::move(exitSignal) });
        std::thread loading(loadImageFile, std::ref(g_ThreadFinishSignals.at(imageExifResolverCode)), imagePixels,imageExifResolverCode);
        loading.detach();
#endif

        loadImageFile(imagePixels);
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
