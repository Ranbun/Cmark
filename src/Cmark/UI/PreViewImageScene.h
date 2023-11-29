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
        int x;
        int y;
        int z;
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
        void setPixmapItem(QGraphicsPixmapItem *item);
        void updateSceneRect(const Size & rectSize);
        void Init();

        void updateTexItems(const CM::ExifMap & map);

    private:
        QGraphicsPixmapItem * m_pixmapItem;
        std::vector<ImageInfoItem> m_infos;

    protected:

    private:
        void InitTexItems();


    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H
