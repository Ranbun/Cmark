#ifndef CAMERAMARK_PREVIEWIMAGEITEM_H
#define CAMERAMARK_PREVIEWIMAGEITEM_H

#include <QGraphicsPixmapItem>
#include "Scene/PreViewImageScene.h"
#include "Scene/SceneLayout.h"

namespace CM
{
    class PreViewImageItem : public QGraphicsPixmapItem
    {
    public:
        explicit PreViewImageItem(QGraphicsItem *parent , const SceneLayout & layout);
        ~PreViewImageItem()  override;

        /**
         * @brief 重新设置显示的图片
         * @param previewPixmap 显示的图片
         */
        void resetPixmap(const QPixmap & previewPixmap);

        /**
         * @brief 更新当前的pixmap item(更新pixmap的大小和在场景中的位置)
         */
        void update();

        /**
         * @brief 更新显示pixmap的大小
         */
        void updatePixmapSize();

        /**
         * @brief 更新pixmap的位置
         */
        void updatePixmapPosition();

        [[nodiscard]] const QPixmap & target() const {return m_pixmap;}

    private:
        static QPixmap scaledPixmap(const QPixmap &image, int w, int h);

    private:
        QPixmap m_pixmap;
        const SceneLayout & m_sceneLayout;
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H
