#ifndef CAMERAMARK_FILELOAD_H
#define CAMERAMARK_FILELOAD_H

#include <iostream>
#include <vector>
#include <tuple>
#include <filesystem>

namespace CM
{
    /**
     * @brief 加载图片文件文件的类
     */
    class FileLoad
    {
    public:
        FileLoad() = default;
        [[maybe_unused]] static std::vector<unsigned char> Load(const std::filesystem::path &path); ///< just read signal file

    };
}

#endif //CAMERAMARK_FILELOAD_H
