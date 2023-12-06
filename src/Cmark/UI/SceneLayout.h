#ifndef CAMERAMARK_SCENELAYOUT_H
#define CAMERAMARK_SCENELAYOUT_H

#include <Base/CSize.h>

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

        [[nodiscard]] const Margin & getMargin() const {return m_margin;}
        void setMargin(int left,int right,int top,int bottom);

        /// logo
        void setLogoSpace(int space);
        [[nodiscard]] int logoSpace() const;

        [[nodiscard]] const CSize & LogoSize()const {return m_LogoSize;}

        void setLogoSize(int w,int h){ setLogoSize({w,h});}
        void setLogoSize(const CSize & size) {m_LogoSize = size;}

    private:
        Margin m_margin;                ///< scene margin{left right top bottom}
        int m_logoSpace {10};           ///< show image space with log
        CSize m_LogoSize{64,64};  ///< logo size
    };

} // CM

#endif //CAMERAMARK_SCENELAYOUT_H
