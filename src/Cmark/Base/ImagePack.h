#ifndef CAMERAMARK_IMAGEPACK_H
#define CAMERAMARK_IMAGEPACK_H

#include <memory>
#include <mutex>

#include <QByteArray>

namespace CM
{
    struct ImagePack
    {
        std::shared_ptr<QByteArray> imageData;
        const std::string & fileName;
        std::shared_ptr<std::mutex> localMutex;
        size_t fileIndexCode;
    };

}

#endif 
