#include "SceneLayoutEditor.h"
#include "./ui_dialog.h"

SceneLayoutEditor::SceneLayoutEditor(QWidget* parent)
    : QDialog(parent)
      , ui(new Ui::SceneLayout)
{
    ui->setupUi(this);
}

SceneLayoutEditor::~SceneLayoutEditor()
{
    delete ui;
}

void SceneLayoutEditor::show(CM::SceneLayoutSettings& setting)
{
    /// TODO: update show info

    sceneLayout = &setting;
    QWidget::show();
    update(setting);
}

void SceneLayoutEditor::update(CM::SceneLayoutSettings& setting)
{
    sceneLayout = &setting;

    const auto& [l, r, t, b] = sceneLayout->getMargin();
    {
        /// left
        ui->m_LeftMarginSlider->setValue(l);
        ui->m_leftMarginValueLabel->setText(QString::number(l));
    }
    {
        /// right
        ui->m_rightMarginSlider->setValue(r);
        ui->m_rightMargibValueLabel->setText(QString::number(r));
    }
    {   /// top
        ui->m_topMarginSlider->setValue(t);
        ui->m_topMarginValueLabel->setText(QString::number(t));
    }
    {   /// bottom

        ui->m_bottomMarginSlider->setValue(b);
        ui->m_bottomMarginValueLabel->setText(QString::number(b));
    }
}

void SceneLayoutEditor::on_m_LeftMarginSlider_valueChanged(const int value)
{
    ui->m_leftMarginValueLabel->setText(QString::number(value));
    auto& [l,r,t,b] = sceneLayout->getMargin();
    l = ui->m_LeftMarginSlider->value();
    sceneLayout->update();
    emit updatedScene();
}


void SceneLayoutEditor::on_m_rightMarginSlider_valueChanged(const int value)
{
    ui->m_rightMargibValueLabel->setText(QString::number(value));
    auto& [l, r, t, b] = sceneLayout->getMargin();
    r = ui->m_rightMarginSlider->value();
    sceneLayout->update();
    emit updatedScene();
}


void SceneLayoutEditor::on_m_topMarginSlider_valueChanged(const int value)
{
    ui->m_topMarginValueLabel->setText(QString::number(value));
    auto& [l, r, t, b] = sceneLayout->getMargin();
    t = ui->m_topMarginSlider->value();
    sceneLayout->update();
    emit updatedScene();
}


void SceneLayoutEditor::on_m_bottomMarginSlider_valueChanged(const int value)
{
    ui->m_bottomMarginValueLabel->setText(QString::number(value));
    auto& [l, r, t, b] = sceneLayout->getMargin();
    b = ui->m_bottomMarginSlider->value();
    sceneLayout->update();
    emit updatedScene();
}

void SceneLayoutEditor::on_m_resetDefaultLayoutButton_clicked()
{
    sceneLayout->resetLayout();
    update(*sceneLayout);
    emit updatedScene();
}

