#include "CMark.h"

#include "PictureManager.h"

#include "File/ImageProcess/ImageProcess.h"
#include "Base/ImagePack.h"

#include <QBuffer>

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

    void PictureManager::loadImage(const ImagePack &pack, const bool synchronization)
    {
        if (getImage(pack.m_FileIndexCode))
            return;

        auto readFileToImage = [synchronization](const ImagePack& imagePack)
        {
            auto imageIndexCode = imagePack.m_FileIndexCode;
            QImage readImage;
            readImage.fill(Qt::transparent);

            {
                std::lock_guard localMutex(*imagePack.m_LocalMutex);
                const QFileInfo fileInfo(QString::fromStdString(imagePack.m_FileName));
                const auto format = fileInfo.suffix().toUpper();
                readImage = *ImageProcess::toQImage(imagePack.m_ImageData, format);
            }
            /// convert to QPixmap
            auto preViewImage = std::make_shared<QPixmap>(QPixmap::fromImage(readImage));
            if(imagePack.Size.w != -1 && imagePack.Size.h != -1)
            {
                const auto & [w,h] = imagePack.Size;

                const auto newWidth = w;
                const auto imageSize = QSizeF(preViewImage->size());
                const auto imageRatio = imageSize.width() / imageSize.height();
                const auto newHeight = static_cast<int>(std::floor(static_cast<float>(newWidth) / imageRatio));

                *preViewImage =   preViewImage->scaled(newWidth,newHeight,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
            }
            insert({imageIndexCode, preViewImage});
        };

        if(!synchronization)
        {
            std::thread readImage(readFileToImage, std::ref(pack));
            readImage.join();
        }
        else
        {
            readFileToImage(pack);
        }
    }

    void PictureManager::destroy()
    {
        g_LoadFinishSignals.clear();
        m_Maps.clear();
    }

    void PictureManager::insert(const std::pair<size_t, std::shared_ptr<QPixmap>> &d)
    {
        m_Maps.insert(d);
    }
}
