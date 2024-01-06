#ifndef CAMERAMARK_IMAGEPROPERTYDOCKWIDGET_H
#define CAMERAMARK_IMAGEPROPERTYDOCKWIDGET_H

#include <QDockWidget>

namespace CM
{
    class ImagePropertyDockWidget : public QDockWidget
    {
    public:
        ImagePropertyDockWidget(QWidget *parent = nullptr);
        ~ImagePropertyDockWidget() override = default;
    };

} // CM

#endif //CAMERAMARK_IMAGEPROPERTYDOCKWIDGET_H
