#include "SceneLayoutSettings.h"
#include <cmath>

namespace CM
{
    void SceneLayoutSettings::setMargin(const int left, const int right, const int top, const int bottom)
    {
        m_margin = Margin{left, right, top, bottom};
    }

    void SceneLayoutSettings::setLogoWithImageSpace(const int space)
    {
        m_logoSpaceWithImage = space;
    }

    int SceneLayoutSettings::logoWithImageSpace() const
    {
        return m_logoSpaceWithImage;
    }

    const Margin& SceneLayoutSettings::getMargin() const
    {
        return m_margin;
    }

    Margin& SceneLayoutSettings::getMargin()
    {
        return m_margin;
    }

    void SceneLayoutSettings::setRightMaxWidth(int w)
    {
        m_rightMaxWidth = w;
    }

    int SceneLayoutSettings::splitRectWidth() const
    {
        return m_splitRectWidth;
    }

    const SceneLayoutSettings::LPoint& SceneLayoutSettings::logoPosition() const
    {
        return m_logoPosition;
    }

    void SceneLayoutSettings::resetLayout()
    {
        m_margin = {30, 30, 30, 10};
    }

    const SceneLayoutSettings::LSize& SceneLayoutSettings::LogoSize() const
    {
        return m_logoSize;
    }

    void SceneLayoutSettings::setLogoSize(int w, int h)
    {
        setLogoSize({w, h});
    }

    void SceneLayoutSettings::setLogoSize(const LSize& size)
    {
        m_logoSize = size;
    }

    void SceneLayoutSettings::updateLeftTextOffset()
    {
        const auto width = m_margin.left + m_showImageSize.w + m_margin.right;
        m_leftTextOffset = static_cast<int>(static_cast<float>(width) * 0.1);
    }

    void SceneLayoutSettings::setImageSize(const LSize& size)
    {
        m_showImageSize = size;
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
        return m_leftTextOffset;
    }

    const SceneLayoutSettings::LSize& SceneLayoutSettings::ImageSize() const
    {
        return m_showImageSize;
    }

    int SceneLayoutSettings::logoWithSplitLineSpace() const
    {
        return m_logoSplitRectSpace;
    }

    void SceneLayoutSettings::updateRightTextOffset()
    {
        m_rightTextOffset = static_cast<int>(m_logoSize.w * 1.45);
        m_rightTextOffset = (m_margin.left + m_margin.right + m_showImageSize.w) * 0.1f;
        if (m_rightTextOffset < m_margin.right)
        {
            m_rightTextOffset = m_margin.right;
        }
    }

    void SceneLayoutSettings::updateLogoSplitSpace()
    {
        m_logoSplitRectSpace = static_cast<int>(m_logoSize.w * 0.3);
    }

    int SceneLayoutSettings::rightTextOffset() const
    {
        return m_rightTextOffset;
    }

    void SceneLayoutSettings::updateSplitRectWidth()
    {
        m_splitRectWidth = static_cast<int>(std::round(static_cast<float>(m_logoSize.w) * 0.05f));
    }

    int SceneLayoutSettings::rightTextMaxWidth() const
    {
        return m_rightMaxWidth;
    }

    void SceneLayoutSettings::updateLogoPosition()
    {
        const auto sceneW = m_margin.left + m_margin.right + m_showImageSize.w;
        m_logoPosition.x = (sceneW - m_rightTextOffset - m_rightMaxWidth - m_splitRectWidth - 2 * m_logoSplitRectSpace -
            m_logoSize.w);
        m_logoPosition.y = (m_margin.top + m_showImageSize.h + m_logoSpaceWithImage);
    }
} // CM
