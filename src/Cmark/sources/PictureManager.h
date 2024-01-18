#ifndef CMARK_PICTUREMANAGER_H_
#define CMARK_PICTUREMANAGER_H_


#include <Base/ImagePack.h>
#include <Base/FixMap.hpp>


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

        static void insert(const std::pair<size_t, std::shared_ptr<QPixmap>>& d)
        {
            m_Maps.insert(d);
        }

        static void insert(size_t key, const std::shared_ptr<QPixmap>& value);

        static std::shared_ptr<QPixmap> getImage(size_t key);

        static void remove(size_t index);

        /**
         * @brief load image from QByteArray
         * @param pack params pack
         */
        static void loadImage(const ImagePack& pack);

    private:
        static void destroy();

    private:
        static FixMap<size_t, std::shared_ptr<QPixmap>> m_Maps;
    };
}

#endif
