#ifndef CAMERAMARK_CPOINT_H
#define CAMERAMARK_CPOINT_H

#include  <array>

namespace CM
{
    struct Point
    {
        Point() = default;;

        [[maybe_unused]] Point(std::initializer_list<int> list)
        {
            std::copy(list.begin(), list.end(),m_points.begin());
        }

        [[nodiscard]] [[maybe_unused]] int x() const{return m_points[0];}
        [[nodiscard]] [[maybe_unused]] int y() const{return m_points[1];}
        [[nodiscard]] [[maybe_unused]] int z() const{return m_points[2];}

        [[maybe_unused]] void setX(int x) { m_points[0] = x;}
        [[maybe_unused]] void setY(int y) { m_points[1] = y;}
        [[maybe_unused]] void setZ(int z) { m_points[2] = z;}

        [[maybe_unused]] void setPosition(std::initializer_list<int> list)
        {
            std::copy(list.begin(), list.end(),m_points.begin());
        }

    private:
        std::array<int,3> m_points{0,0,0};

    };

    using CPoint = Point;
}


#endif
