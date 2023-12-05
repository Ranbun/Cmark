#ifndef CAMERAMARK_SCENELAYOUT_H
#define CAMERAMARK_SCENELAYOUT_H

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

        const Margin & getMargin(){return m_margin;}
        void setMargin(int left,int right,int top,int bottom);

        void setLogoSpace(int space);;
        int logoSpace();

    private:
        Margin m_margin;
        int m_logoSpace {10};    ///< show image space with log

    };

} // CM

#endif //CAMERAMARK_SCENELAYOUT_H
