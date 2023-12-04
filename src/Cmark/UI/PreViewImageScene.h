#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H
#define CAMERAMARK_PREVIEWIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "../Base/Type.h"
#include "../Base/CPoint.h"

#include <algorithm>

namespace CM
{
    using Size = CPoint;

    struct ImageInfoItem
    {
        CM::ExifKey m_key;
        std::string m_title;
        QGraphicsTextItem * m_field;
        CPoint pos{-1, -1};  ///< position
        std::string m_infos{};
        bool m_visible{false};
    };

    enum class Exiflayout
    {
        left_top,
        left_bottom,
        right_top,
        right_bottom
    };

    /**
     * @brief 记录显示的Exif信息和它的布局位置
     */
    struct showExifInfo
    {
        Exiflayout layout;
        std::vector<ExifKey> m_keys;
    };

    class PreViewImageScene : public QGraphicsScene
    {
    public:
        explicit PreViewImageScene(QObject * parent = nullptr);
        ~PreViewImageScene() override = default;
        void Init();

        /**
         * @brief 根据当前使用的视图更新场景的大小(此时场景和视图具有1对一关系)
         * @param view 当前使用的视图
         */
        void updateSceneRect(QGraphicsView *view, const QRectF &sceneSize);

        /**
         * @brief 当前显示的所有文字信息
         * @param map 所有的文字信息
         */
        void updateTexItems(const CM::ExifList & map);

        /**
         * @brief 更新所有文字的位置
         */
        void updateTexItems();

        void updateLogoPixmap(const QPixmap & logo);

        void updateLogoPos();

        void updatePreviewPixmap(const QPixmap & pixmap);

        void saveLoadedPixmap();


    private:
        QGraphicsPixmapItem * m_previewImageItem;
        QGraphicsPixmapItem * m_logoPixmapItem;
        std::vector<ImageInfoItem> m_infos;
        std::vector<showExifInfo> m_showInfos;

    protected:

    private:
        void InitTexItems();
        void InitLogoItem();
        void InitPreviewImageItem();


    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H
