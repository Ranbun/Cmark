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

    void DisplayWidget::AddImage(std::filesystem::path & path)
    {
        const auto & geometry = this->geometry();
        QVBoxLayout layout(this);

        auto p = QString::fromStdString(path.string());
        QPixmap pixmap(p);
        pixmap = pixmap.scaled(400,300);
        auto item = m_scene->addPixmap(pixmap);
        static int i = 1;
        item->setPos((i++) * 400,300);
        // 将视图添加到布局中
    }

    void DisplayWidget::resizeEvent(QResizeEvent *event)
    {
        auto windowSize = event->size();
        m_view->setGeometry(QRect(0, 0, windowSize.width(), windowSize.height()));
        QWidget::resizeEvent(event);
    }
} // CM
