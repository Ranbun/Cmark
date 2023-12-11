#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <iostream>

#include <CMark.h>

namespace CM
{

    PreViewImageScene::PreViewImageScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_showImageItem(new PreViewImageItem(nullptr, m_sceneLayout))
    , m_logoItem(new QGraphicsPixmapItem)
    {
        Init();
    }

    void PreViewImageScene::updateSceneRect(QGraphicsView *view, const QRect &newSceneRect)
    {
        QRect rect = view ? view->geometry() : newSceneRect;
        auto createRect = [&rect](int offset)->QRect
        {
            return {0,0,rect.width() - offset,rect.height() - offset};
        };
        view ? setSceneRect(createRect(2)): setSceneRect(createRect(0));

        ((PreViewImageItem*)m_showImageItem)->update();
        auto imageTargetSize = m_showImageItem->boundingRect().size().toSize();

        if(!view)
        {
            m_sceneLayout.setImageSize({newSceneRect.width(),newSceneRect.height()});
        }
        else
        {
            m_sceneLayout.setImageSize({imageTargetSize.width(),imageTargetSize.height()});
        }

        m_sceneLayout.update();

        updateSplitRect();

        updateLogoPosition();

        updateTexItemsPosition();
        updateMarginItems();
    }

    void PreViewImageScene::Init()
    {
        InitMargin();
        InitTargetImageItem();
        InitTexItems();
        InitLogoItem();
        InitSplitRect();
    }

    void PreViewImageScene::InitTexItems()
    {
        /// default show infos in image
        m_showInfos.emplace_back(showExifInfo{showExifTexPositionIndex::left_top, {ExifKey::Camera_model}});
        m_showInfos.emplace_back(showExifInfo{showExifTexPositionIndex::left_bottom, {ExifKey::Image_date}});
        m_showInfos.emplace_back(showExifInfo{showExifTexPositionIndex::right_top, {ExifKey::Lens_Model}});
        m_showInfos.emplace_back(showExifInfo{showExifTexPositionIndex::right_bottom, {ExifKey::FocalLength,ExifKey::F_stop, ExifKey::Exposure_time, ExifKey::ISO_speed}});   ///< TODO： 可以显示多条信息 需要做 || 处理

        m_textItem.insert({showExifTexPositionIndex::left_top, addText("")});
        m_textItem.insert({showExifTexPositionIndex::left_bottom, addText("")});
        m_textItem.insert({showExifTexPositionIndex::right_top, addText("")});
        m_textItem.insert({showExifTexPositionIndex::right_bottom, addText("")});

        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPixelSize(13);

        for(auto & [key,item]: m_textItem)
        {
            switch (key)
            {
                case showExifTexPositionIndex::left_top:
                case showExifTexPositionIndex::right_top:
                    font.setBold(true);
                    item->setFont(font);
                    font.setBold(false);
                    break;
                default:
                    item->setFont(font);
                break;
            }

        }
    }

