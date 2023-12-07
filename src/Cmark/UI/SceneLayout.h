#ifndef CAMERAMARK_SCENELAYOUT_H
#define CAMERAMARK_SCENELAYOUT_H

#include <Base/CSize.h>
#include <Base/CPoint.h>

namespace CM
{
    /**
     * @brief Margin layout
     */
    struct Margin  ///< use pixel
    {
        int left {30};
        int right {30};
        int top {30};
        int bottom {5};
    };

    class SceneLayout
    {
    public:
        SceneLayout() = default;
        ~SceneLayout() = default;

        [[nodiscard]] const Margin & getMargin() const;
        void setMargin(int left,int right,int top,int bottom);

        /// logo
        void setLogoSpace(int space);
        [[nodiscard]] int logoSpace() const;

        [[nodiscard]] const CSize & LogoSize()const;
        void setLogoSize(int w,int h);
        void setLogoSize(const CSize & size);

        void updateLeftTextOffset();   ///< 更新内部计算结果

    private:
        CSize m_showImageSize;          ///< 显示的图片的大小
        Margin m_margin;                ///< scene margin{left right top bottom}
        int m_logoSpace {10};           ///< show image space with log
        CSize m_LogoSize{64,64};  ///< logo size

        int m_leftTextOffset;   ///< 左边部分文字的偏移长度 (left + image.width() + right) * 0.1
        // CPoint m_LogoPosition;  ///< ((left + image.width() + right) - m_leftTextOffset - leftText.Width()) * 0.55 + m_leftTextOffset + leftText.Width() 的位置的地方
        // line width 6 pixel


    };

} // CM

#endif //CAMERAMARK_SCENELAYOUT_H
