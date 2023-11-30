#ifndef CAMERAMARK_PREVIEWIMAGEITEM_H
#define CAMERAMARK_PREVIEWIMAGEITEM_H

#include <QGraphicsPixmapItem>
#include "PreViewImageScene.h"

namespace CM
{
    class PreViewImageItem : public QGraphicsPixmapItem
    {
    public:
        explicit PreViewImageItem(QGraphicsItem * parent = nullptr);
        ~PreViewImageItem()  override = default;

        void resetPixmap(const QPixmap & previewPixmap);
        void update();

    private:
        void updatePixmapSize();
        void updatePos();

    private:
        QPixmap m_pixmap;
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H
