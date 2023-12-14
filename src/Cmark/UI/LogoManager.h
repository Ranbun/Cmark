#ifndef CAMERAMARK_LOGOMANAGER_H
#define CAMERAMARK_LOGOMANAGER_H

class QPixmap;

namespace CM
{
    enum CameraIndex : uint8_t
    {
        None,   ///< empty
        Nikon,
        Sony,
        Canon,
        Apple,
        Panasonic,
        Leica,
        Hasselblad,
        Fujifilm
         /// ...... TODO: need add others
    };

    class LogoManager
    {
    public:
        LogoManager() = default;

        static void Init();

        /**
         * @brief 通过cameraMake参数获取使用的相机
         * @param cameraMake exif Info
         * @return CameraIndex指示相机的制造商
         */
        static CameraIndex resolverCameraIndex(const std::string& cameraMake);

        /**
         * @brief load logo
         * @param cameraIndex 加载的logo的相机
         */
        static void loadCameraLogo(const CameraIndex & cameraIndex);

        /**
         * @brief 获取camera maker logo
         * @param cameraIndex
         * @return logo with QPixmap
         */
        static std::shared_ptr<QPixmap> getCameraMakerLogo(const CameraIndex & cameraIndex);

    private:
        static std::unordered_map<CameraIndex,std::shared_ptr<QPixmap>> m_logos;
        static std::unordered_map<std::string,CameraIndex> cameraMakerMap;
        static std::once_flag m_initFlag;
    };

} // CM

#endif //CAMERAMARK_LOGOMANAGER_H
