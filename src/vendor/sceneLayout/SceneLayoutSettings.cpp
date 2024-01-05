#include "SceneLayoutSettings.h"
#include <cmath>

namespace CM
{
    void SceneLayoutSettings::setMargin(const int left, const int right, const int top, const int bottom)
    {
        m_Margin = Margin{left, right, top, bottom};
    }

    SceneLayoutSettings::LPos SceneLayoutSettings::imagePos() const
    {
        return { m_Margin.m_Left,m_Margin.m_Top };
    }

    void SceneLayoutSettings::setLogoWithImageSpace(const int space)
    {
        m_LogoSpaceWithImage = space;
    }

    int SceneLayoutSettings::logoWithImageSpace() const
    {
        return m_LogoSpaceWithImage;
    }

    const Margin& SceneLayoutSettings::getMargin() const
    {
        return m_Margin;
    }

    Margin& SceneLayoutSettings::getMargin()
    {
        return m_Margin;
    }

    void SceneLayoutSettings::setRightMaxWidth(int w)
    {
        m_RightMaxWidth = w;
    }

    int SceneLayoutSettings::splitRectWidth() const
    {
        return m_SplitRectWidth;
    }

    const SceneLayoutSettings::LPoint& SceneLayoutSettings::logoPosition() const
    {
        return m_LogoPosition;
    }

    void SceneLayoutSettings::resetLayout()
    {
        m_Margin = {30, 30, 30, 10};
    }

    const SceneLayoutSettings::LSize& SceneLayoutSettings::logoSize() const
    {
        return m_LogoSize;
    }

    void SceneLayoutSettings::setLogoSize(int w, int h)
    {
        setLogoSize({w, h});
    }

    void SceneLayoutSettings::setLogoSize(const LSize& size)
    {
        m_LogoSize = size;
    }

    void SceneLayoutSettings::updateLeftTextOffset()
    {
        const auto width = m_Margin.m_Left + m_ShowImageSize.w + m_Margin.m_Right;
        m_LeftTextOffset = static_cast<int>(static_cast<float>(width) * 0.1);
    }

    void SceneLayoutSettings::setImageSize(const LSize& size)
    {
        m_ShowImageSize = size;
    }

    SceneLayoutSettings::SceneLayoutSettings() = default;

    SceneLayoutSettings::~SceneLayoutSettings() = default;

    void SceneLayoutSettings::update()
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

    int SceneLayoutSettings::leftTextOffset() const
    {
        return m_LeftTextOffset;
    }

    const SceneLayoutSettings::LSize& SceneLayoutSettings::imageSize() const
    {
        return m_ShowImageSize;
    }

    int SceneLayoutSettings::logoWithSplitLineSpace() const
    {
        return m_LogoSplitRectSpace;
    }

    void SceneLayoutSettings::updateRightTextOffset()
    {
        m_RightTextOffset = static_cast<int>(m_LogoSize.w * 1.45);
        m_RightTextOffset = (m_Margin.m_Left + m_Margin.m_Right + m_ShowImageSize.w) * 0.1f;
        if (m_RightTextOffset < m_Margin.m_Right)
        {
            m_RightTextOffset = m_Margin.m_Right;
        }
    }

    void SceneLayoutSettings::updateLogoSplitSpace()
    {
        m_LogoSplitRectSpace = static_cast<int>(m_LogoSize.w * 0.3);
    }

    int SceneLayoutSettings::rightTextOffset() const
    {
        return m_RightTextOffset;
    }

    void SceneLayoutSettings::updateSplitRectWidth()
    {
        m_SplitRectWidth = static_cast<int>(std::round(static_cast<float>(m_LogoSize.w) * 0.05f));

        if(m_SplitRectWidth > 7)
        {
            m_SplitRectWidth = 7;
        }

    }

    int SceneLayoutSettings::rightTextMaxWidth() const
    {
        return m_RightMaxWidth;
    }

    void SceneLayoutSettings::updateLogoPosition()
    {
        const auto sceneW = m_Margin.m_Left + m_Margin.m_Right + m_ShowImageSize.w;
        m_LogoPosition.x = (sceneW - m_RightTextOffset - m_RightMaxWidth - m_SplitRectWidth - 2 * m_LogoSplitRectSpace -
            m_LogoSize.w);
        m_LogoPosition.y = (m_Margin.m_Top + m_ShowImageSize.h + m_LogoSpaceWithImage);
    }
} // CM
