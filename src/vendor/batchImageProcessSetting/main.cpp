#include "BatchProcessSetting.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BatchProcessSetting w;
    w.show();
    return a.exec();
}
