#include "CMark.h"
#include "SceneLayout.h"

namespace CM
{
    void SceneLayout::setMargin(int left,int right,int top,int bottom)
    {
        m_margin = Margin{left,right,top,bottom};
    }

    void SceneLayout::setLogoWithImageSpace(int space)
    {
        m_logoSpaceWithImage = space;
    }

    int SceneLayout::logoWithImageSpace() const
    {
        return m_logoSpaceWithImage;
    }

    const Margin &SceneLayout::getMargin() const
    {
        return m_margin;
    }

    const CSize &SceneLayout::LogoSize() const
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

        /// applyLayout some items
        updateLeftTextOffset();
        updateRightTextOffset();
        updateLogoSplitSpace();
        updateSplitRectWidth();

        /// logo position
        updateLogoPosition();
    }

    int SceneLayout::leftTextOffset() const
    {
        return m_leftTextOffset;
    }

    const CSize &SceneLayout::ImageSize() const
    {
        return m_showImageSize;
    }

    int SceneLayout::logoWithSplitLineSpace() const
    {
        return m_logoSplitRectSpace;
    }

    void SceneLayout::updateRightTextOffset()
    {
        m_rightTextOffset = static_cast<int>(m_logoSize.w * 1.45);
        m_rightTextOffset = (m_margin.left + m_margin.right + m_showImageSize.width()) * 0.1f;
        if(m_rightTextOffset < m_margin.right)
        {
            m_rightTextOffset =  m_margin.right;
        }
    }

    void SceneLayout::updateLogoSplitSpace()
    {
        m_logoSplitRectSpace = static_cast<int>(m_logoSize.w * 0.3);
    }

    int SceneLayout::rightTextOffset() const
    {
        return m_rightTextOffset;
    }

    void SceneLayout::updateSplitRectWidth()
    {
        m_splitRectWidth = (int)std::round(static_cast<float>(m_logoSize.w) * 0.05f);
    }

    int SceneLayout::rightTextMaxWidth() const
    {
        return m_rightMaxWidth;
    }

    void SceneLayout::updateLogoPosition()
    {
        auto sceneW = m_margin.left + m_margin.right + m_showImageSize.w;
        m_logoPosition.setX(sceneW - m_rightTextOffset - m_rightMaxWidth - m_splitRectWidth - 2 * m_logoSplitRectSpace - m_logoSize.w);
        m_logoPosition.setY(m_margin.top + m_showImageSize.h + m_logoSpaceWithImage);
    }

} // CM
