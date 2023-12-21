#include "SceneLayoutEditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<QWidget> w = std::make_shared<SceneLayoutEditor>();
    w->show();
    return a.exec();
}
