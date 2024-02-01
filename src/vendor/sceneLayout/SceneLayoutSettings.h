#ifndef CAMERAMARK_SCENELAYOUTSETTINGS_H
#define CAMERAMARK_SCENELAYOUTSETTINGS_H

namespace CM
{
    /**
     * @brief Margin layout
     */
    struct Margin  ///< use pixel
    {
        int m_Left {0};
        int m_Right {0};
        int m_Top {0};
        int m_Bottom {10};
    };

    class SceneLayoutSettings
    {
    public:
        struct LSize
        {
            int w;
            int h;
        };

        struct LPoint
        {
            int x;
            int y;
        };
        using LPos = LPoint;

        static const LSize & fixPreViewImageSize() { return m_PreViewImageSize; }

    private:
        inline static LSize m_PreViewImageSize = LSize{ 960, 720 };

    public:
        SceneLayoutSettings();
        ~SceneLayoutSettings();

        /**
         * @brief 计算一些布局使用的结果
         */
        void update();

        [[nodiscard]] const Margin & getMargin() const;
        Margin & getMargin();
        void setMargin(int left,int right,int top,int bottom);

        /// Image
        [[nodiscard]] LPos imagePos() const;

        /// logo
        [[deprecated]] void setLogoWithImageSpace(int space);
        [[deprecated]] [[nodiscard]] int logoWithImageSpace() const;
        [[deprecated]] [[nodiscard]] int logoWithSplitLineSpace() const;

        /// LOGO
        [[nodiscard]] const LSize & logoSize()const;

        [[deprecated]] void setLogoSize(int w,int h);
        [[deprecated]] void setLogoSize(const LSize &size);

        [[maybe_unused]] void setImageSize(const LSize &size);
        [[nodiscard]] const LSize & imageSize() const;

        [[deprecated]] [[nodiscard]] int leftTextOffset() const;
        [[deprecated]] [[nodiscard]] int rightTextOffset() const;
        [[deprecated]] [[nodiscard]] int rightTextMaxWidth() const;

        /**
         * @brief 设置right_top & right_bottom 显示的文字的最大长度
         * @param w 需要设置的长度
         */
        void setRightMaxWidth(int w);

        /**
         * @brief 分割rect的宽度
         * @return
         */
        [[nodiscard]] int splitRectWidth() const;

        [[nodiscard]] const LPoint & logoPosition() const;

        void resetLayout();

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

        [[maybe_unused]] LSize m_ShowImageSize{1,1};
        [[maybe_unused]] Margin m_Margin;
        [[maybe_unused]] LPoint m_LogoPosition{ -1,-1 };
        [[maybe_unused]] LSize m_LogoSize{ 64, 64 };

        int m_LogoSpaceWithImage {10};              ///< logo的上边部分到image的bottom的距离
        int m_LeftTextOffset{m_Margin.m_Left};      ///< 左边部分文字的偏移长度 (left + image.width() + right) * 0.1
        int m_RightTextOffset{m_Margin.m_Right};    ///< 左边部分文字的偏移长度 (LogoSize.w) * 1.45 right in it
        int m_LogoSplitRectSpace{15};               ///< logo & 显示的文字和分割线之间的距离

        int m_SplitRectWidth{0};      ///< 文字与图标分割线(rect & fill)的宽度
        int m_RightMaxWidth{200};       ///< right top & bottom show text item max width 显示的文字的最大宽度
    };

} // CM

#endif //CAMERAMARK_SCENELAYOUTSETTINGS_H
