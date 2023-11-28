#include "DisplayWidget.h"
#include "../Load/FileLoad.h"
#include "../Load/EXIFResolver.h"

#include <filesystem>
#include <mutex>

#include <QImage>
#include <QPixmap>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>

#include <QVBoxLayout>
#include <QResizeEvent>

namespace CM
{
    DisplayWidget::DisplayWidget(QWidget *parent)
    :QWidget(parent)
    , m_scene(new QGraphicsScene)
    , m_view (new QGraphicsView)
    , m_previewImage(new QGraphicsPixmapItem)
    {
        m_view->setScene(m_scene);
        connect(this,&DisplayWidget::Created,this, [ parent= this, view = m_view ]()
        {
            view->setParent(parent);
        });

        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        m_scene->addItem(m_previewImage);

        emit Created();
    }

    void DisplayWidget::Open(const std::filesystem::path& path) const
    {
        assert(this);
    }

    void DisplayWidget::paintEvent(QPaintEvent *event)
    {

        QWidget::paintEvent(event);
    }

    void DisplayWidget::PreViewImage(const std::filesystem::path & path) const
    {
        //m_scene->clear();

        const auto& pictureData = CM::FileLoad::Load(path.string());
        EXIFResolver resolver;
        EXIFResolver::check(resolver.resolver(pictureData));
        const auto& result = resolver.getInfos();

        ///TODO: 
        const QImage preImage = QImage::fromData(pictureData.data(),pictureData.size());
        auto preViewImage = QPixmap::fromImage(preImage);

        const auto& geometry = this->geometry();
        const auto previewSize = QSizeF{ static_cast<float>(geometry.width()) * 0.6f,static_cast<float>(geometry.height()) };

        const int imageW = static_cast<int>(previewSize.width());
        const int imageH = static_cast<int>(static_cast<float>(previewSize.height()) / static_cast<float>(previewSize.width()) * imageW);

        preViewImage = preViewImage.scaled(imageW, imageH,Qt::AspectRatioMode::KeepAspectRatio);
        m_previewImage->setPixmap(preViewImage);

        const auto imageY = previewSize.height() / 2 - preViewImage.height() / 2;
        constexpr auto imageX = 0;

        m_previewImage->setPos(imageX, imageY);

        /// show image infos
        QPoint itemPos(imageX + imageW + m_offsetX, m_offsetY + imageY);
        {
            const std::string camera_make = "Camera make: " + result.Make;
            auto camera_make_item = m_scene->addText(camera_make.c_str());
            camera_make_item->setPos(itemPos);
            itemPos.setY(camera_make_item->pos().y() + camera_make_item->boundingRect().height() + m_offsetY);
        }

        {
            const std::string camera_model = "Camera model: " + result.Model;
            const auto camera_model_item = m_scene->addText(camera_model.c_str());
            camera_model_item->setPos(itemPos);
            itemPos.setY(camera_model_item->pos().y() + camera_model_item->boundingRect().height() + m_offsetY);
        }

        {
            const std::string camera_model = "Exposure time: " + std::to_string(static_cast<int>(1.0 / result.ExposureTime));
            const auto camera_model_item = m_scene->addText(camera_model.c_str());
            camera_model_item->setPos(itemPos);
            itemPos.setY(camera_model_item->pos().y() + camera_model_item->boundingRect().height() + m_offsetY);
        }
        {
            const std::string camera_model = "F-stop: " + std::to_string(static_cast<int>(result.FNumber)) + "." + std::to_string(static_cast<int>(result.FNumber * 10) % 10) + "f";
            const auto camera_model_item = m_scene->addText(camera_model.c_str());
            camera_model_item->setPos(itemPos);
            itemPos.setY(camera_model_item->pos().y() + camera_model_item->boundingRect().height() + m_offsetY);
        }

        {
            const std::string camera_model = "ISO speed: " + std::to_string(result.ISOSpeedRatings);
            const auto camera_model_item = m_scene->addText(camera_model.c_str());
            camera_model_item->setPos(itemPos);
            itemPos.setY(camera_model_item->pos().y() + camera_model_item->boundingRect().height() + m_offsetY);
        }

        {
            const std::string camera_model = "Lens model: " + result.LensInfo.Model;
            const auto camera_model_item = m_scene->addText(camera_model.c_str());
            camera_model_item->setPos(itemPos);
            itemPos.setY(camera_model_item->pos().y() + camera_model_item->boundingRect().height() + m_offsetY);
        }
    }

    void DisplayWidget::resizeEvent(QResizeEvent *event)
    {
        const auto windowSize = event->size();
        m_view->setGeometry(QRect(0, 0, windowSize.width(), windowSize.height()));
        m_view->setSceneRect(QRect(0, 0, windowSize.width(), windowSize.height()));

        auto viewG= m_view->geometry();
        auto sceneG = m_scene->sceneRect();
        auto sceneGB = m_scene->itemsBoundingRect();

        auto sg = m_view->sceneRect();

        QWidget::resizeEvent(event);
    }
} // CM
