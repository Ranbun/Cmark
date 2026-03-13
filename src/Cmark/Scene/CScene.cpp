#include "CMark.h"
#include "CScene.h"
#include "PreViewImageItem.h"
#include "SceneDef.h"

namespace CM
{
CScene::CScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_logoItem(new QGraphicsPixmapItem)
    , m_sceneLayout(std::make_shared<SceneLayoutSettings>())
{
    m_showImageItem = new PreViewImageItem(nullptr, m_sceneLayout);
    init();
    const auto &[fixImageSizeW, fixImageSizeH] = SceneLayoutSettings::fixPreViewImageSize();
    m_sceneLayout->setImageSize({fixImageSizeW, fixImageSizeH});
    m_sceneLayout->update();
}

CScene::~CScene() = default;

void CScene::init()
{
    m_plainTextFont.setFamily("Microsoft YaHei");
    m_plainTextFont.setPixelSize(20);

    initMargin();
    initTargetImageItem();
    initTexItems();
    initLogoItem();
    initSplitRect();

    connect(this, &CScene::noScenesChanged, [this](bool) { updateOverlayItemsVisibility(); });
}

void CScene::initTexItems()
{
    m_showInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::LeftTop, {ExifKey::CameraModel}});
    m_showInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::LeftBottom, {ExifKey::ImageDate}});
    m_showInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::RightTop, {ExifKey::LensModel}});
    m_showInfos.emplace_back(ShowExifInfo{
        ShowExifTexPositionIndex::RightBottom,
        {ExifKey::FocalLength, ExifKey::FStop, ExifKey::ExposureTime, ExifKey::ISOSpeed}});

    m_textItem.insert({ShowExifTexPositionIndex::LeftTop, addText("")});
    m_textItem.insert({ShowExifTexPositionIndex::LeftBottom, addText("")});
    m_textItem.insert({ShowExifTexPositionIndex::RightTop, addText("")});
    m_textItem.insert({ShowExifTexPositionIndex::RightBottom, addText("")});

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(11);
    font.setPixelSize(13);

    for (auto &[key, item] : m_textItem)
    {
        switch (key)
        {
        case ShowExifTexPositionIndex::LeftTop:
        case ShowExifTexPositionIndex::RightTop:
            font.setBold(true);
            item->setFont(font);
            font.setBold(false);
            break;
        case ShowExifTexPositionIndex::LeftBottom:
        case ShowExifTexPositionIndex::RightBottom:
            item->setFont(font);
            break;
        }
    }
}

void CScene::resetTexItemsPlainText(const ExifInfoMap &exifInfoMap)
{
    m_targetImageExifInfoMap = exifInfoMap;

    for (const auto &[layoutIndex, keys] : m_showInfos)
    {
        const auto item = m_textItem.at(layoutIndex);
        std::vector<std::string> exifInfos;
        for (const auto &key : keys)
            exifInfos.push_back(m_targetImageExifInfoMap[key]);

        auto res = std::accumulate(exifInfos.begin(), exifInfos.end(), std::string(),
            [](const std::string &a, const std::string &b) {
                if (a.empty() && b.empty())
                    return std::string();
                const auto tail = b.empty() ? std::string() : std::string(" ") + b;
                return a + tail;
            });
        item->setVisible(!res.empty());
        item->setPlainText(res.c_str());
    }

    const auto rightTopTextRect = m_textItem.at(ShowExifTexPositionIndex::RightTop)->boundingRect();
    const auto rightBottomTextRect = m_textItem.at(ShowExifTexPositionIndex::RightBottom)->boundingRect();
    auto maxW = std::max(rightTopTextRect.width(), rightBottomTextRect.width());
    if (maxW < 200)
        maxW = 200;
    m_sceneLayout->setRightMaxWidth(static_cast<int>(maxW));

    updateTexItemsPosition();
}

void CScene::updateTexItemsPosition()
{
    auto &[left, right, top, bottom] = m_sceneLayout->getMargin();
    const auto logoWithImageSpacing = m_sceneLayout->logoWithImageSpace();
    const auto &logoSize = m_sceneLayout->logoSize();
    const auto &imageSize = m_sceneLayout->imageSize();

    for (const auto &[layoutIndex, keys] : m_showInfos)
    {
        Q_UNUSED(keys);
        const auto item = m_textItem.at(layoutIndex);
        const auto &itemRect = item->boundingRect().toRect();

        switch (layoutIndex)
        {
        case ShowExifTexPositionIndex::LeftTop:
            item->setPos(QPoint(m_sceneLayout->leftTextOffset(), top + imageSize.h + logoWithImageSpacing));
            break;
        case ShowExifTexPositionIndex::LeftBottom:
            item->setPos(QPoint(m_sceneLayout->leftTextOffset(),
                top + logoWithImageSpacing + imageSize.h + logoSize.h - itemRect.height()));
            break;
        case ShowExifTexPositionIndex::RightTop:
            item->setPos(QPoint(
                left + imageSize.w + right - m_sceneLayout->rightTextMaxWidth() - m_sceneLayout->rightTextOffset(),
                top + imageSize.h + logoWithImageSpacing));
            break;
        case ShowExifTexPositionIndex::RightBottom:
            item->setPos(QPoint(
                left + imageSize.w + right - m_sceneLayout->rightTextMaxWidth() - m_sceneLayout->rightTextOffset(),
                top + logoWithImageSpacing + imageSize.h + logoSize.h - itemRect.height()));
            break;
        }
    }
}

