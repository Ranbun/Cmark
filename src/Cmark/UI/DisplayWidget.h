#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <filesystem>
#include <QWidget>

namespace CM
{

    class DisplayWidget : public QWidget
    {
    public:
        explicit DisplayWidget(QWidget * parent = nullptr);
    public:
        void Open(const std::filesystem::path& path);


    protected:
        void paintEvent(QPaintEvent * event) override;


    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
