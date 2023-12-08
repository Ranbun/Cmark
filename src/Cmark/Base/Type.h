#ifndef CAMERAMARK_TYPE_H_
#define CAMERAMARK_TYPE_H_

#include <string>
#include <unordered_map>

namespace CM
{
    enum class ExifKey: uint16_t
    {
        None = 0,
        Camera_make,    ///<相机制造商
        Camera_model,
        Software,
        Bits_per_sample,
        Image_width,
        Image_height,
        /// data or time
        Image_date,
        Exposure_time,  /// 曝光时间

        F_stop,     ///< 光圈
        ISO_speed,  ///<感光度
        Shutter_speed,

        Lens_Model,  ///< 棱镜组 -- 镜头信息

        /// GPS
        GPS = None
    };

    struct ExifPack
    {
        ExifKey key;
        std::string text;
    };

    using ExifInfoMap = std::unordered_map<ExifKey,std::string>;

}


#endif
