#ifndef CAMERAMARK_TYPE_H_
#define CAMERAMARK_TYPE_H_

#include <string>

namespace CM
{
    enum class ExifKey: uint8_t
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

        /// GPS
        GPS = None
    };

    struct ExifPack
    {
        ExifKey key;
        std::string text;
    };

    using ExifMap = std::vector<ExifPack>;

    // using ExifMap = std::unordered_map<ExifKey,std::string>;

}


#endif
