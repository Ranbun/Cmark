#ifndef CAMERAMARK_CSIZE_H
#define CAMERAMARK_CSIZE_H

namespace CM
{
    union Size
    {
        struct
        {
            [[maybe_unused]] int x;
            [[maybe_unused]] int y;
        };
        struct
        {
            int w;
            int h;
        };
        [[nodiscard]] [[maybe_unused]] int width() const{return w;}
        [[nodiscard]] [[maybe_unused]] int height() const{return h;}
    };

    using CSize = Size;

} // CM

#endif //CAMERAMARK_CSIZE_H
