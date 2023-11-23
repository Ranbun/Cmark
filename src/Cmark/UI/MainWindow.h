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

        void InitUI();
    	void InitMenu();
        void InitConnect();

    private:
        std::shared_ptr<DisplayWidget> m_displayWidget;

        /// File Actions
        QAction* newAction;
        QAction* openDirectoryAction;   ///< 打开文件夹

    };

} // CM

#endif //CAMERAMARK_MAINWINDOW_H
