#include "DisplayWidget.h"

#include <filesystem>
#include <mutex>

#include <QPixmap>

#include <QGraphicsScene>
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
    {
        m_view->setScene(m_scene);
        connect(this,&DisplayWidget::Created,this, [ parent= this, view = m_view ]()
        {
            view->setParent(parent);
        });
        emit Created();
    }

    void DisplayWidget::Open(const std::filesystem::path& path)
    {

    }

    void DisplayWidget::paintEvent(QPaintEvent *event)
    {

        QWidget::paintEvent(event);
    }

    void DisplayWidget::AddImage(const std::filesystem::path & path) const
    {
        const auto & geometry = this->geometry();


        auto p = QString::fromStdString(path.string());
        QPixmap preViewImage(p);

        const auto imageW = geometry.width()/ 2.0;

        preViewImage = preViewImage.scaled(imageW,300);
        const auto item = m_scene->addPixmap(preViewImage);


        const auto imageY = geometry.height() / 2 - preViewImage.height() / 2;
        const auto imageX = 0;

        item->setPos(imageX, imageY);

        item->setAcceptDrops(true);

        // 将视图添加到布局中
    }

    void DisplayWidget::resizeEvent(QResizeEvent *event)
    {
        const auto windowSize = event->size();
        m_view->setGeometry(QRect(0, 0, windowSize.width(), windowSize.height()));
        m_scene->setSceneRect(QRect(0, 0, windowSize.width(), windowSize.height()));

        QWidget::resizeEvent(event);
    }
} // CM
