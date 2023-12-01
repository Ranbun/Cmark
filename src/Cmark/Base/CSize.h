#ifndef CAMERAMARK_CSIZE_H
#define CAMERAMARK_CSIZE_H

namespace CM
{
    union Size
    {
        struct
        {
            int x;
            int y;
        };
        struct
        {
            int w;
            int h;
        };
    };

    using CSize = Size;

} // CM

#endif //CAMERAMARK_CSIZE_H
