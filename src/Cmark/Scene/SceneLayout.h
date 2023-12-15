#ifndef CAMERAMARK_SCENELAYOUT_H
#define CAMERAMARK_SCENELAYOUT_H

#include "Base/CSize.h"
#include "Base/CPoint.h"

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
        int bottom {10};
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
        [[maybe_unused]] void setLogoWithImageSpace(int space);

        [[nodiscard]] int logoWithImageSpace() const;
        [[nodiscard]] int logoWithSplitLineSpace() const;

        [[nodiscard]] const CSize & LogoSize()const;
        void setLogoSize(int w,int h);
        void setLogoSize(const CSize & size);


        [[maybe_unused]] void setImageSize(const CSize & size);
        [[nodiscard]] const CSize & ImageSize() const;

        [[nodiscard]] [[maybe_unused]] int leftTextOffset() const;
        [[nodiscard]] [[maybe_unused]] int rightTextOffset() const;

        [[maybe_unused]] [[nodiscard]] int rightTextMaxWidth() const;

        /**
         * @brief 设置right_top & right_bottom 显示的文字的最大长度
         * @param w 需要设置的长度
         */
        void setRightMaxWidth(int w){m_rightMaxWidth = w;}

        /**
         * @brief 分割rect的宽度
         * @return
         */
        [[nodiscard]] int splitRectWidth() const {return m_splitRectWidth;}

        const CPoint & logoPosition(){return m_logoPosition;}


    private:
        /**
         * @brief 更新左侧开始渲染文字前的便宜
         */
        void updateLeftTextOffset();

        /**
         * @brief 更新右侧文字结尾的距离
         */
        void updateRightTextOffset();

        /**
         * @brief 更新logo 到 splitRect的距离
         */
        void updateLogoSplitSpace();

        /**
         * @brief 更新分割rect的宽度
         */
        void updateSplitRectWidth();

        /**
         * @brief 计算logo的position
         */
        void updateLogoPosition();

    private:
        CSize m_showImageSize{1,1};          ///< 显示的图片的大小
        Margin m_margin;                ///< scene margin{left right top bottom}

        int m_logoSpaceWithImage {10};           ///< show image space with log: TODO draw rect with height == m_logoSpace
        CSize m_logoSize{64, 64};  ///< logo size
        CPoint m_logoPosition{-1,-1};

        int m_leftTextOffset{m_margin.left};      ///< 左边部分文字的偏移长度 (left + image.width() + right) * 0.1
        int m_rightTextOffset{m_margin.right};    ///< 左边部分文字的偏移长度 (LogoSize.w) * 1.45 right in it
        int m_logoSplitRectSpace{15};             ///< logo & 显示的文字和分割线之间的距离

        int m_splitRectWidth{0};    ///< 文字与图标分割线(rect & fill)的宽度
        int m_rightMaxWidth{0};       ///< right top & bottom show text item max width

    };

} // CM

#endif //CAMERAMARK_SCENELAYOUT_H
