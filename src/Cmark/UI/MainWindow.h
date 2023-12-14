#ifndef CAMERAMARK_MAINWINDOW_H
#define CAMERAMARK_MAINWINDOW_H

#include <QMainWindow>

namespace CM
{
    class DisplayWidget;
    class FileTreeDockWidget;
    class ImagePropertyDockWidget;
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
        void InitTool();
        void InitConnect();

    private:
        /// widget
        std::shared_ptr<DisplayWidget> m_displayWidget{nullptr};
        std::shared_ptr<FileTreeDockWidget> m_leftDockWidget{nullptr};
        std::shared_ptr<ImagePropertyDockWidget> m_rightDockWidget{nullptr};

        /// File Actions
        QAction* newAction{nullptr};
        QAction* openDirectoryAction{nullptr};   ///< 打开文件夹
    };

} // CM

#endif //CAMERAMARK_MAINWINDOW_H
