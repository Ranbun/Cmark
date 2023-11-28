#ifndef CAMERAMARK_MAINWINDOW_H
#define CAMERAMARK_MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include <filesystem>

namespace CM
{
    class DisplayWidget;
    class LeftDockWidget;

}

namespace CM
{

    class MainWindow :public  QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow();
        void PreViewImage(const std::filesystem::path & path) const;

    private:

        void InitWindowLayout();
        void InitUi();
    	void InitMenu();
        void InitConnect();

    private:
        /// widget
        std::shared_ptr<DisplayWidget> m_displayWidget;
        std::shared_ptr<LeftDockWidget> m_leftDockWidget;


        /// File Actions
        QAction* newAction;
        QAction* openDirectoryAction;   ///< 打开文件夹

    };

} // CM

#endif //CAMERAMARK_MAINWINDOW_H
