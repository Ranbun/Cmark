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

    const CSize &SceneLayout::logoSize() const
    {
        return m_logoSize;
    }

    void SceneLayout::setLogoSize(int w, int h)
    {
        setLogoSize({w,h});
    }

    void SceneLayout::setLogoSize(const CSize &size)
    {
        m_logoSize = size;
    }

    void SceneLayout::updateLeftTextOffset()
    {
        auto width = m_margin.left + m_showImageSize.w + m_margin.right;
        m_leftTextOffset = (int)(static_cast<float>(width) * 0.1);
    }

    void SceneLayout::setImageSize(const CSize &size)
    {
        m_showImageSize = size;
    }

    void SceneLayout::update()
    {
        /// finished image size
        /// logo size
        /// margin left & right & top & bottom

        /// update some items
        updateLeftTextOffset();
        updateRightTextOffset();
        updateLogoSplitSpace();

        updateSplitRectWidth();

        /// logo position


        /// line position

    }

    int SceneLayout::leftTextOffset() const
    {
        return m_leftTextOffset;
    }

    const CSize &SceneLayout::previewImageSize() const
    {
        return m_showImageSize;
    }

    int SceneLayout::logoSplitLineSpace() const
    {
        return m_logoSplitLineSpace;
    }

    void SceneLayout::updateRightTextOffset()
    {
        m_rightTextOffset = static_cast<int>(m_logoSize.w * 1.45);
        if(m_rightTextOffset < m_margin.right)
        {
            m_rightTextOffset =  m_margin.right;
        }
    }

    void SceneLayout::updateLogoSplitSpace()
    {
        m_logoSplitLineSpace = static_cast<int>(m_logoSize.w * 0.3);
    }

    int SceneLayout::rightTextOffset() const
    {
        return m_rightTextOffset;
    }

    void SceneLayout::updateSplitRectWidth()
    {
        m_splitRectWidth = (int)(m_logoSize.w * 0.05 + 0.5);
    }

} // CM
