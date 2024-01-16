#ifndef CMARK_RESOURCESTOOLS_H_
#define CMARK_RESOURCESTOOLS_H_

#include "PictureManager.h"
#include "sources/LogoManager.h"
#include "Loader/EXIFResolver.h"

namespace CM::Tools
{
    using ImageResolver = CM::EXIFResolver;

    class ResourcesTools
    {
    public:
        ResourcesTools() = default;

        ResourcesTools(const ResourcesTools&) = delete;
        ResourcesTools(const ResourcesTools&&) = delete;
        ResourcesTools& operator=(const ResourcesTools&) = delete;
        ResourcesTools& operator=(const ResourcesTools&&) = delete;

        ~ResourcesTools() = default;

        static std::shared_ptr<QPixmap> getLoadedImage(const size_t index)
        {
            return PictureManager::getImage(index);
        }

        static size_t loadImage(const std::string& path)
        {
            return PictureManager::loadImage(path);
        }

        static void destory()
        {
            EXIFResolver::destory();
            LogoManager::destory();
            PictureManager::destory();
        }

        /// TODO: load image & resolver exif infos

    };
}


#endif // !CMARK_RESOURCESTOOLS_H_
