#include "MainWindow.h"

#include "DisplayWidget.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QToolBar>
#include <QFileDialog>
#include <QAction>

#include "FileTreeDockWidget.h"
#include "ImagePropertyDockWidget.h"
#include "StatusBar.h"

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
        m_displayWidget = std::shared_ptr<DisplayWidget>(new DisplayWidget, []([[maybe_unused]] DisplayWidget* w) {});
        this->setCentralWidget(m_displayWidget.get());
        this->setMinimumSize({ 960,720 });  ///< resize Window
        m_displayWidget->setMinimumSize(640,480);
        /// left dock widget
        {
            m_leftDockWidget = std::shared_ptr<FileTreeDockWidget>(new FileTreeDockWidget("Dock Widget", this), []([[maybe_unused]] FileTreeDockWidget* w) {});
            m_leftDockWidget->setWindowTitle("");

            addDockWidget(Qt::LeftDockWidgetArea, m_leftDockWidget.get());
        }

        /// right dock widget
        {
            m_rightDockWidget = std::shared_ptr<ImagePropertyDockWidget>(new ImagePropertyDockWidget(this), []([[maybe_unused]] ImagePropertyDockWidget* w) {});
            m_rightDockWidget->setWindowTitle("");
            m_rightDockWidget->setVisible(false);

            // addDockWidget(Qt::RightDockWidgetArea, m_rightDockWidget.get());
        }

        setContentsMargins(0,0,0,0);
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
        connect(newAction, &QAction::triggered, [this]()
        {
            m_leftDockWidget->New();

        });
        connect(openDirectoryAction, &QAction::triggered, [this]()
        {
            const auto directoryPath = QFileDialog::getExistingDirectory(this);
            const auto path = std::filesystem::path(directoryPath.toStdString());
            m_leftDockWidget->ShowMessage(directoryPath.toStdString());
            m_displayWidget->Open(path);
        });

        QObject::connect(m_leftDockWidget.get(), &FileTreeDockWidget::previewImage, [this](const QString & path)
        {
            std::filesystem::path imagePath(path.toStdString());
            StatusBar::showMessage("preview image: " + path);
            StatusBar::repaint();
            PreViewImage(imagePath);
        });

    }

    void MainWindow::InitMenu()
    {
        auto status = statusBar();  ///< 使用内置方法创建 status bar
        CM::StatusBar::Init(status);

        const auto MenuBar = menuBar();

        const auto file = new QMenu("File(&F)");
        MenuBar->addMenu(file);

        newAction = new QAction("New");
        newAction->setIcon(QIcon("./sources/icons/new.png"));
        newAction->setToolTip(tr("set FileSystem Empty "));
        newAction->setShortcut({ "Ctrl+N" });
        file->addAction(newAction);

        openDirectoryAction = new QAction("Open");
        openDirectoryAction->setToolTip(tr("Open Directory"));
        openDirectoryAction->setShortcut({ "Ctrl+P" });
        openDirectoryAction->setIcon(QIcon("./sources/icons/open.png"));
        file->addAction(openDirectoryAction);
    }

    void MainWindow::PreViewImage(const std::filesystem::path & path) const
    {
        m_displayWidget->PreViewImage(path);
    }

    void MainWindow::InitTool()
    {
        auto toolBar = addToolBar("toolbar");

        toolBar->setFloatable(false);
        toolBar->setMovable(false);
        toolBar->setIconSize({16,16});

        /// save preview image
        {
            auto savePreviewImageAction = toolBar->addAction("Save preview");
            savePreviewImageAction->setToolTip("Save preview Image");
            QPixmap previewSceneSaveIcon("./sources/icons/previewSceneSave.png");
            previewSceneSaveIcon = previewSceneSaveIcon.scaled({16, 16}, Qt::KeepAspectRatio);
            savePreviewImageAction->setIcon(previewSceneSaveIcon);

            connect(savePreviewImageAction, &QAction::triggered, [this]() {
                m_displayWidget->saveScene(SceneIndex::PREVIEW_SCENE);
            });
        }

        /// save it
        {
            auto save = toolBar->addAction("Add Logo Save");
            save->setToolTip("Save as Image with logo");
            QPixmap previewSceneSaveIcon("./sources/icons/save.png");
            previewSceneSaveIcon = previewSceneSaveIcon.scaled({16, 16}, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            save->setIcon(previewSceneSaveIcon);

            connect(save, &QAction::triggered, [this]() {
                m_displayWidget->saveScene(SceneIndex::GENERATELOGO_SCENE);
            });
        }

        /// save loaded pixmap
        {
#if  _DEBUG
            auto save = toolBar->addAction("loaded pixmap Save");
            save->setToolTip("Save Loaded pixmap");
            QPixmap previewSceneSaveIcon("./sources/icons/save_uploaded.png");
            previewSceneSaveIcon = previewSceneSaveIcon.scaled({16, 16}, Qt::KeepAspectRatio,Qt::SmoothTransformation);
            save->setIcon(previewSceneSaveIcon);

            connect(save, &QAction::triggered, [this]() {
                m_displayWidget->saveLoadedPixmap();
            });
#endif
        }


    }
} // CM
