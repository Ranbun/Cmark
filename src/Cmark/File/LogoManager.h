#ifndef CAMERAMARK_LOGOMANAGER_H
#define CAMERAMARK_LOGOMANAGER_H

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

class QImage;

namespace CM
{
    enum class CameraIndex : uint8_t
    {
        None = 0,
        Nikon,
        Sony,
        Canon,
        Apple,
        Panasonic,
        Leica,
        Hasselblad,
        Fujifilm
    };

    /**
     * @brief 集中定义相机厂商信息：厂商名、枚举、Logo 资源路径
     */
    struct CameraInfo
    {
        std::string makerName;
        CameraIndex index;
        std::string logoPath;
    };

    class LogoManager
    {
    public:
        static LogoManager& instance();

        LogoManager(const LogoManager&) = delete;
        LogoManager(LogoManager&&) = delete;
        LogoManager& operator=(const LogoManager&) = delete;
        LogoManager& operator=(LogoManager&&) = delete;

        /**
         * @brief 通过 cameraMake 参数获取使用的相机
         * @param cameraMake exif Info
         * @return CameraIndex 指示相机的制造商
         */
        CameraIndex resolveCameraIndex(const std::string& cameraMake);

        /**
         * @brief 获取 camera maker logo
         * @param cameraIndex 相机索引
         * @return logo with QImage (线程安全)
         */
        std::shared_ptr<QImage> getCameraMakerLogo(CameraIndex cameraIndex);

        /**
         * @brief 销毁 logo manager，清空所有加载的 logo
         */
        void destroy();

    private:
        LogoManager();
        ~LogoManager() = default;

        void loadCameraLogo(CameraIndex cameraIndex);

        std::mutex m_dataMutex;
        std::unordered_map<CameraIndex, std::shared_ptr<QImage>> m_logos;
        std::unordered_map<std::string, CameraIndex> m_cameraMakerMap;
        std::unordered_map<CameraIndex, std::string> m_logoPathMap;
    };
} // namespace CM

#endif // CAMERAMARK_LOGOMANAGER_H
