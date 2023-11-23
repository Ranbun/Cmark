#include <QPen>
#include "DisplayWidget.h"

namespace CM
{
    DisplayWidget::DisplayWidget(QWidget *parent)
    :QWidget(parent)
    {

    }

    void DisplayWidget::paintEvent(QPaintEvent *event)
    {

        QWidget::paintEvent(event);
    }
} // CM