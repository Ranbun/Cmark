#ifndef CAMERAMARK_LOGOMANAGER_H
#define CAMERAMARK_LOGOMANAGER_H

#include <iostream>
#include <unordered_map>

#include <QPixmap>

namespace CM
{
    enum CameraIndex : uint8_t
    {
        None,
        Nikon,
        Sony,
        Canon,
        Apple,
         /// ...... TODO: need add others
    };

    class LogoManager
    {
    public:
        LogoManager() = default;

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
    };

} // CM

#endif //CAMERAMARK_LOGOMANAGER_H
