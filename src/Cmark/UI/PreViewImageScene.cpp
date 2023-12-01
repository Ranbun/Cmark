#include "PreViewImageScene.h"
#include "PreViewImageItem.h"
#include <QGraphicsView>

namespace CM
{
    static int textOffset = 5;

    PreViewImageScene::PreViewImageScene(QObject *parent)
    : QGraphicsScene(parent)
    , m_previewImageItem(new PreViewImageItem)
    , m_logoPixmapItem(new QGraphicsPixmapItem)
    {
        Init();
    }

    void PreViewImageScene::updateSceneRect(QGraphicsView *view, const QRectF &sceneSize)
    {
        auto func = [](){};   /// do nothing

        QRectF rect = view ? view->geometry() : this->sceneRect();
        view ? setSceneRect(0,0,rect.width() - 2,rect.height() - 2):func();

        ((PreViewImageItem*)m_previewImageItem)->update();
        updateTexItems();
        updateLogoPos();
    }

    void PreViewImageScene::Init()
    {
        InitPreviewImageItem();
        InitTexItems();
        InitLogoItem();
    }

    void PreViewImageScene::InitTexItems()
    {
        /// default show infos in image
        m_showInfos.emplace_back(showExifInfo{Exiflayout::left_top, {ExifKey::Camera_model}});
        m_showInfos.emplace_back(showExifInfo{Exiflayout::left_bottom, {ExifKey::Image_date}});
        m_showInfos.emplace_back(showExifInfo{Exiflayout::right_top, {ExifKey::Lens_Model}});
        m_showInfos.emplace_back(showExifInfo{Exiflayout::right_bottom, {ExifKey::F_stop,ExifKey::Exposure_time,ExifKey::ISO_speed}});   ///< TODO： 可以显示多条信息 需要做 || 处理

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

        {
            ImageInfoItem item;
            item.m_key = CM::ExifKey::Lens_Model;
            item.m_title = "Lens model: ";
            item.m_field = new QGraphicsTextItem;
            m_infos.emplace_back(item);

            item.m_field->setVisible(item.m_visible);
            addItem(item.m_field);
        }

        {
            ImageInfoItem item;
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
        auto pos = m_previewImageItem->pos();
        auto rect = m_previewImageItem->boundingRect();

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
                auto pixmapItem = it->m_field;
                pixmapItem->setPlainText((it->m_title + it->m_infos).c_str());
            }
        }

        const auto logoPos = m_logoPixmapItem->pos();
        const auto logoRect = m_logoPixmapItem->boundingRect();

        for(const auto & info: m_showInfos)
        {
            auto & [layout,key] = info;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = key[0]](CM::ImageInfoItem & info){
                return info.m_key == key;
            });

            if(it == m_infos.end())
            {
                std::runtime_error("No Key!");
            }

            switch (layout)
            {
                case Exiflayout::left_top:
                {
                    QPoint position(static_cast<int>(pos.x()), static_cast<int>(pos.y() + rect.height()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    auto pixmapItem = it->m_field;
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case Exiflayout::left_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(pos.x()), logoPos.y() + logoRect.height() - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case Exiflayout::right_top:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(logoPos.x()) + logoRect.width() + 40, static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setVisible(it->m_visible);
                    pixmapItem->setPos(position);
                }
                break;
                case Exiflayout::right_bottom:
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
        const auto pos = m_previewImageItem->pos();
        const auto rect = m_previewImageItem->boundingRect();

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

        const auto logoPos = m_logoPixmapItem->pos();
        const auto logoRect = m_logoPixmapItem->boundingRect();

        for(auto & info: m_showInfos)
        {
            auto & [layout,key] = info;
            auto it = std::find_if(m_infos.begin(), m_infos.end(),[key = key[0]](CM::ImageInfoItem & info){
                return info.m_key == key;
            });

            if(it == m_infos.end())
            {
                std::runtime_error("No Key!");
            }

            switch (layout)
            {
                case Exiflayout::left_top:
                {
                    QPoint position(static_cast<int>(pos.x()), static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    auto pixmapItem = it->m_field;
                    pixmapItem->setPos(position);
                }
                    break;
                case Exiflayout::left_bottom:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(pos.x()), logoPos.y() + logoRect.height() - itemRect.height());
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                    break;
                case Exiflayout::right_top:
                {
                    auto pixmapItem = it->m_field;
                    auto itemRect = pixmapItem->boundingRect();

                    QPoint position(static_cast<int>(logoPos.x()) + logoRect.width() + 40, static_cast<int>(logoPos.y()));
                    it->pos = CM::CPoint{position.x(), position.y(), 0};
                    pixmapItem->setPos(position);
                }
                    break;
                case Exiflayout::right_bottom:
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
        m_logoPixmapItem->setPixmap(pixmap);
        addItem(m_logoPixmapItem);
    }

    void PreViewImageScene::updateLogoPixmap(const QPixmap &logo)
    {
        m_logoPixmapItem->setPixmap(logo.scaled({64,64},Qt::KeepAspectRatio,Qt::SmoothTransformation));

        auto rect = m_previewImageItem->boundingRect();
        auto pos = m_previewImageItem->pos();
        m_logoPixmapItem->setPos(pos.x() + rect.width() / 2.0,pos.y() + rect.height() + 20);
    }

    void PreViewImageScene::updateLogoPos()
    {
        auto rect = m_previewImageItem->boundingRect();
        auto pos = m_previewImageItem->pos();
        m_logoPixmapItem->setPos(pos.x() + rect.width() / 2.0,pos.y() + rect.height() + 20);
    }

    void PreViewImageScene::InitPreviewImageItem()
    {
        auto pixmap = QPixmap();
        pixmap.fill(Qt::TransparentMode);
        m_previewImageItem->setPixmap(pixmap);
        addItem(m_previewImageItem);
    }

    void PreViewImageScene::updatePreviewPixmap(const QPixmap &pixmap)
    {
        ((PreViewImageItem*)(m_previewImageItem))->resetPixmap(pixmap);
    }
} // CM
