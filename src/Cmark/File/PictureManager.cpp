#include "PictureManager.h"

#include <Base/ImagePack.h>
#include <CMark.h>
#include <File/ImageProcess/ImageProcess.h>
#include <QtConcurrent/QtConcurrentRun>
#include "Resolver/EXIFResolver.h"
#include "SceneLayoutSettings.h"

namespace CM
{

class Task : public QRunnable
{
    std::function<void()> m_func;

public:
    explicit Task(const std::function<void()>& func)
        : m_func(std::move(func))
    {
        setAutoDelete(true);
    }
    void run() override
    {
        if (m_func)
        {
            m_func();
        }
    }
};

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
        const auto feature = m_PendingTasks.at(key);
        feature.wait();

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

std::shared_future<std::shared_ptr<QImage>> PictureManager::loadImage(const std::string& path)
{
    const auto indexCode = ImageProcess::generateFileIndexCode(path);

    /// check image exist
    std::shared_ptr<QImage> cachedImage;
    if (m_Cache.getItem(indexCode, cachedImage))
    {
        std::promise<std::shared_ptr<QImage>> loadSignalPromise;
        loadSignalPromise.set_value(cachedImage);
        return loadSignalPromise.get_future().share();
    }

    /// check task exist
    std::unique_lock lock(m_PendingMutex);
    if (m_PendingTasks.count(indexCode))
    {
        return m_PendingTasks.at(indexCode);
    }
    /// create task to thread pool
    const auto promise = std::make_shared<std::promise<std::shared_ptr<QImage>>>();
    std::shared_future<std::shared_ptr<QImage>> feature = promise->get_future().share();

    /// register task
    m_PendingTasks[indexCode] = feature;

    lock.unlock();

    m_ThreadPool.start(new Task(
        [indexCode, path, promise, this]()
        {
            try
            {
                auto loadedImage = std::make_shared<QImage>();

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
                promise->set_value(loadedImage);
#if _DEBUG
                if (loadedImage->isNull())
                {
                    throw std::runtime_error("Failed to load image from: " + path);
                }
#endif
            }
            catch (...)
            {
                promise->set_exception(std::current_exception());
            }

            std::scoped_lock PendingLock(m_PendingMutex);
            m_PendingTasks.erase(indexCode);
        }));

    return feature;
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
