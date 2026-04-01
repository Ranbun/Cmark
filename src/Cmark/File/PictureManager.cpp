#include "PictureManager.h"

#include <Base/ImagePack.h>
#include <CMark.h>
#include <File/ImageProcess/ImageProcess.h>
#include <QtConcurrent/QtConcurrentRun>
#include "Resolver/EXIFResolver.h"
#include "SceneLayoutSettings.h"

namespace CM
{
PictureManager& PictureManager::Instance()
{
    static PictureManager instance;
    return instance;
}

std::shared_ptr<QImage> PictureManager::getImage(const size_t key)
{
    std::shared_ptr<QImage> cachedImage;
    if (m_Cache.getItem(key, cachedImage))
    {
        return cachedImage;
    }

    std::unique_lock lock(m_PendingMutex);
    if (m_PendingTasks.count(key))
    {
        auto feature = m_PendingTasks.at(key);
        feature.waitForFinished();  ///< 阻塞等待

        m_Cache.getItem(key, cachedImage);
        return cachedImage;
    }

    if (key == 0)
    {
        /// maybe we need a welcome image in this!
        return {};
    }

#if _DEBUG
    throw std::runtime_error("Can not found Image!");
#else
    cachedImage = std::make_shared<QImage>();
    cachedImage->scaled(1, 1);
    cachedImage->fill(Qt::transparent);
    return cachedImage;
#endif
}

QFuture<size_t> PictureManager::loadImage(const std::string& path)
{
    const auto indexCode = ImageProcess::generateFileIndexCode(path);

    /// check image exist
    std::shared_ptr<QImage> cachedImage;
    if (m_Cache.getItem(indexCode, cachedImage))
    {
        QFutureInterface<size_t> future;
        future.reportStarted();
        future.reportResult(indexCode);
        future.reportFinished();
        return std::move(future.future());
    }

    /// check task exist
    std::unique_lock lock(m_PendingMutex);
    if (m_PendingTasks.count(indexCode))
    {
        return m_PendingTasks.at(indexCode);
    }
    lock.unlock();

    /// register task
    auto future = QtConcurrent::run(
        &m_ThreadPool,
        [indexCode, path, this]() -> size_t
        {
            auto loadedImage = std::make_shared<QImage>();
            try
            {
                auto [w, h] = SceneLayoutSettings::fixPreViewImageSize();
                const auto data = ImageProcess::loadFile(QString::fromStdString(path));
                const ImagePack loadImagePack{indexCode, data, path, std::make_shared<std::mutex>(), {w, h}};

                const QFileInfo fileInfo(QString::fromStdString(path));
                const auto format = fileInfo.suffix().toUpper();

                loadedImage = ImageProcess::toQImage(data, format);

                if (w != -1 && h != -1)
                {
                    const auto imageSize = QSizeF(loadedImage->size());
                    const auto imageRatio = imageSize.width() / imageSize.height();
                    const auto newHeight = static_cast<int>(std::floor(static_cast<float>(w) / imageRatio));

                    *loadedImage =
                        loadedImage->scaled(w, newHeight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
                }
                EXIFResolver::resolver(loadImagePack);

                m_Cache.insert(indexCode, loadedImage);

#if _DEBUG
                if (loadedImage->isNull())
                {
                    throw std::runtime_error("Failed to load image from: " + path);
                }
#endif
            }
            catch (...)
            {
                loadedImage->fill(Qt::transparent);
            }

            std::scoped_lock PendingLock(m_PendingMutex);
            m_PendingTasks.erase(indexCode);

            return indexCode;
        });

    {
        std::scoped_lock PendingLock(m_PendingMutex);
        if (m_PendingTasks.count(indexCode))
        {
            return m_PendingTasks.at(indexCode);
        }
        m_PendingTasks[indexCode] = future;
    }

    return std::move(future);
}

PictureManager::PictureManager()
{
    m_ThreadPool.setMaxThreadCount(4);
}

void PictureManager::destroyCached()
{
    m_ThreadPool.waitForDone();

    m_Cache.clear();
    m_PendingTasks.clear();
}

}  // namespace CM
