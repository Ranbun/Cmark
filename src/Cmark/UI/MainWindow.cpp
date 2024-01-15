#include <CMark.h>

#include "MainWindow.h"

#include "FileTreeDockWidget.h"
#include "ImagePropertyDockWidget.h"
#include "StatusBar.h"
#include "DisplayWidget.h"
#include "sources/ResourcesTool.h"


#include <QMenuBar>
#include <QToolBar>
#include <QFileDialog>
#include <QAction>

#if _DEBUG
#include <QDebug>
#endif

namespace CM
{
    MainWindow::MainWindow()
        : QMainWindow(nullptr)
    {
        InitUi();
    }

    void MainWindow::InitWindowLayout()
    {
        /// Central Widget
        m_DisplayWidget = std::shared_ptr<DisplayWidget>(new DisplayWidget, []([[maybe_unused]] DisplayWidget* w)
        {
        });
        this->setCentralWidget(m_DisplayWidget.get());
        this->setMinimumSize({960, 720}); ///< resize Window
        m_DisplayWidget->setMinimumSize(640, 480);
        /// left dock widget
        {
            m_LeftDockWidget = std::shared_ptr<FileTreeDockWidget>(new FileTreeDockWidget("Dock Widget", this),
                                                                   []([[maybe_unused]] FileTreeDockWidget* w)
                                                                   {
                                                                   });
            m_LeftDockWidget->setWindowTitle("");

            addDockWidget(Qt::LeftDockWidgetArea, m_LeftDockWidget.get());
        }

        /// right dock widget
        {
            m_RightDockWidget = std::shared_ptr<ImagePropertyDockWidget>(
                new ImagePropertyDockWidget(this), []([[maybe_unused]] ImagePropertyDockWidget* w)
                {
                });
            m_RightDockWidget->setWindowTitle("");
            m_RightDockWidget->setVisible(false);

            // addDockWidget(Qt::RightDockWidgetArea, m_rightDockWidget.get());
        }

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
        connect(m_NewAction, &QAction::triggered, [this]()
        {
            m_LeftDockWidget->New();
            Tools::ResourcesTools::destory();
        });

        connect(m_OpenDirectoryAction, &QAction::triggered, [this]()
        {
            const auto directoryPath = QFileDialog::getExistingDirectory(this);
            const auto path = std::filesystem::path(directoryPath.toStdString());
            m_LeftDockWidget->showMessage(directoryPath.toStdString());
            emit m_DisplayWidget->sigOpen(directoryPath.toStdString());
        });

        connect(m_BatchProcessImage, &QAction::triggered, this,[this]()
        {
            const QString rootPath  = emit sigBatchProcessImagesRootPath();
            const QDir directory(rootPath);

            // 获取目录下的所有文件
            QFileInfoList fileList = directory.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);

#if _DEBUG
            // 遍历文件列表并输出文件名
            foreach(const QFileInfo & fileInfo, fileList)
            {
                qDebug() << fileInfo.fileName();
                /// 读取文件
                /// 获取旋转
                QImage i;
                

            }
#endif 



        });
        
        connect(this,&MainWindow::sigBatchProcessImagesRootPath,m_LeftDockWidget.get(),[this]()->QString
            {
                return m_LeftDockWidget->rootImagePath();
            });


        connect(m_LeftDockWidget.get(), &FileTreeDockWidget::previewImage, [this](const QString& path)
        {
            const std::filesystem::path imagePath(path.toStdString());
            StatusBar::showMessage("preview image: " + path);
            StatusBar::repaint();
            emit m_DisplayWidget->sigPreViewImage(path.toStdString());
        });

#if  0
        connect(m_EditPreviewSceneLayoutAction, &QAction::triggered, m_DisplayWidget.get(),
                &DisplayWidget::sigPreViewLayoutSettingsPanel);
#endif
    }

    void MainWindow::InitMenu()
    {
        const auto status = statusBar(); ///< 使用内置方法创建 status bar
        CM::StatusBar::Init(status);

        const auto MenuBar = menuBar();

        const auto file = new QMenu("File(&F)");
        MenuBar->addMenu(file);

        m_NewAction = new QAction("New");
        m_NewAction->setIcon(QIcon("./sources/icons/new.png"));
        m_NewAction->setToolTip(tr("set FileSystem Empty "));
        m_NewAction->setShortcut({"Ctrl+N"});
        file->addAction(m_NewAction);

        m_OpenDirectoryAction = new QAction("Open");
        m_OpenDirectoryAction->setToolTip(tr("Open Directory"));
        m_OpenDirectoryAction->setShortcut({"Ctrl+P"});
        m_OpenDirectoryAction->setIcon(QIcon("./sources/icons/open.png"));
        file->addAction(m_OpenDirectoryAction);

        m_BatchProcessImage = new QAction("Process All");
        m_BatchProcessImage->setToolTip(tr("Process All Image Files"));
        m_BatchProcessImage->setIcon(QIcon("./sources/icons/open.png"));
        file->addAction(m_BatchProcessImage);

        const auto Edit = new QMenu("Edit(&E)");
        MenuBar->addMenu(Edit);
        m_EditPreviewSceneLayoutAction = new QAction("Layout Setting");
        m_EditPreviewSceneLayoutAction->setToolTip(tr("Preview Scene Layout Setting"));
        m_EditPreviewSceneLayoutAction->setShortcut({"Ctrl+E"});
        m_EditPreviewSceneLayoutAction->setIcon(QIcon("./sources/icons/previewSceneLayoutsettings.png"));
        Edit->addAction(m_EditPreviewSceneLayoutAction);

    }

    void MainWindow::InitTool()
    {
        const auto toolBar = addToolBar("toolbar");

        toolBar->setFloatable(false);
        toolBar->setMovable(false);
        toolBar->setIconSize({16, 16});

        /// save preview image
        {
            const auto savePreviewImageAction = toolBar->addAction("Save preview");
            savePreviewImageAction->setToolTip("Save preview Image");
            QPixmap previewSceneSaveIcon("./sources/icons/previewSceneSave.png");
            previewSceneSaveIcon = previewSceneSaveIcon.scaled({16, 16}, Qt::KeepAspectRatio);
            savePreviewImageAction->setIcon(previewSceneSaveIcon);

            connect(savePreviewImageAction, &QAction::triggered, [this]()
            {
                m_DisplayWidget->saveScene(SceneIndex::PreviewScene);
            });
        }

        /// save it
        {
            const auto save = toolBar->addAction("Add Logo Save");
            save->setToolTip("Save as Image with logo");
            QPixmap previewSceneSaveIcon("./sources/icons/save.png");
            previewSceneSaveIcon = previewSceneSaveIcon.scaled({16, 16}, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            save->setIcon(previewSceneSaveIcon);

            connect(save, &QAction::triggered, [this]()
            {
                m_DisplayWidget->saveScene(SceneIndex::GenerateLogoScene);
            });
        }



    }
} // CM
