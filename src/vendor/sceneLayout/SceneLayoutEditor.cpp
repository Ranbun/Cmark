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

void SceneLayoutEditor::show(const std::shared_ptr<CM::SceneLayoutSettings>& setting)
{
    m_SceneLayout = setting;

    update(setting);

    QWidget::show();
}

void SceneLayoutEditor::update(const std::shared_ptr<CM::SceneLayoutSettings>& setting)
{
    m_SceneLayout = setting;

    const auto& [l, r, t, b] = m_SceneLayout->getMargin();
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
    auto& [l,r,t,b] = m_SceneLayout->getMargin();
    l = ui->m_LeftMarginSlider->value();
    m_SceneLayout->update();
    emit updatedScene();
}


void SceneLayoutEditor::on_m_rightMarginSlider_valueChanged(const int value)
{
    ui->m_rightMargibValueLabel->setText(QString::number(value));
    auto& [l, r, t, b] = m_SceneLayout->getMargin();
    r = ui->m_rightMarginSlider->value();
    m_SceneLayout->update();
    emit updatedScene();
}


void SceneLayoutEditor::on_m_topMarginSlider_valueChanged(const int value)
{
    ui->m_topMarginValueLabel->setText(QString::number(value));
    auto& [l, r, t, b] = m_SceneLayout->getMargin();
    t = ui->m_topMarginSlider->value();
    m_SceneLayout->update();
    emit updatedScene();
}


void SceneLayoutEditor::on_m_bottomMarginSlider_valueChanged(const int value)
{
    ui->m_bottomMarginValueLabel->setText(QString::number(value));
    auto& [l, r, t, b] = m_SceneLayout->getMargin();
    b = ui->m_bottomMarginSlider->value();
    m_SceneLayout->update();
    emit updatedScene();
}

void SceneLayoutEditor::on_m_resetDefaultLayoutButton_clicked()
{
    m_SceneLayout->resetLayout();
    update(m_SceneLayout);
    emit updatedScene();
}

