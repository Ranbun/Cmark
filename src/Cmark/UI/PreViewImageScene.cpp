#include "PreViewImageScene.h"

namespace CM
{
    PreViewImageScene::PreViewImageScene(QObject *parent)
    : QGraphicsScene(parent)
    {
        Init();
    }

    void PreViewImageScene::updateSceneRect(const Size & rectSize)
    {
        setSceneRect(0, 0, rectSize.x, rectSize.y);
    }

    void PreViewImageScene::Init()
    {
        InitTexItems();
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

    void PreViewImageScene::updateTexItems(const ExifMap &map)
    {
        auto beginPos = CM::Point{0,0,0};

        int offset = 5;
        auto pos = m_pixmapItem->pos();
        auto rect = m_pixmapItem->boundingRect();

        QPoint position(pos.x() + rect.width() + offset, offset + pos.y());

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
                it->m_field->setVisible(it->m_visible);
                it->m_field->setPos(position);
                it->m_field->setPlainText(it->m_infos.c_str());

                position.setY(it->m_field->pos().y() + it->m_field->boundingRect().height() + offset);
            }
        }
    }

    void PreViewImageScene::setPixmapItem(QGraphicsPixmapItem *item)
    {
        m_pixmapItem = item;
    }
} // CM
