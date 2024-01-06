#ifndef  CAMERAMARK_FILETREEDOCKWIDGET_H_
#define  CAMERAMARK_FILETREEDOCKWIDGET_H_

#include <QDockWidget>
#include <QTextEdit>

class QTreeView;
class QFileSystemModel;

namespace CM
{
    class FileTreeDockWidget : public QDockWidget
    {
        Q_OBJECT
    public:
        explicit FileTreeDockWidget(const QString & title, QWidget * parent = nullptr);
        ~FileTreeDockWidget() override;

        void New();
        void ShowMessage(const std::string& message);

    private:
        QTreeView * m_TreeView;
        QFileSystemModel * m_FileSystemModel;

    signals:
        void previewImage(const QString & path);

    };

}


#endif // ! CAMERAMARK_FILETREEDOCKWIDGET_H_
