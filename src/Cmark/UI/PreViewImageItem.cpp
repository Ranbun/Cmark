//
// Created by RanLH on 11/29/2023.
//

#include "PreViewImageItem.h"

namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent)
    {

    }

    void PreViewImageItem::updatePixmapSize()
    {
        const auto currentScene = scene();
        if(!currentScene) return;
        const auto &  sceneRect = currentScene->sceneRect();
        CM::Size sceneRectSize{(int)sceneRect.width(),(int)sceneRect.height(),0};

#if _DEBUG
        auto ph = m_pixmap.height();
        auto pw = m_pixmap.width();
#endif
        auto w = (int)(sceneRectSize.x * 0.6);
        auto h = static_cast<int>(static_cast<float>(m_pixmap.height()) / static_cast<float>(m_pixmap.width()) * w);

        if(m_pixmap.isNull()) return ;

        setPixmap(m_pixmap.scaled(w,h,Qt::KeepAspectRatio));
    }

    void PreViewImageItem::resetPixmap(const QPixmap &previewPixmap)
    {
        m_pixmap = previewPixmap;
        if(m_pixmap.isNull())
        {
            throw std::runtime_error("Pixmap is Null!");
        }

        update();
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

        const auto & rect = boundingRect();
        const auto imageY = sceneRect.height() / 2 - rect.height() / 2;
        constexpr auto imageX = 0;
        setPos(imageX, imageY);
    }
} // CM
