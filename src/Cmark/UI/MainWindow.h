#ifndef CAMERAMARK_MAINWINDOW_H
#define CAMERAMARK_MAINWINDOW_H

#include <QMainWindow>
#include <memory>

namespace CM
{
    class DisplayWidget;
}

namespace CM
{

    class MainWindow :public  QMainWindow
    {
    public:
        MainWindow();

    private:
        std::shared_ptr<DisplayWidget> m_displayWidget;

    };

} // CM

#endif //CAMERAMARK_MAINWINDOW_H
