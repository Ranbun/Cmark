#ifndef CAMERAMARK_PREVIEWIMAGEITEM_H
#define CAMERAMARK_PREVIEWIMAGEITEM_H

#include <QGraphicsPixmapItem>
#include "PreViewImageScene.h"

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
        explicit PreViewImageItem(QGraphicsItem * parent = nullptr);
        ~PreViewImageItem()  override = default;

        void resetPixmap(const QPixmap & previewPixmap);
        void update();

    private:
        void updatePixmapSize();
        void updatePos();

    private:
        QPixmap m_pixmap;
        ImageLayout m_margin{};
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H
