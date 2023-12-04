#include "PreViewImageItem.h"

namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
    {
        m_margin = {30,30,213,30};
    }

    void PreViewImageItem::updatePixmapSize()
    {
#if _DEBUG
        auto ph = m_pixmap.height();
        auto pw = m_pixmap.width();
#endif

        const auto currentScene = scene();
        if(!currentScene) return;
        const auto &  sceneRect = currentScene->sceneRect();

        CM::Size sceneRectSize{(int)sceneRect.width(),(int)sceneRect.height(),0};

        auto newWidth = sceneRectSize.x - m_margin.leftMargin - m_margin.rightMargin;
        auto newHeight = static_cast<int>(static_cast<float>(m_pixmap.height()) / static_cast<float>(m_pixmap.width()) * static_cast<float>(newWidth));

        if(m_pixmap.isNull()) return ;

        setPixmap(PreViewImageItem::scaledPixmap(m_pixmap,newWidth,newHeight));
    }

    void PreViewImageItem::resetPixmap(const QPixmap &previewPixmap)
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
        updatePos();
    }

    void PreViewImageItem::updatePos()
    {
        const auto currentScene = scene();
        if(!currentScene) return;
        const auto &  sceneRect = currentScene->sceneRect();

        auto posX = m_margin.leftMargin;
        auto posY = m_margin.topMargin;
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
} // CM
