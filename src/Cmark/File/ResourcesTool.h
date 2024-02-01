#ifndef CMARK_RESOURCESTOOLS_H_
#define CMARK_RESOURCESTOOLS_H_

#include "PictureManager.h"
#include "File/LogoManager.h"
#include "File/Resolver/EXIFResolver.h"
#include "File/ImageProcess/ImageProcess.h"

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
            auto data = CM::ImageProcess::loadFile(path.c_str());
            QFileInfo fileInfo(path.c_str());

            auto suffix = fileInfo.suffix().toUpper();
            auto image = CM::ImageProcess::toQImage(data,suffix);

            auto fileIndexCode = CM::ImageProcess::generateFileIndexCode(path);
            auto pixmap = std::make_shared<QPixmap>(QPixmap::fromImage(*image.get()));
            image.reset();
            PictureManager::insert({fileIndexCode,pixmap});

            return fileIndexCode;
        }

        static void destroy()
        {
            EXIFResolver::destroyCache();
            LogoManager::destory();
            PictureManager::destroy();
        }
    };
}


#endif // !CMARK_RESOURCESTOOLS_H_
