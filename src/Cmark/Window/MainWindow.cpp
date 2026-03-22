#include "MainWindow.h"
#include <CMark.h>

#include <File/BatchImageProcess.h>
#include <File/ResourcesTool.h>
#include <Log/CLog.h>
#include "DisplayWidget.h"
#include "FileTreeDockWidget.h"
#include "ImagePropertyDockWidget.h"
#include "StatusBar.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QStyle>
#include <QThread>
#include <QToolBar>

namespace CM
{
MainWindow::MainWindow()
    : QMainWindow(nullptr)
{
    InitUi();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
}

void MainWindow::InitWindowLayout()
{
    /// Central Widget
    m_DisplayWidget = std::shared_ptr<DisplayWidget>(new DisplayWidget, []([[maybe_unused]] DisplayWidget* w) {});
    this->setCentralWidget(m_DisplayWidget.get());
    this->setMinimumSize({960, 720});  ///< resize Window
    m_DisplayWidget->setMinimumSize(640, 480);
    /// left dock widget
    {
        m_FileTreeDockWidget = std::shared_ptr<FileTreeDockWidget>(new FileTreeDockWidget("Dock Widget", this),
                                                                   []([[maybe_unused]] FileTreeDockWidget* w) {});
        m_FileTreeDockWidget->setWindowTitle("");

        addDockWidget(Qt::LeftDockWidgetArea, m_FileTreeDockWidget.get());
    }

    /// right dock widget
    {
        m_ImagePropertyDockWidget = std::shared_ptr<ImagePropertyDockWidget>(
            new ImagePropertyDockWidget(this), []([[maybe_unused]] ImagePropertyDockWidget* w) {});
        m_ImagePropertyDockWidget->setVisible(true);
        addDockWidget(Qt::LeftDockWidgetArea, m_ImagePropertyDockWidget.get());
    }

    splitDockWidget(m_FileTreeDockWidget.get(), m_ImagePropertyDockWidget.get(), Qt::Vertical);

    setContentsMargins(0, 0, 0, 0);
}

void MainWindow::InitUi()
{
    InitWindowLayout();
    InitMenu();
    InitTool();
    InitConnect();

    StatusBar::showMessage("Initialized!");
}

void MainWindow::InitConnect()
{
    /// menu actions
    connect(m_CleanWorkspaceAction, &QAction::triggered,
            [this]()
            {
                m_FileTreeDockWidget->New();
                Tools::ResourcesTools::destroy();
            });

    connect(m_OpenDirectoryAction, &QAction::triggered,
            [this]()
            {
                const auto workPath = m_FileTreeDockWidget->rootImagePath();
                const auto directoryPath = QFileDialog::getExistingDirectory(
                    this, "Select Directory", workPath.isEmpty() ? QString("./") : workPath);
                if (directoryPath.isEmpty())
                {
                    CLog::Warning<QString>("Can not found Path!");
                    return;
                }
                const QDir dir(directoryPath);
                m_FileTreeDockWidget->Open(dir);
                emit m_DisplayWidget->sigOpen(directoryPath.toStdString());

                StatusBar::showMessage("open directory: " + directoryPath);
                StatusBar::repaint();
            });

    connect(m_BatchProcessImageAction, &QAction::triggered, this,
            [this]()
            {
                const QString rootPath = m_FileTreeDockWidget->rootImagePath();
                if (rootPath.isEmpty())
                {
                    CLog::Warning("BatchProcessImage error: Can not found Path!");
                    return;
                }

                auto* batchImageProcessor = new BatchImageProcessor({rootPath});
                auto* thread = new QThread;

                batchImageProcessor->moveToThread(thread);
                connect(thread, &QThread::started, batchImageProcessor, &BatchImageProcessor::Run);
                connect(thread, &QThread::finished, thread, &QThread::deleteLater);
                connect(batchImageProcessor, &BatchImageProcessor::workFinished, batchImageProcessor,
                        &BatchImageProcessor::deleteLater);

                thread->start();
            });

    /// 打开文件
    connect(m_PreviewFileAction, &QAction::triggered, this,
            [this]()
            {
                const QString file = QFileDialog::getOpenFileName(this);
                const QFileInfo fileIns(file);
                if (!fileIns.exists())
                {
                    return;
                }
                CLogInstance.PrintMes<QString>(file);
                emit m_DisplayWidget->sigPreviewImage(file.toStdString());
            });

    /// 显示图片属性
    connect(m_DisplayWidget.get(), &DisplayWidget::sigShowPreviewItemProperty, this,
            [this](const std::string& path)
            {
                emit m_ImagePropertyDockWidget->sigShowProperty(path);
            });

    /// 显示文件
    connect(m_FileTreeDockWidget.get(), &FileTreeDockWidget::previewImage,
            [this](const QString& path)
            {
                StatusBar::showMessage("preview image: " + path);
                StatusBar::repaint();

                emit m_DisplayWidget->sigPreviewImage(path.toStdString());
                emit m_ImagePropertyDockWidget->sigShowProperty(path.toStdString());
            });

    /// 警告信息
    connect(
        this, &MainWindow::sigWarning, this, [](const QString& info) { CLog::Warning(info); }, Qt::QueuedConnection);

    /// 开启预览信息
    connect(m_EnablePreViewAction, &QAction::triggered, this,
            [this](bool checked)
            {
                if (checked)
                {
                    emit m_DisplayWidget->sigDisplayImageMode(DisplayWidgetMode::IMAGE_WITH_INFO);
                }
                else
                {
                    emit m_DisplayWidget->sigDisplayImageMode(DisplayWidgetMode::IMAGE);
                }
            });
}

void MainWindow::InitMenu()
{
    const auto status = statusBar();  ///< 使用内置方法创建 status bar
    CM::StatusBar::Init(status);

    const auto currentMenuBar = menuBar();

    const auto file = new QMenu(R"(File(&F))");
    currentMenuBar->addMenu(file);

    m_PreviewFileAction = new QAction("Preview File");
    file->addAction(m_PreviewFileAction);
    m_PreviewFileAction->setIcon(QIcon(":/icons/openFile.png"));

    m_OpenDirectoryAction = new QAction("Open Directory");
    m_OpenDirectoryAction->setToolTip(tr("Open Directory"));
    m_OpenDirectoryAction->setShortcut({"Ctrl+P"});
    m_OpenDirectoryAction->setIcon(QIcon(":/icons/openDirectory.png"));
    file->addAction(m_OpenDirectoryAction);

    m_BatchProcessImageAction = new QAction("Process All");
    m_BatchProcessImageAction->setToolTip(tr("Process All Image Files"));
    m_BatchProcessImageAction->setIcon(QIcon(":/icons/multiProcess.png"));
    m_BatchProcessImageAction->setShortcut({"Ctrl+Shift+A"});
    file->addAction(m_BatchProcessImageAction);

    m_CleanWorkspaceAction = new QAction("Clean WorkSpace");
    m_CleanWorkspaceAction->setIcon(QIcon(":/icons/new.png"));
    m_CleanWorkspaceAction->setToolTip(tr("set FileSystem Empty "));
    file->addAction(m_CleanWorkspaceAction);

    const auto Edit = new QMenu(R"(Edit(&E))");
    currentMenuBar->addMenu(Edit);
    m_EnablePreViewAction = new QAction("Enable PreView", this);
    m_EnablePreViewAction->setToolTip(tr("Enable PreView"));
    m_EnablePreViewAction->setCheckable(true);
    m_EnablePreViewAction->setChecked(false);
    Edit->addAction(m_EnablePreViewAction);
}

void MainWindow::InitTool()
{
    const auto toolBar = addToolBar("toolbar");

    toolBar->setFloatable(false);
    toolBar->setMovable(false);
    toolBar->setIconSize({16, 16});

    toolBar->addAction(m_PreviewFileAction);
    toolBar->addAction(m_OpenDirectoryAction);

    /// 添加分割线
    toolBar->addSeparator();

    /// save preview image
    {
        const auto savePreviewImageAction = toolBar->addAction("Save preview");
        savePreviewImageAction->setToolTip("Save preview Image");
        QPixmap previewSceneSaveIcon(":/icons/save.png");
        previewSceneSaveIcon = previewSceneSaveIcon.scaled({16, 16}, Qt::KeepAspectRatio);
        savePreviewImageAction->setIcon(previewSceneSaveIcon);

        connect(savePreviewImageAction, &QAction::triggered,
                [this]() { m_DisplayWidget->saveScene(SceneIndex::PreviewScene); });
    }
}
}  // namespace CM
