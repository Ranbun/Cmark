#include "DisplayWidget.h"

#include <filesystem>

#include <QPixmap>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>

namespace CM
{
    DisplayWidget::DisplayWidget(QWidget *parent)
    :QWidget(parent)
    , m_scene(new QGraphicsScene)
    {

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
        QVBoxLayout layout(this);

        auto p = QString::fromStdString(path.string());
        QPixmap pixmap(p);
        pixmap.scaled(400,300);

        // 创建一个视图，并将场景设置为视图的场景
        QGraphicsView view(m_scene);

        view.resize(400, 300);

        m_scene->addPixmap(pixmap);

        view.setScene(m_scene);
        // 将视图添加到布局中
        layout.addWidget(&view);

        // 设置QWidget的布局
        setLayout(&layout);

        // pixmap.save("./test.png","png");
    }
} // CM
