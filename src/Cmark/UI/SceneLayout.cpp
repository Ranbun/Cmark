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

    int SceneLayout::logoSpace() const
    {
        return m_logoSpace;
    }

    const Margin &SceneLayout::getMargin() const
    {
        return m_margin;
    }

    const CSize &SceneLayout::LogoSize() const
    {
        return m_LogoSize;
    }

    void SceneLayout::setLogoSize(int w, int h)
    {
        setLogoSize({w,h});
    }

    void SceneLayout::setLogoSize(const CSize &size)
    {
        m_LogoSize = size;
    }

} // CM
