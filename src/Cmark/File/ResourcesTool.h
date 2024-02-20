#ifndef CMARK_RESOURCESTOOLS_H_
#define CMARK_RESOURCESTOOLS_H_

#include "PictureManager.h"
#include <File/Resolver/EXIFResolver.h>

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

        static void destroy()
        {
            EXIFResolver::destroyCached();
            PictureManager::destroyCached();
        }
    };
}


#endif // !CMARK_RESOURCESTOOLS_H_
