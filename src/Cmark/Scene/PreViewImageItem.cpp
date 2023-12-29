#include "PreViewImageItem.h"

#include "SceneDef.h"
#include "sources/PictureManager.h"

namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem* parent, const std::shared_ptr<SceneLayoutSettings>& layout)
        : QGraphicsPixmapItem(parent)
          , m_SceneLayout(layout)
    {
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

        const auto & [w,h] = m_SceneLayout.lock()->ImageSize();
        setPixmap(scaledPixmap(pic,w,h));

        updateImageRatio(pic);

        const auto rect = boundingRect().toRect();
        update(rect);
    }

    void PreViewImageItem::applyLayout(const std::shared_ptr<SceneLayoutSettings>& layout)
    {
        if(layout)
        {
            const auto& [w, h] = layout->ImageSize();
            resizeImage(w, h);

            const auto& [l, r, t, b] = layout->getMargin();
            setPos(l, t);
        }

    }

    const float& PreViewImageItem::imageRatio() const
    {
        return m_ImageRatio;
    }

    bool PreViewImageItem::showSplitLine()
    {
        auto p = target();
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
        return image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QPixmap PreViewImageItem::scaledPixmap(const std::shared_ptr<QPixmap>& image, const int w, const int h)
    {
        return scaledPixmap(*image, w, h);
    }

    std::shared_ptr<QPixmap> PreViewImageItem::target() const
    {
        const auto pixmapIndex = data(static_cast<int>(CM::GraphicsItemDataIndex::PixmapIndex)).toULongLong();
        return CM::PictureManager::getImage(pixmapIndex);
    }

    void PreViewImageItem::updateImageRatio(const std::shared_ptr<QPixmap>& tar)
    {
        const auto pixSize = QSizeF(tar->size());
        m_ImageRatio = static_cast<float>(pixSize.height() / pixSize.width());
    }

    PreViewImageItem::~PreViewImageItem() = default;
} // CM
