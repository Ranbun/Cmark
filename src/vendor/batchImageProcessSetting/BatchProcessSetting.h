#ifndef BATCHPROCESSSETTING_H
#define BATCHPROCESSSETTING_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class BatchProcessSetting;
}
QT_END_NAMESPACE

class BatchProcessSetting : public QDialog
{
    Q_OBJECT

public:
    BatchProcessSetting(QWidget *parent = nullptr);
    ~BatchProcessSetting();

private:
    Ui::BatchProcessSetting *ui;
};
#endif // BATCHPROCESSSETTING_H
