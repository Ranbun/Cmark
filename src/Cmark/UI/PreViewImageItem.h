#ifndef CAMERAMARK_PREVIEWIMAGEITEM_H
#define CAMERAMARK_PREVIEWIMAGEITEM_H

#include <QGraphicsPixmapItem>
#include "PreViewImageScene.h"
#include "SceneLayout.h"

namespace CM
{
    struct ImageLayout
    {
        int leftMargin;
        int topMargin;
        int bottomMargin;
        int rightMargin;
    };

    class PreViewImageItem : public QGraphicsPixmapItem
    {
    public:
        explicit PreViewImageItem(QGraphicsItem *parent , const SceneLayout & layout);
        ~PreViewImageItem()  override = default;

        void resetPixmap(const QPixmap & previewPixmap);
        void update();

        void updatePixmapSize();
        void updatePos();

    private:
        static QPixmap scaledPixmap(const QPixmap &image, int w, int h);

    private:
        QPixmap m_pixmap;
        ImageLayout m_margin{};
        const CM::SceneLayout & m_sceneLayout;
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H
