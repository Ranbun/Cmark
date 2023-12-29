#ifndef CAMERAMARK_FILELOAD_H_
#define CAMERAMARK_FILELOAD_H_

namespace CM
{
    /**
     * @brief 加载图片文件文件的类
     */
    class FileLoad
    {
    public:
        FileLoad() = default;
        [[maybe_unused]] static std::shared_ptr<std::vector<unsigned char>> Load(const std::filesystem::path& path); ///< just read signal file
    };
}

#endif //CAMERAMARK_FILELOAD_H_
