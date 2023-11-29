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

        void updatePixmapSize();
        void resetPixmap(const QPixmap & previewPixmap);
        void updatePos();

        void update();

    private:
        QPixmap m_pixmap;
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H
