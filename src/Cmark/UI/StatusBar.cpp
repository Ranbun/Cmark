#include "StatusBar.h"

#include <QStatusBar>
#include <QString>

namespace CM
{
    QStatusBar * StatusBar::m_statusBar = nullptr;

    void StatusBar::showMessage(const QString &message)
    {
        StatusBar::check();
        StatusBar::m_statusBar->showMessage(message,0);
    }

    void StatusBar::showMessage(const QString &message, int timeout)
    {
        StatusBar::check();
        m_statusBar->showMessage(message,timeout);
    }

    void StatusBar::Init(QStatusBar *bar)
    {
        m_statusBar = bar;
        StatusBar::check();
    }

    void StatusBar::clearMessage()
    {
        StatusBar::check();
        StatusBar::m_statusBar->clearMessage();
    }

    void StatusBar::repaint()
    {
        StatusBar::check();
        StatusBar::m_statusBar->repaint();
    }

    QStatusBar *StatusBar::statusBar()
    {
        StatusBar::check();
        return m_statusBar;
    }

    void StatusBar::check()
    {
        if(m_statusBar == nullptr)
        {
            throw std::runtime_error("Please Init StatusBar!");
        }
    }
} // CM
