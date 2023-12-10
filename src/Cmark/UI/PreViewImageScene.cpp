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
    }

    void PreViewImageScene::updateTexItems(const ExifInfoMap &exifInfoMap)
    {
        m_targetImageExifInfoLists = exifInfoMap;  ///< copy infos

        auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoWithImageSpacing = m_sceneLayout.logoSpace();
        const auto & logoSize = m_sceneLayout.LogoSize();
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
    }

    void PreViewImageScene::updateTexItemsPosition()
    {
        auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoWithImageSpacing = m_sceneLayout.logoSpace();
        const auto & logoSize = m_sceneLayout.LogoSize();
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

        const auto rightBottomItem = m_textItem.at(showExifTexPositionIndex::right_bottom);
        const auto rightTopItem = m_textItem.at(showExifTexPositionIndex::right_top);

        const auto closestLeft = rightBottomItem->pos().x() < rightTopItem->pos().x() ? rightBottomItem->pos().x():rightTopItem->pos().x();
        rightBottomItem->setX(closestLeft);
        rightTopItem->setX(closestLeft);
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
        const auto & logoSize = m_sceneLayout.LogoSize();
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

        /// TODO： calc logo position
        m_logoItem->setPos(pos.x() + rect.width() / 2.0, pos.y() + rect.height() + logoSpaceWithImage);
    }

    void PreViewImageScene::InitTargetImageItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::TransparentMode);
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
} // CM
