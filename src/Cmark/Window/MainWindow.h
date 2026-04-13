#ifndef CAMERAMARK_MAINWINDOW_H
#define CAMERAMARK_MAINWINDOW_H

#include <QMainWindow>

namespace CM
{
class DisplayWidget;
class FileTreeDockWidget;
class ImagePropertyDockWidget;
}  // namespace CM

namespace CM
{
class MainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

private:
    /// layout widgets
    std::shared_ptr<DisplayWidget> m_DisplayWidget{nullptr};                      ///< display widget: show scene(image)
    std::shared_ptr<FileTreeDockWidget> m_FileTreeDockWidget{nullptr};            ///< show file tree
    std::shared_ptr<ImagePropertyDockWidget> m_ImagePropertyDockWidget{nullptr};  ///< show file property

    /// File Menu Actions
    QAction* m_CleanWorkspaceAction{nullptr};
    QAction* m_OpenDirectoryAction{nullptr};  ///< 打开文件夹
    QAction* m_BatchProcessImageAction{nullptr};
    QAction* m_PreviewFileAction{nullptr};

    /// Edit Menu Actions
    QAction* m_EnablePreViewAction{nullptr};

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void InitWindowLayout();
    void InitUi();
    void InitMenu();
    void InitTool();
    void InitConnect();

signals:
    void sigWarning(QString info);
};

}  // namespace CM

#endif  // CAMERAMARK_MAINWINDOW_H
