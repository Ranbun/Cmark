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
        using CData = std::shared_ptr<std::vector<unsigned char>>;

        FileLoad() = default;
        [[maybe_unused]] static CData load(const std::string& path); ///< just read signal file
        static std::shared_ptr<QByteArray> toQByteArray(const CData &inputData);
    };
}

#endif //CAMERAMARK_FILELOAD_H_
