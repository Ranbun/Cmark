#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include "SceneLayout.h"

#include <QGraphicsView>
#include <QFileDialog>
#include <iostream>

namespace CM
{
    static int textOffset = 5;

    PreViewImageScene::PreViewImageScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_showImageItem(new PreViewImageItem)
    , m_logoItem(new QGraphicsPixmapItem)
    {
        Init();
    }

    void PreViewImageScene::updateSceneRect(QGraphicsView *view, const QRectF &sceneSize)
    {
        auto func = [](){};   /// do nothing

        QRectF rect = view ? view->geometry() : this->sceneRect();
        view ? setSceneRect(0,0,rect.width() - 2,rect.height() - 2):func();

        ((PreViewImageItem*)m_showImageItem)->update();
        updateTexItems();
        updateLogoPos();
    }

    void PreViewImageScene::Init()
    {
        InitMargin();
        InitPreviewImageItem();
        InitTexItems();
        InitLogoItem();
    }

    void PreViewImageScene::InitTexItems()
    {
        /// default show infos in image
        m_showInfos.emplace_back(showExifInfo{ExifLayout::left_top, {ExifKey::Camera_model}});
        m_showInfos.emplace_back(showExifInfo{ExifLayout::left_bottom, {ExifKey::Image_date}});
        m_showInfos.emplace_back(showExifInfo{ExifLayout::right_top, {ExifKey::Lens_Model}});
        m_showInfos.emplace_back(showExifInfo{ExifLayout::right_bottom, {ExifKey::F_stop, ExifKey::Exposure_time, ExifKey::ISO_speed}});   ///< TODO： 可以显示多条信息 需要做 || 处理

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::Camera_make;
            item.m_title = "Camera Make: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::Camera_model;
            item.m_title = "Camera Model: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::Exposure_time;
            item.m_title = "Exposure Time: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::F_stop;
            item.m_title = "F-Stop: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::ISO_speed;
            item.m_title = "ISO Speed: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::Lens_Model;
            item.m_title = "Lens model: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItemPack item;
            item.m_key = CM::ExifKey::Image_date;
            item.m_title = "Image data: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }


    }

    void PreViewImageScene::updateTexItems(const ExifList &map)
    {
        auto pos = m_showImageItem->pos();
        auto rect = m_showImageItem->boundingRect();

        for(const auto & item : map)
        {
            const auto & [key,text] = item;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = item.key](CM::ImageInfoItemPack & info){
                return info.m_key == key;
            });

            if(it != m_infos.end())
            {
                it->m_infos = text;
                it->m_visible = true;
                auto pixmapItem = it->m_field;
                pixmapItem->setPlainText((it->m_title + it->m_infos).c_str());
            }
        }

        const auto logoPos = m_logoItem->pos();
        const auto logoRect = m_logoItem->boundingRect();

        for(const auto & info: m_showInfos)
        {
            auto & [layout,key] = info;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = key[0]](CM::ImageInfoItemPack & info){
                return info.m_key == key;
            });

            if(it == m_infos.end())
            {
                std::runtime_error("No Key!");
            }

            switch (layout)
            {
                case ExifLayout::left_top:
                {
                    QPoint position(static_cast<int>(pos.x()), static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    auto pixmapItem = it->m_field;
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case ExifLayout::left_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(pos.x()), logoPos.y() + logoRect.height() - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case ExifLayout::right_top:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(logoPos.x()) + logoRect.width() + 40, static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case ExifLayout::right_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(logoPos.x()) + logoRect.width() + 40, logoPos.y() + logoRect.height() - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                default:
                    break;
            }
        }
    }

    void PreViewImageScene::updateTexItems()
    {
        const auto pos = m_showImageItem->pos();
        const auto rect = m_showImageItem->boundingRect();

#if  0
        QPoint position(pos.x() + rect.width() + textOffset, textOffset + pos.y());

        for(auto & item : m_infos)
        {
            auto pixmapItem = item.m_field;

            item.pos = CPoint{(position.x()), (position.y()), 0};
            pixmapItem->setPos(position);

            position.setY(static_cast<int>((pixmapItem->pos().y())) + (int)(pixmapItem->boundingRect().height()) + textOffset);
        }
#endif

        const auto logoPos = m_logoItem->pos();
        const auto logoRect = m_logoItem->boundingRect();

        for(auto & info: m_showInfos)
        {
            auto & [layout,key] = info;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = key[0]](CM::ImageInfoItemPack & info){
                return info.m_key == key;
            });

            if(it == m_infos.end())
            {
                std::runtime_error("No Key!");
            }

            switch (layout)
            {
                case ExifLayout::left_top:
                {
                    QPoint position(static_cast<int>(pos.x()), static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    auto pixmapItem = it->m_field;
                    pixmapItem->setPos(position);
                }
                    break;
                case ExifLayout::left_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(pos.x()), logoPos.y() + logoRect.height() - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                    break;
                case ExifLayout::right_top:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(logoPos.x()) + logoRect.width() + 40, static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                    break;
                case ExifLayout::right_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(logoPos.x()) + logoRect.width() + 40, logoPos.y() + logoRect.height() - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                break;
                default:
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

    void PreViewImageScene::updateLogoPixmap(const QPixmap &logo)
    {
        m_logoItem->setPixmap(logo.scaled({64, 64}, Qt::KeepAspectRatio, Qt::SmoothTransformation));

        auto rect = m_showImageItem->boundingRect();
        auto pos = m_showImageItem->pos();
        m_logoItem->setPos(pos.x() + rect.width() / 2.0, pos.y() + rect.height() + 20);
    }

    void PreViewImageScene::updateLogoPos()
    {
        auto rect = m_showImageItem->boundingRect();
        auto pos = m_showImageItem->pos();
        m_logoItem->setPos(pos.x() + rect.width() / 2.0, pos.y() + rect.height() + 20);
    }

    void PreViewImageScene::InitPreviewImageItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::TransparentMode);
        m_showImageItem->setPixmap(pixmap);
        addItem(m_showImageItem);
    }

    void PreViewImageScene::updatePreviewPixmap(const QPixmap &pixmap)
    {
        ((PreViewImageItem*)(m_showImageItem))->resetPixmap(pixmap);
    }

    void PreViewImageScene::saveLoadedPixmap()
    {
        auto fileName = QFileDialog::getSaveFileName(nullptr,tr("Save File"),
                                                     "./untitled.png",
                                                     tr("Images (*.png *.xpm *.jpg)"));

        auto res = m_showImageItem->pixmap().save(fileName);
        std::cout <<"Save res:" << res<<std::endl;
    }

    void PreViewImageScene::InitMargin()
    {
        auto rect = sceneRect().toRect();

        m_left = new QGraphicsRectItem(0,0,m_sceneLayout.getMargin().left,rect.width());
        m_right = new QGraphicsRectItem(0,0,m_sceneLayout.getMargin().left,rect.width());
        m_top = new QGraphicsRectItem(0,0,m_sceneLayout.getMargin().left,rect.width());
        m_bottom = new QGraphicsRectItem(0,0,m_sceneLayout.getMargin().left,rect.width());

        addItem(m_left);

    }
} // CM
