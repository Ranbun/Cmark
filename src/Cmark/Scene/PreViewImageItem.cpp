#include "PreViewImageItem.h"

namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem *parent, const SceneLayoutSettings & layout)
    : QGraphicsPixmapItem(parent)
    , m_sceneLayout(layout)
    {

    }

    void PreViewImageItem::updatePixmapSize()
    {
        if(m_pixmap.isNull()) return;
        auto imageSize = m_sceneLayout.ImageSize();
        setPixmap(PreViewImageItem::scaledPixmap(m_pixmap, imageSize.w, imageSize.h));
    }

    void PreViewImageItem::resetPixmap(const QPixmap & previewPixmap)
    {
        m_pixmap = previewPixmap;
        if(m_pixmap.isNull())
        {
            throw std::runtime_error("Pixmap is Null!");
        }

        auto rect = sceneBoundingRect().toRect();
        setPixmap(PreViewImageItem::scaledPixmap(m_pixmap, rect.width(),rect.height()));

        auto pixSize = QSizeF(m_pixmap.size());
        m_ImageRatio = pixSize.height() / pixSize.width();
    }

    void PreViewImageItem::applyLayout()
    {
        updatePixmapSize();
        updatePixmapPosition();
    }

    void PreViewImageItem::updatePixmapPosition()
    {
        const auto currentScene = scene();
        if(!currentScene) return;

        auto posX = m_sceneLayout.getMargin().left;
        auto posY = m_sceneLayout.getMargin().right;
        setPos(posX, posY);
    }

    QPixmap PreViewImageItem::scaledPixmap(const QPixmap &image, int w, int h)
    {
        if(image.isNull())
        {
            return {w,h};
        }
        return image.scaled(w,h,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    PreViewImageItem::~PreViewImageItem() = default;

} // CM
