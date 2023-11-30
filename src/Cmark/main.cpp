#include <QApplication>
#include <iostream>
#include <filesystem>

#include "Load/FileLoad.h"
#include "Load/EXIFResolver.h"
#include "UI/MainWindow.h"

namespace Tools
{
    void showInfos(const easyexif::EXIFInfo & result)
    {
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
    }
};

int main(int argc,char ** argv)
{
    QApplication app(argc,argv);
    CM::MainWindow manApp;

    manApp.resize(720,560);

    manApp.setWindowTitle("Window App");
    manApp.show();

#if  _DEBUG

    auto path = std::filesystem::path("./sources/pictures/DSC_3.jpg");
    manApp.PreViewImage(path);

    const auto& pictureData = CM::FileLoad::Load(path.string());
    CM::EXIFResolver resolver;
    CM::EXIFResolver::check(resolver.resolver(pictureData));
    const auto& result = resolver.getInfos();

    Tools::showInfos(result);
#endif

    return QApplication::exec();
}

