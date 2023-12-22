#include "PreViewImageItem.h"

namespace CM
{
    PreViewImageItem::PreViewImageItem(QGraphicsItem* parent, const std::shared_ptr<SceneLayoutSettings>& layout)
        : QGraphicsPixmapItem(parent)
          , m_SceneLayout(layout)
    {
    }

    void PreViewImageItem::updatePixmapSize()
    {
        const auto layout = m_SceneLayout.lock();
        if (m_Pixmap.isNull()) return;
        const auto [ImageW, ImageH] = layout->ImageSize();
        setPixmap(scaledPixmap(m_Pixmap, ImageW, ImageH));
    }

    void PreViewImageItem::resetPixmap(const QPixmap& previewPixmap)
    {
        m_Pixmap = previewPixmap;
        if (m_Pixmap.isNull())
        {
            throw std::runtime_error("Pixmap is Null!");
        }

        const auto rect = sceneBoundingRect().toRect();
        setPixmap(PreViewImageItem::scaledPixmap(m_Pixmap, rect.width(), rect.height()));

        const auto pixSize = QSizeF(m_Pixmap.size());
        m_ImageRatio = static_cast<float>(pixSize.height() / pixSize.width());
    }

    void PreViewImageItem::applyLayout()
    {
        updatePixmapSize();
        updatePixmapPosition();
    }

    void PreViewImageItem::updatePixmapPosition()
    {
        if (const auto currentScene = scene();
            !currentScene)
        {
            return;
        }

        const auto layout = m_SceneLayout.lock();

        const auto& [l,r,t,b] = layout->getMargin();
        setPos(l, t);
    }

    const float& PreViewImageItem::imageRatio() const
    {
        return m_ImageRatio;
    }

    const QPixmap& PreViewImageItem::target() const
    {
        return m_Pixmap;
    }

    QPixmap PreViewImageItem::scaledPixmap(const QPixmap& image, int w, int h)
    {
        if (image.isNull())
        {
            return {w, h};
        }
        return image.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    PreViewImageItem::~PreViewImageItem() = default;
} // CM
