#include "CMark.h"

#include "CScene.h"
#include "PreViewImageItem.h"

#include <QGraphicsView>
#include <QFileDialog>

namespace CM
{
    CScene::CScene(QObject* parent)
        : QGraphicsScene(parent)
          , m_LogoItem(new QGraphicsPixmapItem)
          , m_SceneLayout(std::make_shared<SceneLayoutSettings>())
    {
        m_ShowImageItem = new PreViewImageItem(nullptr, m_SceneLayout);


        Init();
    }

    CScene::~CScene() = default;

    void CScene::Init()
    {
        InitMargin();
        InitTargetImageItem();
        InitTexItems();
        InitLogoItem();
        InitSplitRect();
    }

    void CScene::InitTexItems()
    {
        /// default show infos in image
        m_ShowInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::LeftTop, {ExifKey::Camera_model}});
        m_ShowInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::LeftBottom, {ExifKey::Image_date}});
        m_ShowInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::RightTop, {ExifKey::Lens_Model}});
        m_ShowInfos.emplace_back(ShowExifInfo{
            ShowExifTexPositionIndex::RightBottom,
            {ExifKey::FocalLength, ExifKey::F_stop, ExifKey::Exposure_time, ExifKey::ISO_speed}
        });

        m_TextItem.insert({ShowExifTexPositionIndex::LeftTop, addText("")});
        m_TextItem.insert({ShowExifTexPositionIndex::LeftBottom, addText("")});
        m_TextItem.insert({ShowExifTexPositionIndex::RightTop, addText("")});
        m_TextItem.insert({ShowExifTexPositionIndex::RightBottom, addText("")});

        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(11);
        font.setPixelSize(13);

        for (auto& [key,item] : m_TextItem)
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
            default:
                item->setFont(font);
                break;
            }
        }
    }

    void CScene::resetTexItemsPlainText(const ExifInfoMap& exifInfoMap)
    {
        m_TargetImageExifInfoLists = exifInfoMap; ///< copy infos

        for (const auto& [layoutIndex, keys] : m_ShowInfos)
        {
            const auto item = m_TextItem.at(layoutIndex);
            std::vector<std::string> exifInfos;
            std::for_each(keys.begin(), keys.end(), [&](const ExifKey& key)
            {
                exifInfos.emplace_back(m_TargetImageExifInfoLists[key]);
            });

            auto res = std::accumulate(exifInfos.begin(), exifInfos.end(), std::string(),
                                       [](const std::string& a, const std::string& b)
                                       {
                                           auto tail = b.empty() ? std::string() : std::string(" ") + b;
                                           return a + tail;
                                       });

            item->setPlainText(res.c_str());
        }

        /// TODO: may be applyLayout it
        const auto rightTopTextRect = m_TextItem.at(ShowExifTexPositionIndex::RightTop)->boundingRect();
        const auto rightBottomTextRect = m_TextItem.at(ShowExifTexPositionIndex::RightBottom)->boundingRect();
        const auto maxW = rightTopTextRect.width() > rightBottomTextRect.width()
                              ? rightTopTextRect.width()
                              : rightBottomTextRect.width();
        m_SceneLayout->setRightMaxWidth(static_cast<int>(maxW));

        updateTexItemsPosition();
    }

    void CScene::updateTexItemsPosition()
    {
        auto& [left,right,top,bottom] = m_SceneLayout->getMargin();
        const auto logoWithImageSpacing = m_SceneLayout->logoWithImageSpace();
        const auto& logoSize = m_SceneLayout->LogoSize();
        const auto& imageSize = m_SceneLayout->ImageSize();

        for (const auto& [layoutIndex, keys] : m_ShowInfos)
        {
            const auto item = m_TextItem.at(layoutIndex);
            const auto& itemRect = item->boundingRect().toRect();

            switch (layoutIndex)
            {
            case ShowExifTexPositionIndex::LeftTop:
                {
                    QPoint position(m_SceneLayout->leftTextOffset(), top + imageSize.h + logoWithImageSpacing);
                    item->setPos(position);
                }
                break;
            case ShowExifTexPositionIndex::LeftBottom:
                {
                    QPoint position(m_SceneLayout->leftTextOffset(),
                                    top + logoWithImageSpacing + imageSize.h + logoSize.h - itemRect.height());
                    item->setPos(position);
                }
                break;
            case ShowExifTexPositionIndex::RightTop:
                {
                    QPoint position(
                        left + imageSize.w + right - m_SceneLayout->rightTextMaxWidth() - m_SceneLayout->
                        rightTextOffset(), top + imageSize.h + logoWithImageSpacing);
                    item->setPos(position);
                }
                break;
            case ShowExifTexPositionIndex::RightBottom:
                {
                    QPoint position(
                        left + imageSize.w + right - m_SceneLayout->rightTextMaxWidth() - m_SceneLayout->
                        rightTextOffset(), top + logoWithImageSpacing + imageSize.h + logoSize.h - itemRect.height());
                    item->setPos(position);
                }
                break;
            }
        }
    }

    void CScene::InitLogoItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::TransparentMode);
        m_LogoItem->setPixmap(pixmap);
        addItem(m_LogoItem);
    }

    void CScene::resetLogoPixmap(const std::shared_ptr<QPixmap>& logo, CameraIndex cameraIndex)
    {
        const auto& [logoW, logoH] = m_SceneLayout->LogoSize();

        const auto h = static_cast<float>(logoH);
        const auto ratio = static_cast<float>(logo->height()) / static_cast<float>(logo->width());
        auto newW = static_cast<int>(std::round(h / ratio));

        m_SceneLayout->setLogoSize(newW, logoH);
        m_LogoItem->setPixmap(logo->scaled({newW, logoH}, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_LogoItem->setData(static_cast<int>(GraphicsItemDataIndex::CameraIndex), static_cast<int>(cameraIndex));

        updateLogoPosition();
    }

    void CScene::updateLogoPosition()
    {
        const auto logoSpaceWithImage = m_SceneLayout->logoWithImageSpace();
        const auto [imageW, imageH] = m_SceneLayout->ImageSize();
        const auto& [l,r,t,b] = m_SceneLayout->getMargin();

        const auto x = l + r + imageW - m_SceneLayout->logoWithSplitLineSpace() * 2.0 - m_SceneLayout->LogoSize().w -
            m_SceneLayout->rightTextMaxWidth() - m_SceneLayout->rightTextOffset() - m_SceneLayout->splitRectWidth();
        const auto y = t + imageH + logoSpaceWithImage;
        m_LogoItem->setPos(x, y);
    }

    void CScene::InitTargetImageItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::transparent);
        m_ShowImageItem->setPixmap(pixmap);
        addItem(m_ShowImageItem);
    }

    void CScene::resetPreviewImageTarget(const QPixmap& pixmap, size_t imageIndexCode) const
    {
        m_ShowImageItem->resetPixmap(pixmap);
        m_SceneLayout->setImageSize({pixmap.width(), pixmap.height()});
        m_ShowImageItem->setData(static_cast<int>(GraphicsItemDataIndex::PixmapIndex), QVariant(imageIndexCode));
    }

    void CScene::InitMargin()
    {
        const auto rect = sceneRect().toRect();
        const auto& [left,right,top,bottom] = m_SceneLayout->getMargin();

        m_Left = new QGraphicsRectItem(0, 0, left, rect.width());
        m_Right = new QGraphicsRectItem(0, 0, right, rect.height());
        m_Top = new QGraphicsRectItem(0, 0, rect.width(), top);
        m_Bottom = new QGraphicsRectItem(0, 0, m_SceneLayout->getMargin().left, rect.width());

        const auto& rectPen = QPen(Qt::transparent);
        m_Left->setPen(rectPen);
        m_Right->setPen(rectPen);
        m_Top->setPen(rectPen);
        m_Bottom->setPen(rectPen);

        addItem(m_Left);
        addItem(m_Right);
        addItem(m_Bottom);
        addItem(m_Top);
    }

    void CScene::updateMarginItems()
    {
        const auto& [left,right,top,bottom] = m_SceneLayout->getMargin();
        const auto logoSpaceWithShowImage = m_SceneLayout->logoWithImageSpace();
        const auto& imageRect = m_SceneLayout->ImageSize();
        const auto& logoRect = m_SceneLayout->LogoSize();

        const auto sceneBoundMarginRectH = top + imageRect.h + logoSpaceWithShowImage + logoRect.h + bottom;
        const auto sceneBoundMarginRectW = left + imageRect.w + right;

        m_Left->setRect(0, 0, left, sceneBoundMarginRectH);
        m_Right->setRect(left + imageRect.w, 0, right, sceneBoundMarginRectH);
        m_Top->setRect(0, 0, sceneBoundMarginRectW, top);

        m_Bottom->setRect(0, top + imageRect.h + logoSpaceWithShowImage + logoRect.h, sceneBoundMarginRectW, bottom);
    }

    void CScene::InitSplitRect()
    {
        m_SplitRectItem = new QGraphicsRectItem;
        QPen pen;
        pen.setWidth(1);
        pen.setColor(Qt::transparent);

        QBrush brush(Qt::SolidPattern);
        brush.setColor(QColor(219, 219, 219));

        QRadialGradient radialGradient(100, 100, 100, 0, 100);
        radialGradient.setColorAt(0, QColor(253, 253, 253));
        radialGradient.setColorAt(0.1, QColor(253, 253, 253));
        radialGradient.setColorAt(0.2, QColor(216, 216, 216));
        radialGradient.setColorAt(0.5, QColor(218, 218, 218));
        radialGradient.setColorAt(0.6, QColor(217, 217, 217));
        radialGradient.setColorAt(0.9, QColor(222, 222, 222));
        radialGradient.setColorAt(1.0, QColor(241, 241, 241));

        m_SplitRectItem->setBrush(radialGradient);
        m_SplitRectItem->setPen(Qt::NoPen);

        addItem(m_SplitRectItem);
    }

    void CScene::updateSplitRect()
    {
        const auto& logoSize = m_SceneLayout->LogoSize();
        const auto imageH = m_SceneLayout->ImageSize().h;
        const auto imageW = m_SceneLayout->ImageSize().w;
        const auto spacing = m_SceneLayout->logoWithImageSpace();
        const auto& [left,right,top,bottom] = m_SceneLayout->getMargin();

        const auto splitRectW = m_SceneLayout->splitRectWidth();

        const auto x = left + right + imageW - m_SceneLayout->rightTextOffset() - m_SceneLayout->rightTextMaxWidth() -
            m_SceneLayout->logoWithSplitLineSpace() - splitRectW;
        const auto y = top + imageH + spacing;

        m_SplitRectItem->setRect(x, y, splitRectW, logoSize.h);

        const auto r = m_SplitRectItem->rect();

        QLinearGradient radialGradient(r.left(), r.top(), r.right(), r.top());
        radialGradient.setColorAt(0, QColor(253, 253, 253));
        radialGradient.setColorAt(0.2, QColor(216, 216, 216));
        radialGradient.setColorAt(0.5, QColor(218, 218, 218));
        radialGradient.setColorAt(0.6, QColor(217, 217, 217));
        radialGradient.setColorAt(0.9, QColor(222, 222, 222));
        radialGradient.setColorAt(1.0, QColor(241, 241, 241));
        m_SplitRectItem->setBrush(radialGradient);
    }

    void CScene::applyLayout(const std::shared_ptr<SceneLayoutSettings>& layout)
    {
        if (layout)
        {
            *m_SceneLayout = *layout;
        }

        updateShowImage();
        updateTexItemsPosition();
        updateLogoPosition();
        updateSplitRect();
        updateMarginItems();
    }

    void CScene::updateLayout()
    {
        m_SceneLayout->update();
    }

    std::shared_ptr<SceneLayoutSettings> CScene::layoutSettings() const
    {
        return m_SceneLayout;
    }

    void CScene::updateShowImage()
    {
        m_ShowImageItem->applyLayout();
    }
} // CM
