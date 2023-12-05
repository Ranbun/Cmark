#include "FileTreeDockWidget.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>

#if _DEBUG
#include <QDebug>
#endif

namespace CM
{
    FileTreeDockWidget::FileTreeDockWidget(const QString& title, QWidget* parent)
        : QDockWidget(title,parent)
        , m_FileSystemModel(nullptr)
    {

        setContentsMargins(0, 0, 0, 0);
        setFeatures(NoDockWidgetFeatures);

        /// show file tree
        m_TreeView = new QTreeView(this);
        m_TreeView->setWindowTitle("Directory Tree");

        setWidget(m_TreeView);

        setStyleSheet("QDockWidget { background-color: lightgray; }");

        connect(m_TreeView, &QTreeView::clicked, this, [this](const QModelIndex& index)
        {
            auto isDir = m_FileSystemModel->isDir(index);
            if(isDir)
            {
                return ;
            }

            const QString filePath = m_FileSystemModel->filePath(index);
            emit previewImage(filePath);
            qDebug() << "Clicked file path: " << filePath;
        });

#if  _DEBUG
        auto lTitleBar = this->titleBarWidget();
        auto lEmptyWidget = new QWidget();
        setTitleBarWidget(lEmptyWidget);
        delete lTitleBar;
#endif

        layout()->setContentsMargins(0,0,0,0);
        layout()->setSpacing(0);

    }

    FileTreeDockWidget::~FileTreeDockWidget() = default;

    void FileTreeDockWidget::New()
    {
        m_TreeView->setModel(nullptr);
        m_FileSystemModel->deleteLater();
        delete m_FileSystemModel;
        m_FileSystemModel = nullptr;
    }

    void FileTreeDockWidget::ShowMessage(const std::string& message)
    {
        if(!m_FileSystemModel)
        {
            m_FileSystemModel = new QFileSystemModel;
            m_FileSystemModel->setRootPath(message.c_str());
            m_TreeView->setModel(m_FileSystemModel);

            QStringList filter;
            filter <<"*.jpg";
            m_FileSystemModel->setNameFilters(filter);
            m_FileSystemModel->setNameFilterDisables(false);
        }
        m_FileSystemModel->setRootPath(message.c_str());
        m_TreeView->setRootIndex(m_FileSystemModel->index(message.c_str()));
    }
}
