#include "LeftDockWidget.h"

#include <QTextEdit>
#include <QVBoxLayout>
#include <QTreeView>
#include <QFileSystemModel>

#if _DEBUG
#include <QDebug>
#endif

namespace CM
{
    LeftDockWidget::LeftDockWidget(const QString& title, QWidget* parent)
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

    LeftDockWidget::~LeftDockWidget() = default;

    void LeftDockWidget::New()
    {
        m_TreeView->setModel(nullptr);
        m_FileSystemModel->deleteLater();
        delete m_FileSystemModel;
        m_FileSystemModel = nullptr;
    }

    void LeftDockWidget::ShowMessage(const std::string& message)
    {
        if(!m_FileSystemModel)
        {
            m_FileSystemModel = new QFileSystemModel;
            m_FileSystemModel->setRootPath(message.c_str());
            m_TreeView->setModel(m_FileSystemModel);
        }
        m_FileSystemModel->setRootPath(message.c_str());
        m_TreeView->setRootIndex(m_FileSystemModel->index(message.c_str()));
    }
}
