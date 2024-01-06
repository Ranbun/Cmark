#include "PreViewImageItem.h"

#include <iostream>

#include "SceneDef.h"
#include "sources/PictureManager.h"

#if _DEBUG

#include <qDebug>

#endif


namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem* parent, const std::shared_ptr<SceneLayoutSettings>& layout)
        : QGraphicsPixmapItem(parent)
          , m_SceneLayout(layout)
    {
//        setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
//        setFlag(QGraphicsPixmapItem::ItemIgnoresTransformations);
    }

    void PreViewImageItem::resizeImage(const int w, const int h)
    {
        const auto pixmap = target();
        if (!pixmap || pixmap->isNull())
        {
            return;
        }

        setPixmap(scaledPixmap(pixmap, w, h));
    }

    void PreViewImageItem::resetPixmap(const size_t imageIndex)
    {
        const auto pic = CM::PictureManager::getImage(imageIndex);
        if(pic->isNull())
        {
            throw std::runtime_error("Pixmap is Null!");
        }

        const auto & [w,h] = m_SceneLayout.lock()->imageSize();
        setPixmap(scaledPixmap(pic,w,h));

        updateImageRatio(pic);

        const auto rect = boundingRect().toRect();
        update(rect);
    }

    void PreViewImageItem::applyLayout(const std::shared_ptr<SceneLayoutSettings>& layout)
    {
        if(layout)
        {
            const auto& [w, h] = layout->imageSize();
            resizeImage(w, h);

            const auto& [x,y] = layout->imagePos();
            setPos(0, 0);
        }

    }

    const float& PreViewImageItem::imageRatio() const
    {
        return m_ImageRatio;
    }

    auto PreViewImageItem::validImage() const -> bool
    {
        const auto p = target();
        if(!p || p->isNull())
        {
            return false;
        }
        return true;
    }

    QPixmap PreViewImageItem::scaledPixmap(const QPixmap& image, int w, int h)
    {
        if (image.isNull())
        {
            return {w, h};
        }
        return image.scaled(w, h,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    }

    QPixmap PreViewImageItem::scaledPixmap(const std::shared_ptr<QPixmap>& image, const int w, const int h)
    {
        auto p = scaledPixmap(*image, w, h);
        const auto size = p.size();
        std::cout << "scale pixmap";
        qDebug() <<size;
        return p;
    }

    std::shared_ptr<QPixmap> PreViewImageItem::target() const
    {
        const auto pixmapIndex = data(static_cast<int>(CM::GraphicsItemDataIndex::PixmapIndex)).toULongLong();
        return CM::PictureManager::getImage(pixmapIndex);
    }

    void PreViewImageItem::updateImageRatio(const std::shared_ptr<QPixmap>& tar)
    {
        const auto pixSize = QSizeF(tar->size());
        m_ImageRatio = static_cast<float>(pixSize.width() / pixSize.height());  /// w / h
    }

    PreViewImageItem::~PreViewImageItem() = default;
} // CM
