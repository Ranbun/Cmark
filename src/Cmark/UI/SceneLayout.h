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

        /**
         * @brief 计算一些布局使用的结果
         */
        void update();

        [[nodiscard]] const Margin & getMargin() const;
        void setMargin(int left,int right,int top,int bottom);

        /// logo
        void setLogoSpace(int space);
        [[nodiscard]] int logoSplitLineSpace() const;
        [[nodiscard]] int logoSpace() const;

        [[nodiscard]] const CSize & logoSize()const;
        void setLogoSize(int w,int h);
        void setLogoSize(const CSize & size);


        [[maybe_unused]] void setImageSize(const CSize & size);
        [[nodiscard]] const CSize & previewImageSize() const;

        [[nodiscard]] [[maybe_unused]] int leftTextOffset() const;
        [[nodiscard]] [[maybe_unused]] int rightTextOffset() const;

        [[maybe_unused]] int rightMaxWidth(){return m_rightMaxWidth;}


        void setRightMaxWidth(int w){m_rightMaxWidth = w;}

        int splitRectWidth() {return m_splitRectWidth;}


    private:
        void updateLeftTextOffset();   ///< 更新内部计算结果
        void updateRightTextOffset();   ///< 更新内部计算结果
        void updateLogoSplitSpace();
        void updateSplitRectWidth();

    private:
        CSize m_showImageSize{1,1};          ///< 显示的图片的大小
        Margin m_margin;                ///< scene margin{left right top bottom}
        int m_logoSpace {10};           ///< show image space with log: TODO draw rect with height == m_logoSpace
        CSize m_logoSize{64, 64};  ///< logo size

        int m_leftTextOffset{m_margin.left};      ///< 左边部分文字的偏移长度 (left + image.width() + right) * 0.1
        int m_rightTextOffset{m_margin.right};    ///< 左边部分文字的偏移长度 (logoSize.w) * 1.45 right in it
        int m_logoSplitLineSpace{15};             ///< logo & 显示的文字和分割线之间的距离

        int m_splitRectWidth{0};    ///< 文字与图标分割线(rect & fill)的宽度

        int m_rightMaxWidth{0};       ///< right top & bottom show text item max width

    };

} // CM

#endif //CAMERAMARK_SCENELAYOUT_H
