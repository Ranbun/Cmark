#include "MainWindow.h"
#include "DisplayWidget.h"

namespace CM
{
    MainWindow::MainWindow()
    : QMainWindow()
    {
        m_displayWidget = std::shared_ptr<DisplayWidget>(new DisplayWidget,[]([[maybe_unused]] DisplayWidget * w){});
        this->setCentralWidget(m_displayWidget.get());

        this->resize({ 960,720 });  ///< resize Window 

    }
} // CM