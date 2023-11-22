#ifndef CAMERAMARK_EXIFRESOLVER_H
#define CAMERAMARK_EXIFRESOLVER_H

#include "exif.h"
#include <iostream>
#include <vector>

namespace CM
{
    class EXIFResolver
    {
    public:
        EXIFResolver() =default;
        [[maybe_unused]] int resolver(const std::vector<unsigned  char > & pictureData);
        [[nodiscard]] const easyexif::EXIFInfo & getInfos() const {return m_EXIFResolver;}

        static void check(int resolverCode)
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

    private:
        easyexif::EXIFInfo m_EXIFResolver;
    };


} // CM

#endif //CAMERAMARK_EXIFRESOLVER_H
