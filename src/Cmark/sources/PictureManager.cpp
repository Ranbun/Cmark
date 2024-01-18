#include <CMark.h>
#include "PictureManager.h"

#include <QBuffer>
#include <QImageReader>

#include "Loader/FileLoad.h"
#include <ImageProcess/ImageProcess.h>

#include "Base/ImagePack.h"

/// make it thread feature

namespace CM
{
    namespace
    {
        std::unordered_map<size_t, std::promise<void>> g_LoadFinishSignals;
        std::mutex g_Mutex;
    }

    FixMap<size_t, std::shared_ptr<QPixmap>> PictureManager::m_Maps;

    void PictureManager::insert(const size_t key, const std::shared_ptr<QPixmap>& value)
    {
        std::lock_guard<std::mutex> local(g_Mutex);
        m_Maps.insert(key, value);
    }

    std::shared_ptr<QPixmap> PictureManager::getImage(const size_t key)
    {
        if (g_LoadFinishSignals.count(key))
        {
            auto& exitSignal = g_LoadFinishSignals.at(key);
            exitSignal.get_future().wait(); ///< 等待线程结束
            g_LoadFinishSignals.erase(key);
        }

        return m_Maps.getPixmap(key);
    }

    void PictureManager::remove(const size_t index)
    {
        if (g_LoadFinishSignals.count(index))
        {
            auto& exitSignal = g_LoadFinishSignals.at(index);
            exitSignal.get_future().wait(); ///< 等待线程结束
            g_LoadFinishSignals.erase(index);
        }

        std::lock_guard<std::mutex> local(g_Mutex);
        m_Maps.remove(index);
    }

    void PictureManager::loadImage(const ImagePack& pack)
    {
        if (getImage(pack.m_FileIndexCode))
            return;

        auto readFileToImage = [](const ImagePack& imagePack, std::promise<void>& loadedSignal)
        {
            auto imageIndexCode = imagePack.m_FileIndexCode;
            QImage readImage;
            readImage.fill(Qt::transparent);

            {
                std::lock_guard local_mutex(*imagePack.m_LocalMutex);
                const QFileInfo fileInfo(QString::fromStdString(imagePack.m_FileName));
                const auto format = fileInfo.suffix().toUpper();
                readImage = *ImageProcess::toQImage(imagePack.m_ImageData, format);
            }
            /// convert to QPixmap
            auto preViewImage = std::make_shared<QPixmap>(QPixmap::fromImage(readImage));
            insert({imageIndexCode, preViewImage});
            {
                std::lock_guard<std::mutex> local(g_Mutex);
                loadedSignal.set_value();
                g_LoadFinishSignals.erase(imageIndexCode);
            }
        };

        std::promise<void> exitSignal;
        g_LoadFinishSignals.insert({pack.m_FileIndexCode, std::move(exitSignal)});
        std::thread readImage(readFileToImage, std::ref(pack), std::ref(g_LoadFinishSignals.at(pack.m_FileIndexCode)));

        readImage.join();
    }

    void PictureManager::destroy()
    {
        g_LoadFinishSignals.clear();
        m_Maps.clear();
    }
}
