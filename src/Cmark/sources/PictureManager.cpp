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
        m_Maps.insert(key,value);
    }

    std::shared_ptr<QPixmap> PictureManager::getImage(const size_t key)
    {
        if(g_LoadFinishSignals.count(key))
        {
            auto& exitSignal = g_LoadFinishSignals.at(key);
            exitSignal.get_future().wait();   ///< 等待线程结束
            g_LoadFinishSignals.erase(key);
        }

        return m_Maps.getPixmap(key);
    }

    void PictureManager::remove(const size_t index)
    {
        if (g_LoadFinishSignals.count(index))
        {
            auto& exitSignal = g_LoadFinishSignals.at(index);
            exitSignal.get_future().wait();   ///< 等待线程结束
            g_LoadFinishSignals.erase(index);
        }

        std::lock_guard<std::mutex> local(g_Mutex);
        m_Maps.remove(index);

    }

    size_t PictureManager::loadImage(const std::string& path)
    {
        constexpr std::hash<std::string> hasher;
        const auto imageIndexCode = hasher(path);

        if (const auto loadedImage = getImage(imageIndexCode))
        {
            return imageIndexCode;
        }

        auto readFileToImage = [](const std::string& path, std::promise<void> & loadedSignal,size_t imageIndexCode)
        {
            QImage readImage;
            readImage.fill(Qt::transparent);

            const auto loadDataPtr = FileLoad::load(path);
            const auto imageData = FileLoad::toQByteArray(loadDataPtr);

            QBuffer ReadAsImageBuffer(imageData.get());
            {
                ReadAsImageBuffer.open(QIODevice::ReadOnly);
                ReadAsImageBuffer.seek(0);
            }

            const auto imageReader = std::make_shared<QImageReader>(&ReadAsImageBuffer,"JPEG");
            imageReader->setAutoTransform(true);
            readImage = imageReader->read();

            /// convert to QPixmap
            const std::shared_ptr<QPixmap> preViewImage = std::make_shared<QPixmap>(std::move(QPixmap::fromImage(readImage)));
            insert({ imageIndexCode,preViewImage });

            {
                std::lock_guard<std::mutex> local(g_Mutex);
                loadedSignal.set_value();
                g_LoadFinishSignals.erase(imageIndexCode);
            }

        };

        std::promise<void> exitSignal;
        g_LoadFinishSignals.insert({ imageIndexCode,std::move(exitSignal) });
        std::thread readImage(readFileToImage, path,std::ref(g_LoadFinishSignals.at(imageIndexCode)), imageIndexCode);
        readImage.detach();

        return imageIndexCode;
    }

    void PictureManager::loadImage(const ImagePack& pack)
    {
        if (getImage(pack.fileIndexCode))
            return;

        auto readFileToImage = [](const ImagePack& pack, std::promise<void>& loadedSignal)
        {
            auto imageIndexCode = pack.fileIndexCode;
            QImage readImage;
            readImage.fill(Qt::transparent);

            std::unique_lock local_mutex(*pack.localMutex);
            const QFileInfo fileInfo(QString::fromStdString(pack.fileName));
            QBuffer ReadAsImageBuffer(pack.imageData.get());
            local_mutex.unlock();

            ReadAsImageBuffer.open(QIODevice::ReadOnly);
            ReadAsImageBuffer.seek(0);

            const auto format = fileInfo.suffix().toUpper().toStdString();
            const auto imageReader = std::make_shared<QImageReader>(&ReadAsImageBuffer, format.c_str()); ///< maybe we need get file name
            imageReader->setAutoTransform(true);
            readImage = imageReader->read();

            /// convert to QPixmap
            const std::shared_ptr<QPixmap> preViewImage = std::make_shared<QPixmap>(std::move(QPixmap::fromImage(readImage)));
            insert({ imageIndexCode,preViewImage });

            {
                std::lock_guard<std::mutex> local(g_Mutex);
                loadedSignal.set_value();
                g_LoadFinishSignals.erase(imageIndexCode);
            }

        };

        std::promise<void> exitSignal;
        g_LoadFinishSignals.insert({ pack.fileIndexCode,std::move(exitSignal) });
        std::thread readImage(readFileToImage, std::ref(pack), std::ref(g_LoadFinishSignals.at(pack.fileIndexCode)));
        readImage.detach();

    }


    void PictureManager::destory()
    {
        m_Maps.clear();
    }
}
