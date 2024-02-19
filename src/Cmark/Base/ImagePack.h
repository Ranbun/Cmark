#ifndef CAMERAMARK_IMAGEPACK_H
#define CAMERAMARK_IMAGEPACK_H

#include <memory>
#include <mutex>

#include <QByteArray>

namespace CM
{
    struct ImagePack
    {
        struct ImageSize
        {
            int m_W;
            int m_H;
        };

        size_t m_FileIndexCode;
        std::shared_ptr<QByteArray> m_ImageData;
        std::string m_FileName;
        std::shared_ptr<std::mutex> m_LocalMutex;
        ImageSize m_Size{-1,-1};
    };

}

#endif