void CScene::initLogoItem()
{
    QPixmap pixmap;
    pixmap.fill(Qt::TransparentMode);
    m_logoItem->setPixmap(pixmap);
    addItem(m_logoItem);
}

void CScene::resetLogoPixmap(const std::shared_ptr<QPixmap> &logo, CameraIndex cameraIndex)
{
    const auto &[logoW, logoH] = m_sceneLayout->logoSize();
    const auto ratio = static_cast<float>(logo->height()) / static_cast<float>(logo->width());
    const auto newW = static_cast<int>(std::round(static_cast<float>(logoH) / ratio));

    m_sceneLayout->setLogoSize(newW, logoH);
    m_logoItem->setPixmap(logo->scaled({newW, logoH}, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_logoItem->setData(static_cast<int>(GraphicsItemDataIndex::CameraIndex), static_cast<int>(cameraIndex));
    m_logoItem->setVisible(cameraIndex != CameraIndex::None);

    updateLogoPosition();
}

void CScene::updateLogoPosition()
{
    const auto logoSpaceWithImage = m_sceneLayout->logoWithImageSpace();
    const auto [imageW, imageH] = m_sceneLayout->imageSize();
    const auto &[l, r, t, b] = m_sceneLayout->getMargin();
    Q_UNUSED(b);

    const auto x = l + r + imageW - m_sceneLayout->logoWithSplitLineSpace() * 2.0 - m_sceneLayout->logoSize().w
        - m_sceneLayout->rightTextMaxWidth() - m_sceneLayout->rightTextOffset() - m_sceneLayout->splitRectWidth();
    const auto y = t + imageH + logoSpaceWithImage;
    m_logoItem->setPos(x, y);
}

void CScene::initTargetImageItem()
{
    QPixmap pixmap;
    pixmap.fill(Qt::transparent);
    m_showImageItem->setPixmap(pixmap);
    addItem(m_showImageItem);
}

void CScene::resetPreviewImageTarget(const QPixmap &pixmap, size_t imageIndexCode)
{
    m_sceneLayout->setImageSize({pixmap.width(), pixmap.height()});
    m_showImageItem->resetPixmap(imageIndexCode);
    m_showImageItem->setData(static_cast<int>(GraphicsItemDataIndex::PixmapIndex),
        QVariant::fromValue(static_cast<qulonglong>(imageIndexCode)));

    m_showItemFlags = m_showImageItem->validImage();
}

void CScene::initMargin()
{
    const auto rect = sceneRect().toRect();
    const auto &[left, right, top, bottom] = m_sceneLayout->getMargin();

    m_left = new QGraphicsRectItem(0, 0, left, rect.height());
    m_right = new QGraphicsRectItem(0, 0, right, rect.height());
    m_top = new QGraphicsRectItem(0, 0, rect.width(), top);
    m_bottom = new QGraphicsRectItem(0, 0, rect.width(), bottom);

    const QPen rectPen(Qt::transparent);
    m_left->setPen(rectPen);
    m_right->setPen(rectPen);
    m_top->setPen(rectPen);
    m_bottom->setPen(rectPen);

    addItem(m_left);
    addItem(m_right);
    addItem(m_top);
    addItem(m_bottom);
}

void CScene::updateMarginItems()
{
    const auto &[left, right, top, bottom] = m_sceneLayout->getMargin();
    const auto logoSpaceWithShowImage = m_sceneLayout->logoWithImageSpace();
    const auto &imageRect = m_sceneLayout->imageSize();
    const auto &logoRect = m_sceneLayout->logoSize();

    const auto sceneBoundMarginRectH = top + imageRect.h + logoSpaceWithShowImage + logoRect.h + bottom;
    const auto sceneBoundMarginRectW = left + imageRect.w + right;

    m_left->setPen(QPen(Qt::NoPen));
    m_right->setPen(QPen(Qt::NoPen));
    m_top->setPen(QPen(Qt::NoPen));
    m_bottom->setPen(QPen(Qt::NoPen));

    m_left->setRect(0, 0, left, sceneBoundMarginRectH);
    m_right->setRect(left + imageRect.w, 0, right, sceneBoundMarginRectH);
    m_top->setRect(0, 0, sceneBoundMarginRectW, top);
    m_bottom->setRect(0, top + imageRect.h + logoSpaceWithShowImage + logoRect.h, sceneBoundMarginRectW, bottom);

    m_left->setVisible(left != 0);
    m_right->setVisible(right != 0);
    m_top->setVisible(top != 0);
}

void CScene::initSplitRect()
{
    m_splitRectItem = new QGraphicsRectItem;

    QRadialGradient radialGradient(100, 100, 100, 0, 100);
    radialGradient.setColorAt(0, QColor(253, 253, 253));
    radialGradient.setColorAt(0.1, QColor(253, 253, 253));
    radialGradient.setColorAt(0.2, QColor(216, 216, 216));
    radialGradient.setColorAt(0.5, QColor(218, 218, 218));
    radialGradient.setColorAt(0.6, QColor(217, 217, 217));
    radialGradient.setColorAt(0.9, QColor(222, 222, 222));
    radialGradient.setColorAt(1.0, QColor(241, 241, 241));

    m_splitRectItem->setBrush(radialGradient);
    m_splitRectItem->setPen(Qt::NoPen);
    addItem(m_splitRectItem);
}

void CScene::updateSplitRect()
{
    const auto &logoSize = m_sceneLayout->logoSize();
    const auto imageH = m_sceneLayout->imageSize().h;
    const auto imageW = m_sceneLayout->imageSize().w;
    const auto spacing = m_sceneLayout->logoWithImageSpace();
    const auto &[left, right, top, bottom] = m_sceneLayout->getMargin();
    Q_UNUSED(bottom);

    const auto splitRectW = m_sceneLayout->splitRectWidth();
    const auto x = left + right + imageW - m_sceneLayout->rightTextOffset() - m_sceneLayout->rightTextMaxWidth()
        - m_sceneLayout->logoWithSplitLineSpace() - splitRectW;
    const auto y = top + imageH + spacing;

    m_splitRectItem->setRect(x, y, splitRectW, logoSize.h);

    const auto r = m_splitRectItem->rect();
    QLinearGradient linearGradient(r.left(), r.top(), r.right(), r.top());
    linearGradient.setColorAt(0, QColor(253, 253, 253));
    linearGradient.setColorAt(0.2, QColor(216, 216, 216));
    linearGradient.setColorAt(0.5, QColor(218, 218, 218));
    linearGradient.setColorAt(0.6, QColor(217, 217, 217));
    linearGradient.setColorAt(0.9, QColor(222, 222, 222));
    linearGradient.setColorAt(1.0, QColor(241, 241, 241));
    m_splitRectItem->setBrush(linearGradient);

    m_splitRectItem->setVisible(m_logoItem->isVisible());
}

void CScene::applyLayout(const std::shared_ptr<SceneLayoutSettings> &layout)
{
    if (layout)
        *m_sceneLayout = *layout;

    updateShowImage();
    updateTexItemsPosition();
    updateLogoPosition();
    updateSplitRect();
    updateMarginItems();

    const bool showOverlays = m_showItemFlags && !m_imageOnlyMode;
    updateOverlayItemsVisibility();
    emit noScenesChanged(showOverlays);
}

void CScene::setImageOnlyMode(bool enable)
{
    if (m_imageOnlyMode == enable)
        return;
    m_imageOnlyMode = enable;
    updateOverlayItemsVisibility();
    applyLayout(layoutSettings());
    update();
}

bool CScene::isImageOnlyMode() const
{
    return m_imageOnlyMode;
}

void CScene::updateOverlayItemsVisibility()
{
    const bool showOverlays = m_showItemFlags && !m_imageOnlyMode;

    const auto cameraIndex =
        static_cast<CameraIndex>(m_logoItem->data(static_cast<int>(GraphicsItemDataIndex::CameraIndex)).toInt());
    const bool logoVisible = showOverlays && (cameraIndex != CameraIndex::None);
    m_logoItem->setVisible(logoVisible);
    m_splitRectItem->setVisible(logoVisible);
    m_bottom->setVisible(showOverlays);

    for (auto &[key, it] : m_textItem)
    {
        Q_UNUSED(key);
        it->setVisible(showOverlays && !it->toPlainText().isEmpty());
    }

    const auto &[left, right, top, bottom] = m_sceneLayout->getMargin();
    Q_UNUSED(bottom);
    m_left->setVisible(showOverlays && left != 0);
    m_right->setVisible(showOverlays && right != 0);
    m_top->setVisible(showOverlays && top != 0);
}

QRectF CScene::effectiveSceneRect() const
{
    return (m_imageOnlyMode && m_showImageItem) ? m_showImageItem->sceneBoundingRect() : itemsBoundingRect();
}

void CScene::updateLayout()
{
    m_sceneLayout->update();
}

std::shared_ptr<SceneLayoutSettings> CScene::layoutSettings() const
{
    return m_sceneLayout;
}

void CScene::resetStatus()
{
    m_showItemFlags = true;
}

void CScene::updateShowImage()
{
    m_showImageItem->applyLayout(m_sceneLayout);
}

} // namespace CM