    void PreViewImageScene::updateTexItems(const ExifInfoMap &exifInfoMap)
    {
        m_targetImageExifInfoLists = exifInfoMap;  ///< copy infos

        auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoWithImageSpacing = m_sceneLayout.logoSpace();
        const auto & logoSize = m_sceneLayout.logoSize();
        auto imageRect = m_showImageItem->boundingRect().toRect();

        for(const auto & [layoutIndex, keys]: m_showInfos)
        {
            const auto item = m_textItem.at(layoutIndex);
            std::vector<std::string> exifInfos;
            std::for_each(keys.begin(),keys.end(),[&](const ExifKey & key)
            {
                exifInfos.emplace_back(m_targetImageExifInfoLists[key]);
            });

            auto res = std::accumulate(exifInfos.begin(), exifInfos.end(), std::string(), [](const std::string& a, const std::string& b){

                auto tail = b.empty() ? std::string() : std::string(" ") + b;
                return a + tail;
            });

            item->setPlainText(res.c_str());
            const auto & itemRect = item->boundingRect().toRect();

            switch (layoutIndex)
            {
                case showExifTexPositionIndex::left_top:
                {
                    QPoint position(left, top + imageRect.height() + logoWithImageSpacing);
                    item->setPos(position);
                }
                break;
                case showExifTexPositionIndex::left_bottom:
                {
                    QPoint position(left, top + logoWithImageSpacing + imageRect.height() + logoSize.h - itemRect.height());
                    item->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::right_top:
                {
                    QPoint position(left + imageRect.width() - itemRect.width(),top + imageRect.height() + logoWithImageSpacing);
                    item->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::right_bottom:
                {
                    QPoint position(left + imageRect.width() - itemRect.width(),top + logoWithImageSpacing + imageRect.height() + logoSize.h - itemRect.height());
                    item->setPos(position);
                }
                break;
            }
        }

        /// TODO: may be update it
        auto rightTopTextRect = m_textItem.at(showExifTexPositionIndex::right_top)->boundingRect();
        auto rightBottomTextRect = m_textItem.at(showExifTexPositionIndex::right_top)->boundingRect();
        auto maxW = rightTopTextRect.width() > rightBottomTextRect.width() ? rightTopTextRect.width() : rightBottomTextRect.width();
        m_sceneLayout.setRightMaxWidth(maxW);
    }

    void PreViewImageScene::updateTexItemsPosition()
    {
        auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoWithImageSpacing = m_sceneLayout.logoSpace();
        const auto & logoSize = m_sceneLayout.logoSize();
        auto imageRect = m_showImageItem->boundingRect().toRect();

        for(const auto & [layoutIndex, keys]: m_showInfos)
        {
            const auto item = m_textItem.at(layoutIndex);
            const auto & itemRect = item->boundingRect().toRect();

            switch (layoutIndex)
            {
                case showExifTexPositionIndex::left_top:
                {
                    QPoint position(left, top + imageRect.height() + logoWithImageSpacing);
                    item->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::left_bottom:
                {
                    QPoint position(left, top + logoWithImageSpacing + imageRect.height() + logoSize.h - itemRect.height());
                    item->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::right_top:
                {
                    QPoint position(left + imageRect.width() + right - m_sceneLayout.rightMaxWidth() - m_sceneLayout.rightTextOffset(),top + imageRect.height() + logoWithImageSpacing);
                    item->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::right_bottom:
                {
                    QPoint position(left + imageRect.width() + right - m_sceneLayout.rightMaxWidth() - m_sceneLayout.rightTextOffset(),top + logoWithImageSpacing + imageRect.height() + logoSize.h - itemRect.height());
                    item->setPos(position);
                }
                break;
            }
        }
    }

    void PreViewImageScene::InitLogoItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::TransparentMode);
        m_logoItem->setPixmap(pixmap);
        addItem(m_logoItem);
    }

    void PreViewImageScene::resetLogoPixmap(const QPixmap &logo)
    {
        const auto & logoSize = m_sceneLayout.logoSize();
        m_logoItem->setPixmap(logo.scaled({logoSize.w, logoSize.h}, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        auto rect = m_showImageItem->boundingRect();
        auto pos = m_showImageItem->pos();
        m_logoItem->setPos(pos.x() + rect.width() / 2.0, pos.y() + rect.height() + 20);
    }

    void PreViewImageScene::updateLogoPosition()
    {
        auto rect = m_showImageItem->boundingRect();
        auto pos = m_showImageItem->pos();
        auto logoSpaceWithImage = m_sceneLayout.logoSpace();

        auto imageSize = m_sceneLayout.previewImageSize();

        const auto & [l,r,t,b] = m_sceneLayout.getMargin();

        auto x = l + r + imageSize.w - m_sceneLayout.logoSplitLineSpace() * 2.0 - m_sceneLayout.logoSize().w - m_sceneLayout.rightMaxWidth() - m_sceneLayout.rightTextOffset() - m_sceneLayout.splitRectWidth();

        /// TODO： calc logo position
        m_logoItem->setPos(x, pos.y() + rect.height() + logoSpaceWithImage);
    }

    void PreViewImageScene::InitTargetImageItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::transparent);
        m_showImageItem->setPixmap(pixmap);
        addItem(m_showImageItem);
    }

    void PreViewImageScene::resetPreviewImageTarget(const QPixmap &pixmap)
    {
        ((PreViewImageItem*)(m_showImageItem))->resetPixmap(pixmap);
        m_sceneLayout.setImageSize({pixmap.width(),pixmap.height()});
    }

    void PreViewImageScene::InitMargin()
    {
        auto rect = sceneRect().toRect();
        const auto & [left,right,top,bottom] = m_sceneLayout.getMargin();

        m_left = new QGraphicsRectItem(0,0,left,rect.width());
        m_right = new QGraphicsRectItem(0,0,right,rect.height());
        m_top = new QGraphicsRectItem(0,0,rect.width(),top);
        m_bottom = new QGraphicsRectItem(0,0,m_sceneLayout.getMargin().left,rect.width());

        const auto & rectPen = QPen(Qt::transparent);
        m_left->setPen(rectPen);
        m_right->setPen(rectPen);
        m_top->setPen(rectPen);
        m_bottom->setPen(rectPen);

        addItem(m_left);
        addItem(m_right);
        addItem(m_bottom);
        addItem(m_top);
    }

    void PreViewImageScene::updateMarginItems()
    {
        const auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoSpaceWithShowImage = m_sceneLayout.logoSpace();
        const auto & imageRect = m_showImageItem->sceneBoundingRect().toRect();
        const auto & logoRect = m_logoItem->sceneBoundingRect().toRect();

        auto sceneBoundMarginRectH = top + imageRect.height() + logoSpaceWithShowImage + logoRect.height() + bottom;
        auto sceneBoundMarginRectW = left + imageRect.width() + right;

        m_left->setRect(0, 0, left, sceneBoundMarginRectH);
        m_right->setRect(left + imageRect.width(), 0, right, sceneBoundMarginRectH);
        m_top->setRect(0,0,sceneBoundMarginRectW,top);

        m_bottom->setRect(0,top + imageRect.height() + logoSpaceWithShowImage + logoRect.height(),sceneBoundMarginRectW,bottom);
    }

    const QPixmap &PreViewImageScene::originalImageTarget()
    {
        return ((PreViewImageItem*)(m_showImageItem))->target();
    }

    void PreViewImageScene::InitSplitRect()
    {
        m_splitRectItem = new QGraphicsRectItem();

        QPen pen;
        pen.setWidth(1);
        pen.setColor(Qt::transparent);

        QBrush brush(Qt::SolidPattern);
        brush.setColor(QColor(219,219,219));

        QRadialGradient radialGradient(100, 100, 100, 0, 100);
        radialGradient.setColorAt(0, QColor(253,253,253));
        radialGradient.setColorAt(0.1, QColor(253,253,253));
        radialGradient.setColorAt(0.2, QColor(216,216,216));
        radialGradient.setColorAt(0.5, QColor(218,218,218));
        radialGradient.setColorAt(0.6, QColor(217,217,217));
        radialGradient.setColorAt(0.9, QColor(222,222,222));
        radialGradient.setColorAt(1.0, QColor(241,241,241));

        m_splitRectItem->setBrush(radialGradient);
        m_splitRectItem->setPen(Qt::NoPen);

        addItem(m_splitRectItem);
    }

    void PreViewImageScene::updateSplitRect()
    {
        const auto & logoSize = m_sceneLayout.logoSize();
        const auto & logoPosition= m_logoItem->pos();
        auto imageH = m_sceneLayout.previewImageSize().h;
        auto imageW = m_sceneLayout.previewImageSize().w;
        auto spacing = m_sceneLayout.logoSpace();

        auto logoRect = m_logoItem->boundingRect().toRect();

        const auto & [left,right,top,bottom] = m_sceneLayout.getMargin();

        auto splitRectW = m_sceneLayout.splitRectWidth();
        auto x = left + right + imageW - m_sceneLayout.rightTextOffset() - m_sceneLayout.rightMaxWidth()  - m_sceneLayout.logoSplitLineSpace() - splitRectW;

        auto y = top + imageH + spacing;

        m_splitRectItem->setRect(x, y, splitRectW, logoRect.height());

        auto r = m_splitRectItem->rect();

        QLinearGradient radialGradient(r.left(),r.top(),r.right(),r.top());
        radialGradient.setColorAt(0, QColor(253,253,253));
        radialGradient.setColorAt(0.2, QColor(216,216,216));
        radialGradient.setColorAt(0.5, QColor(218,218,218));
        radialGradient.setColorAt(0.6, QColor(217,217,217));
        radialGradient.setColorAt(0.9, QColor(222,222,222));
        radialGradient.setColorAt(1.0, QColor(241,241,241));

        m_splitRectItem->setBrush(radialGradient);
    }
} // CM
