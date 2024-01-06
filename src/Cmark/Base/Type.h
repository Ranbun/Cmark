#ifndef CAMERAMARK_TYPE_H_
#define CAMERAMARK_TYPE_H_

#include <string>
#include <unordered_map>

namespace CM
{
    enum class ExifKey: uint16_t
    {
        None = 0,
        CameraMake,    ///<相机制造商
        CameraModel,
        Software,
        BitsPerSample,
        ImageWidth,
        ImageHeight,
        /// data or time
        ImageDate,
        ExposureTime,  /// 曝光时间

        FocalLength, ///< 焦距
        FStop,     ///< 光圈
        ISOSpeed,  ///<感光度
        ShutterSpeed,

        LensModel,  ///< 棱镜组 -- 镜头信息

        /// GPS
        GPS = None
    };

    /**
     * @brief Exif信息条目与实际存储的信息的映射
     */
    using ExifInfoMap = std::unordered_map<ExifKey,std::string>;

}


#endif
