#ifndef  CAMERAMARK_LEFTDOCKWIDGET_H_
#define CAMERAMARK_LEFTDOCKWIDGET_H_

#include <QDockWidget>
#include <QTextEdit>

class QTreeView;
class QFileSystemModel;

namespace CM
{
    class LeftDockWidget : public QDockWidget
    {
        Q_OBJECT
    public:
        LeftDockWidget(const QString & title,QWidget * parent = nullptr);
        ~LeftDockWidget() override;

        void New();
        void ShowMessage(const std::string& message);

    private:
        QTreeView * m_TreeView;
        QFileSystemModel * m_FileSystemModel;

    signals:
        void previewImage(const QString & path);

    };

}


#endif // ! CAMERAMARK_LEFTDOCKWIDGET_H_
