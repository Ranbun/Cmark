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
        m_showInfos.emplace_back(showExifInfo{showExifTexPositionIndex::right_bottom, {ExifKey::F_stop, ExifKey::Exposure_time, ExifKey::ISO_speed}});   ///< TODO： 可以显示多条信息 需要做 || 处理

        m_textItem.insert({showExifTexPositionIndex::left_top, addText("")});
        m_textItem.insert({showExifTexPositionIndex::left_bottom, addText("")});
        m_textItem.insert({showExifTexPositionIndex::right_top, addText("")});
        m_textItem.insert({showExifTexPositionIndex::right_bottom, addText("")});

#if  0
        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::Camera_make;
            item.m_title = "Camera Make: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::Camera_model;
            item.m_title = "Camera Model: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::Exposure_time;
            item.m_title = "Exposure Time: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::F_stop;
            item.m_title = "F-Stop: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::ISO_speed;
            item.m_title = "ISO Speed: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::Lens_Model;
            item.m_title = "Lens model: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ExifInfoItemPack item;
            item.m_key = CM::ExifKey::Image_date;
            item.m_title = "Image data: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }
#endif

    }

    void PreViewImageScene::updateTexItems(const ExifInfoMap &exifInfoMap)
    {
#if 0
        for(const auto & item : map)
        {
            const auto & [key,text] = item;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = item.key](CM::ExifInfoItemPack & info){
                return info.m_key == key;
            });

            if(it != m_infos.end())
            {
                it->m_infos = text;
                it->m_visible = false;
                auto pixmapItem = it->m_field;
                pixmapItem->setPlainText((it->m_infos).c_str());
            }
        }

        auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoWithImageSpacing = m_sceneLayout.logoSpace();
        const auto & logoSize = m_sceneLayout.LogoSize();

        auto imageRect = m_showImageItem->boundingRect().toRect();

        for(const auto & info: m_showInfos)
        {
            auto & [layout,key] = info;

            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = key[0]](CM::ExifInfoItemPack & info){
                return info.m_key == key;
            });

            if(it == m_infos.end())
            {
                throw std::runtime_error("No Key!");
            }

            it->m_visible = true;

            switch (layout)
            {
                case showExifTexPositionIndex::left_top:
                {
                    QPoint position(left, top + imageRect.height() + logoWithImageSpacing);
                    it->pos = CM::CPoint{position.x(), position.y(), 0};

                    auto pixmapItem = it->m_field;

                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case showExifTexPositionIndex::left_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect().toRect();

                    QPoint position(left, top + logoWithImageSpacing + logoSize.h - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case showExifTexPositionIndex::right_top:
                {
                    auto Item = it->m_field;
                    const auto & itemRect = Item->boundingRect().toRect();

                    QPoint position(left + right + imageRect.width() - itemRect.width(),top + imageRect.height() + logoWithImageSpacing);
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    Item->setVisible(it->m_visible);
                    Item->setPos(position);
                }
                break;
                case showExifTexPositionIndex::right_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect().toRect();

                    QPoint position(left + right + imageRect.width() - itemRect.width(),top + logoWithImageSpacing + logoSize.h - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                default:
                    break;
            }
        }

#endif

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
#if  0
        auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto logoWithImageSpacing = m_sceneLayout.logoSpace();
        const auto & logoSize = m_sceneLayout.LogoSize();

        auto imageRect = m_showImageItem->boundingRect().toRect();

        for(auto & info: m_showInfos)
        {
            auto & [layout,key] = info;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = key[0]](CM::ExifInfoItemPack & info){
                return info.m_key == key;
            });

            if(it == m_infos.end())
            {
                throw std::runtime_error("No Key!");
            }

            switch (layout)
            {
                case showExifTexPositionIndex::left_top:
                {
                    QPoint position(left, top + imageRect.height() + logoWithImageSpacing);
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    auto pixmapItem = it->m_field;
                    pixmapItem->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::left_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect().toRect();
                    QPoint position(left, top + logoWithImageSpacing + imageRect.height() + logoSize.h - itemRect.height());

                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::right_top:
                {
                    auto Item = it->m_field;
                    const auto & itemRect = Item->boundingRect().toRect();
                    QPoint position(left + imageRect.width() - itemRect.width(),top + imageRect.height() + logoWithImageSpacing);

                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    Item->setPos(position);
                }
                    break;
                case showExifTexPositionIndex::right_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect().toRect();
                    QPoint position(left + imageRect.width() - itemRect.width(),top + logoWithImageSpacing + imageRect.height() + logoSize.h - itemRect.height());

                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                break;
                default:
                    break;
            }
        }
#endif
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
