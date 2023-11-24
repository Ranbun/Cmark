#include "MainWindow.h"

#include "DisplayWidget.h"

#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QTextEdit>

#include "LeftDockWidget.h"

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
        this->resize({ 960,720 });  ///< resize Window

        /// left dock widget
        {
            m_leftDockWidget = std::shared_ptr<LeftDockWidget>(new LeftDockWidget("Dock Widget", this), []([[maybe_unused]] LeftDockWidget* w) {});
            m_leftDockWidget->setWindowTitle("");

            addDockWidget(Qt::LeftDockWidgetArea, m_leftDockWidget.get());
        }

        setContentsMargins(0,0,0,0);

    }

    void MainWindow::InitUi()
    {
        InitWindowLayout();
        InitMenu();
        InitConnect();
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

    }

    void MainWindow::InitMenu()
    {
        const auto MenuBar = menuBar();

        const auto file = new QMenu("File(&F)");
        MenuBar->addMenu(file);

        newAction = new QAction("New");
        newAction->setShortcut({ "Ctrl+N" });
        file->addAction(newAction);

        openDirectoryAction = new QAction("Open");
        openDirectoryAction->setShortcut({ "Ctrl+P" });
        file->addAction(openDirectoryAction);


        const auto StatusBar = statusBar();
        StatusBar->showMessage("Init loading");

    }

    void MainWindow::AddImage(std::filesystem::path &path)
    {
        m_displayWidget->AddImage(path);
    }
} // CM
