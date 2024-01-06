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
    class MainWindow final :public  QMainWindow
    {
        Q_OBJECT
    public:
        MainWindow();
        void preViewImage(const std::filesystem::path & path) const;

    private:
        void InitWindowLayout();
        void InitUi();
        void InitMenu();
        void InitTool();
        void InitConnect();

    protected:

        /// widget
        std::shared_ptr<DisplayWidget> m_DisplayWidget{nullptr};
        std::shared_ptr<FileTreeDockWidget> m_LeftDockWidget{nullptr};
        std::shared_ptr<ImagePropertyDockWidget> m_RightDockWidget{nullptr};

        /// File Actions
        QAction* m_NewAction{nullptr};
        QAction* m_OpenDirectoryAction{nullptr};   ///< 打开文件夹

        /// Edit
        QAction * m_EditPreviewSceneLayoutAction{nullptr};

    };

} // CM

#endif //CAMERAMARK_MAINWINDOW_H
