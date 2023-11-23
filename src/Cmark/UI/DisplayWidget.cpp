#include <QPen>
#include "DisplayWidget.h"
#include <filesystem>

namespace CM
{
    DisplayWidget::DisplayWidget(QWidget *parent)
    :QWidget(parent)
    {

    }

    void DisplayWidget::Open(const std::filesystem::path& path)
    {

    }

    void DisplayWidget::paintEvent(QPaintEvent *event)
    {

        QWidget::paintEvent(event);
    }
} // CM