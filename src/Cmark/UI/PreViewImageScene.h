#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H
#define CAMERAMARK_PREVIEWIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "../Base/type.h"

#include <algorithm>

namespace CM
{
    struct Point
    {
        [[maybe_unused]] int x;
        [[maybe_unused]] int y;
        [[maybe_unused]] int z;
    };

    using Size = Point;

    struct ImageInfoItem
    {
        CM::ExifKey m_key;
        std::string m_title;
        QGraphicsTextItem * m_field;
        Point pos{-1,-1};  ///< position
        std::string m_infos{};
        bool m_visible{false};
    };

    class PreViewImageScene : public QGraphicsScene
    {
    public:
        explicit PreViewImageScene(QObject * parent = nullptr);
        ~PreViewImageScene() override = default;
        void Init();

        /**
         * @brief 记录pixmap Item
         * @param item pixmap item
         */
        void setPixmapItem(QGraphicsPixmapItem *item);

        /**
         * @brief 根据当前使用的视图更新场景的大小(此时场景和视图具有1对一关系)
         * @param view 当前使用的视图
         */
        void updateSceneRect(QGraphicsView *view);

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

    private:
        QGraphicsPixmapItem * m_pixmapItem;
        QGraphicsPixmapItem * m_logoPixmapItem;
        std::vector<ImageInfoItem> m_infos;

    protected:

    private:
        void InitTexItems();
        void InitLogoItem();


    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H
