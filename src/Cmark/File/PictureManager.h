#ifndef CMARK_PICTUREMANAGER_H_
#define CMARK_PICTUREMANAGER_H_

#include <Base/FixMap.hpp>
#include <QPixmap>
#include <QThreadPool>
#include <future>
#include <mutex>
#include <unordered_map>

namespace CM
{
namespace Tools
{
class ResourcesTools;
}

class PictureManager
{
    friend class Tools::ResourcesTools;

public:
    static PictureManager& Instance();

    PictureManager(const PictureManager&) = delete;
    PictureManager(PictureManager&&) = delete;
    PictureManager& operator=(const PictureManager&) = delete;
    PictureManager& operator=(PictureManager&&) = delete;

    /**
     * @brief 获取加载完成的pixmap
     * @param key size_t 文件的索引
     * @return std::shared_ptr<QPixmap> 加载的结果
     */
    std::shared_ptr<QImage> getImage(size_t key);
    std::shared_future<std::shared_ptr<QImage>> loadImage(const std::string& path);

private:
    PictureManager();
    void destroyCached();

private:
    LRUCache<size_t, std::shared_ptr<QImage>> m_Cache;
    std::unordered_map<size_t, std::shared_future<std::shared_ptr<QImage>>> m_PendingTasks;
    QThreadPool m_ThreadPool;
    std::mutex m_PendingMutex;

};
}  // namespace CM

#endif
