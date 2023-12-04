#include "SceneLayout.h"

namespace CM
{
    void SceneLayout::setMargin(int left,int right,int top,int bottom)
    {
        m_margin = Margin{left,right,top,bottom};
    }

    void SceneLayout::setLogoSpace(int space)
    {
        m_logoSpace = space;
    }

    int SceneLayout::logoSpace()
    {
        return m_logoSpace;
    }

} // CM
