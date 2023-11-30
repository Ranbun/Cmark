#include "PreViewImageScene.h"
#include <QGraphicsView>

namespace CM
{

    static int textOffset = 5;

    PreViewImageScene::PreViewImageScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_logoPixmapItem(new QGraphicsPixmapItem)
    {
        Init();
    }

    void PreViewImageScene::updateSceneRect(QGraphicsView *view)
    {
        auto rect = view->geometry();
        setSceneRect(0, 0, rect.width() - 2, rect.height() - 2);
    }

    void PreViewImageScene::Init()
    {
        InitTexItems();
        InitLogoItem();
    }

    void PreViewImageScene::InitTexItems()
    {
        {
            ImageInfoItem item;
            item.m_key = CM::ExifKey::Camera_make;
            item.m_title = "Camera Make: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItem item;
            item.m_key = CM::ExifKey::Camera_model;
            item.m_title = "Camera Model: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItem item;
            item.m_key = CM::ExifKey::Exposure_time;
            item.m_title = "Exposure Time: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItem item;
            item.m_key = CM::ExifKey::F_stop;
            item.m_title = "F-Stop: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItem item;
            item.m_key = CM::ExifKey::ISO_speed;
            item.m_title = "ISO Speed: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }
    }

    void PreViewImageScene::updateTexItems(const ExifList &map)
    {
        auto pos = m_pixmapItem->pos();
        auto rect = m_pixmapItem->boundingRect();

        QPoint position(pos.x() + rect.width() + textOffset, textOffset + pos.y());

        for(const auto & item : map)
        {
            const auto & [key,text] = item;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = item.key](CM::ImageInfoItem & info){
                return info.m_key == key;
            });

            if(it != m_infos.end())
            {
                it->m_infos = text;
                it->m_visible = true;
                it->pos = CM::Point{position.x(),position.y(),0};

                auto pixmapItem = it->m_field;

                pixmapItem->setVisible(it->m_visible);
                pixmapItem->setPos(position);
                pixmapItem->setPlainText((it->m_title + it->m_infos).c_str());

                position.setY(static_cast<int>((pixmapItem->pos().y())) + (int)(pixmapItem->boundingRect().height()) + textOffset);
            }
        }
    }

    void PreViewImageScene::updateTexItems()
    {
        const auto pos = m_pixmapItem->pos();
        const auto rect = m_pixmapItem->boundingRect();

        QPoint position(pos.x() + rect.width() + textOffset, textOffset + pos.y());

        for(auto & item : m_infos)
        {
            auto pixmapItem = item.m_field;

            item.pos = Point{(position.x()),(position.y()),0};
            pixmapItem->setPos(position);

            position.setY(static_cast<int>((pixmapItem->pos().y())) + (int)(pixmapItem->boundingRect().height()) + textOffset);
        }
    }

    void PreViewImageScene::setPixmapItem(QGraphicsPixmapItem *item)
    {
        m_pixmapItem = item;
    }

    void PreViewImageScene::InitLogoItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::TransparentMode);
        m_logoPixmapItem->setPixmap(pixmap);
        addItem(m_logoPixmapItem);
    }

    void PreViewImageScene::updateLogoPixmap(const QPixmap &logo)
    {
        m_logoPixmapItem->setPixmap(logo.scaled({64,64},Qt::KeepAspectRatio,Qt::SmoothTransformation));

        auto rect = m_pixmapItem->boundingRect();
        auto pos = m_pixmapItem->pos();
        m_logoPixmapItem->setPos(pos.x() + rect.width() / 2.0,pos.y() + rect.height() + 20);
    }

    void PreViewImageScene::updateLogoPos()
    {
        auto rect = m_pixmapItem->boundingRect();
        auto pos = m_pixmapItem->pos();
        m_logoPixmapItem->setPos(pos.x() + rect.width() / 2.0,pos.y() + rect.height() + 20);
    }
} // CM
