#include <CMark.h>
#include "PictureManager.h"

#include <QBuffer>
#include <QImageReader>

#include "Loader/FileLoad.h"
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

        auto readFileToImage = [](const std::filesystem::path& path, std::promise<void> & loadedSignal,size_t imageIndexCode)
        {
            QImage readImage;
            readImage.fill(Qt::transparent);

            const auto loadDataPtr = FileLoad::load(path);
            auto imageData = FileLoad::toQByteArray(loadDataPtr);

            QBuffer ReadAsImageBuffer(imageData.get());
            {
                ReadAsImageBuffer.open(QIODevice::ReadOnly);
                ReadAsImageBuffer.seek(0);
            }

            const auto imageReader = std::make_shared<QImageReader>(&ReadAsImageBuffer,"JPEG");
            imageReader->setAutoTransform(true);
            readImage = imageReader->read();

#if _DEBUG
            QExifImageHeader readerExifHeader;
            QBuffer ReadImageExifBuffer(imageData.get());
            {
                ReadImageExifBuffer.open(QIODevice::ReadOnly);
                ReadImageExifBuffer.seek(0);
            }
            readerExifHeader.loadFromJpeg(&ReadImageExifBuffer);

            auto imageTags = readerExifHeader.imageTags();
            auto gpsTags = readerExifHeader.gpsTags();
            auto extendedTags = readerExifHeader.extendedTags();

#endif
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

    void PictureManager::destory()
    {
        m_Maps.clear();
    }
}
