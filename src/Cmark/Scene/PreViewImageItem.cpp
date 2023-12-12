#include "PreViewImageItem.h"

namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem *parent, const SceneLayout & layout)
    : QGraphicsPixmapItem(parent)
    , m_sceneLayout(layout)
    {

    }

    void PreViewImageItem::updatePixmapSize()
    {
        /// TODO: image size should ......
        const auto currentScene = scene();
        if(!currentScene) return;
        const auto &  sceneRect = currentScene->sceneRect().toRect();

        const auto & [left,right,top,bottom] = m_sceneLayout.getMargin();

        auto newWidth = sceneRect.width() - left - right;
        auto ImageRatio = static_cast<float>(m_pixmap.height()) / static_cast<float>(m_pixmap.width());
        auto newHeight = static_cast<int>(ImageRatio * static_cast<float>(newWidth));

        if(m_pixmap.isNull()) return;

        setPixmap(PreViewImageItem::scaledPixmap(m_pixmap,newWidth,newHeight));
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
    }

    void PreViewImageItem::update()
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
