#ifndef CMARK_PICTUREMANAGER_H_
#define CMARK_PICTUREMANAGER_H_


#include <Base/FixMap.hpp>
#include <Base/ImagePack.h>


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
        PictureManager() = default;

        /**
         * @brief 插入pixmap
         * @param d const std::pair<size_t, std::shared_ptr<QPixmap>>& 插入的pixmap
         */
        static void insert(const std::pair<size_t, std::shared_ptr<QPixmap>>& d);

        /**
         * @brief 插入QPixmap
         * @param key size_t 图片索引
         * @param value const std::shared_ptr<QPixmap>& 插入的QPixmap
         */
        static void insert(size_t key, const std::shared_ptr<QPixmap>& value);

        /**
         * @brief 获取加载完成的pixmap
         * @param key size_t 文件的索引
         * @return std::shared_ptr<QPixmap> 加载的结果
         */
        static std::shared_ptr<QPixmap> getImage(size_t key);

        /**
         * @brief 移除一个被加载的图片
         * @param index size_t
         */
        static void remove(size_t index);

        /**
         * @brief load image from QByteArray
         * @param pack params pack
         * @param synchronization bool 是否同步
         */
        static void loadImage(const ImagePack &pack, bool synchronization = true);


        /**
         * @brief 获取所有记录的Image
         * @return FixMap<size_t, std::shared_ptr<QPixmap>>
         */
        static FixMap<size_t, std::shared_ptr<QPixmap>> & images(){return m_Maps;}

    private:
        static void destroyCached();

    private:
        static FixMap<size_t, std::shared_ptr<QPixmap>> m_Maps;
    };
}

#endif
