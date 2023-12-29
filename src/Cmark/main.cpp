#include <CMark.h>

#include <QApplication>

#include "Loader/EXIFResolver.h"
#include "UI/MainWindow.h"
#include "sources/LogoManager.h"

#include <SceneLayoutEditor.h>

namespace Tools
{
#if _DEBUG
    void showInfos(std::weak_ptr<CM::EXIFInfo> infos)
    {

        const auto & result = *infos.lock();

        // Dump EXIF information
        qDebug()<< ("Camera make          : %s\n", result.Make.c_str());
        qDebug()<<("Camera model         : %s\n", result.Model.c_str());
        qDebug()<<("Software             : %s\n", result.Software.c_str());
        qDebug()<<("Bits per sample      : %d\n", result.BitsPerSample);
        qDebug()<<("Image width          : %d\n", result.ImageWidth);
        qDebug()<<("Image height         : %d\n", result.ImageHeight);
        qDebug()<<("Image description    : %s\n", result.ImageDescription.c_str());
        qDebug()<<("Image orientation    : %d\n", result.Orientation);
        qDebug()<<("Image copyright      : %s\n", result.Copyright.c_str());
        qDebug()<<("Image date/time      : %s\n", result.DateTime.c_str());
        qDebug()<<("Original date/time   : %s\n", result.DateTimeOriginal.c_str());
        qDebug()<<("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
        qDebug()<<("Subsecond time       : %s\n", result.SubSecTimeOriginal.c_str());
        qDebug()<<("Exposure time        : 1/%d s\n",
               (unsigned)(1.0 / result.ExposureTime));
        qDebug()<<("F-stop               : f/%.1f\n", result.FNumber);
        qDebug()<<("Exposure program     : %d\n", result.ExposureProgram);
        qDebug()<<("ISO speed            : %d\n", result.ISOSpeedRatings);
        qDebug()<<("Subject distance     : %f m\n", result.SubjectDistance);
        qDebug()<<("Exposure bias        : %f EV\n", result.ExposureBiasValue);
        qDebug()<<("Flash used?          : %d\n", result.Flash);
        qDebug()<<("Flash returned light : %d\n", result.FlashReturnedLight);
        qDebug()<<("Flash mode           : %d\n", result.FlashMode);
        qDebug()<<("Metering mode        : %d\n", result.MeteringMode);
        qDebug()<<("Lens focal length    : %f mm\n", result.FocalLength);
        qDebug()<<("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
        qDebug()<<("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
               result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
               result.GeoLocation.LatComponents.minutes,
               result.GeoLocation.LatComponents.seconds,
               result.GeoLocation.LatComponents.direction);
        qDebug()<<("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
               result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
               result.GeoLocation.LonComponents.minutes,
               result.GeoLocation.LonComponents.seconds,
               result.GeoLocation.LonComponents.direction);
        qDebug()<<("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
        qDebug()<<("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
        qDebug()<<("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
        qDebug()<<("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
        qDebug()<<("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
        qDebug()<<("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
        qDebug()<<("Lens make            : %s\n", result.LensInfo.Make.c_str());
        qDebug()<<("Lens model           : %s\n", result.LensInfo.Model.c_str());
        qDebug()<<("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
        qDebug()<<("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);
    }
    int testPath(const std::string & path)
    {
        FILE *fp = fopen(path.c_str(), "rb");
        if (!fp) {
            printf("Can't open file.\n");
            return -1;
        }
        fseek(fp, 0, SEEK_END);
        unsigned long fsize = ftell(fp);
        rewind(fp);
        unsigned char *buf = new unsigned char[fsize];
        if (fread(buf, 1, fsize, fp) != fsize) {
            printf("Can't read file.\n");
            delete[] buf;
            return -2;
        }
        fclose(fp);

        // Parse EXIF
        easyexif::EXIFInfo result;
        int code = result.parseFrom(buf, fsize);
        delete[] buf;
        if (code) {
            printf("Error parsing EXIF: code %d\n", code);
            return -3;
        }

        // Dump EXIF information
        printf("Camera make          : %s\n", result.Make.c_str());
        printf("Camera model         : %s\n", result.Model.c_str());
        printf("Software             : %s\n", result.Software.c_str());
        printf("Bits per sample      : %d\n", result.BitsPerSample);
        printf("Image width          : %d\n", result.ImageWidth);
        printf("Image height         : %d\n", result.ImageHeight);
        printf("Image description    : %s\n", result.ImageDescription.c_str());
        printf("Image orientation    : %d\n", result.Orientation);
        printf("Image copyright      : %s\n", result.Copyright.c_str());
        printf("Image date/time      : %s\n", result.DateTime.c_str());
        printf("Original date/time   : %s\n", result.DateTimeOriginal.c_str());
        printf("Digitize date/time   : %s\n", result.DateTimeDigitized.c_str());
        printf("Subsecond time       : %s\n", result.SubSecTimeOriginal.c_str());
        printf("Exposure time        : 1/%d s\n",
               (unsigned)(1.0 / result.ExposureTime));
        printf("F-stop               : f/%.1f\n", result.FNumber);
        printf("Exposure program     : %d\n", result.ExposureProgram);
        printf("ISO speed            : %d\n", result.ISOSpeedRatings);
        printf("Subject distance     : %f m\n", result.SubjectDistance);
        printf("Exposure bias        : %f EV\n", result.ExposureBiasValue);
        printf("Flash used?          : %d\n", result.Flash);
        printf("Flash returned light : %d\n", result.FlashReturnedLight);
        printf("Flash mode           : %d\n", result.FlashMode);
        printf("Metering mode        : %d\n", result.MeteringMode);
        printf("Lens focal length    : %f mm\n", result.FocalLength);
        printf("35mm focal length    : %u mm\n", result.FocalLengthIn35mm);
        printf("GPS Latitude         : %f deg (%f deg, %f min, %f sec %c)\n",
               result.GeoLocation.Latitude, result.GeoLocation.LatComponents.degrees,
               result.GeoLocation.LatComponents.minutes,
               result.GeoLocation.LatComponents.seconds,
               result.GeoLocation.LatComponents.direction);
        printf("GPS Longitude        : %f deg (%f deg, %f min, %f sec %c)\n",
               result.GeoLocation.Longitude, result.GeoLocation.LonComponents.degrees,
               result.GeoLocation.LonComponents.minutes,
               result.GeoLocation.LonComponents.seconds,
               result.GeoLocation.LonComponents.direction);
        printf("GPS Altitude         : %f m\n", result.GeoLocation.Altitude);
        printf("GPS Precision (DOP)  : %f\n", result.GeoLocation.DOP);
        printf("Lens min focal length: %f mm\n", result.LensInfo.FocalLengthMin);
        printf("Lens max focal length: %f mm\n", result.LensInfo.FocalLengthMax);
        printf("Lens f-stop min      : f/%.1f\n", result.LensInfo.FStopMin);
        printf("Lens f-stop max      : f/%.1f\n", result.LensInfo.FStopMax);
        printf("Lens make            : %s\n", result.LensInfo.Make.c_str());
        printf("Lens model           : %s\n", result.LensInfo.Model.c_str());
        printf("Focal plane XRes     : %f\n", result.LensInfo.FocalPlaneXResolution);
        printf("Focal plane YRes     : %f\n", result.LensInfo.FocalPlaneYResolution);

        return 0;
    }
#endif
};

#define SHOW_WIDGET 1

int main(int argc,char ** argv)
{
#if SHOW_WIDGET

    QApplication app(argc,argv);
    CM::MainWindow manApp;

    CM::LogoManager::Init();

    manApp.resize(720,560);
    manApp.setWindowTitle("Camera Mark");
    manApp.show();

#endif

#if  _DEBUG >> 1
    auto path = std::filesystem::path("./sources/pictures/Nikon/DSC_1218.jpg");
    manApp.PreViewImage(path);

    CM::EXIFResolver resolver;
    auto code = resolver.Hash<std::string>(path.string());
    const auto & [checkRes,infos] = CM::EXIFResolver::check(resolver.checkCode(code));
    if(checkRes)
    {
        const auto& result = resolver.getExifInfo(code);
        Tools::showInfos(result);
    }

    Tools::testPath(path.string());

#endif

#if SHOW_WIDGET
    return QApplication::exec();
#endif
}

