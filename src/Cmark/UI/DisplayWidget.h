#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <QWidget>

namespace CM
{

    class DisplayWidget : public QWidget
    {
    public:
        explicit DisplayWidget(QWidget * parent = nullptr);

    protected:
        void paintEvent(QPaintEvent * event) override;


    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
