#include "BatchProcessSetting.h"
#include "./ui_BatchProcessSetting.h"

BatchProcessSetting::BatchProcessSetting(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::BatchProcessSetting)
{
    ui->setupUi(this);
}

BatchProcessSetting::~BatchProcessSetting()
{
    delete ui;
}
