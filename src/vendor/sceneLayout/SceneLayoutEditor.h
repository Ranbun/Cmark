#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "SceneLayoutSettings.h"

QT_BEGIN_NAMESPACE

namespace Ui
{
    class SceneLayout;
}

QT_END_NAMESPACE


class SceneLayoutEditor : public QDialog
{
    Q_OBJECT
public:
    SceneLayoutEditor(QWidget* parent = nullptr);
    void show(CM::SceneLayoutSettings& setting);
    void update(CM::SceneLayoutSettings& setting);

    ~SceneLayoutEditor() override;

private slots:
    void on_m_rightMarginSlider_valueChanged(int value);
    void on_m_LeftMarginSlider_valueChanged(int value);
    void on_m_topMarginSlider_valueChanged(int value);
    void on_m_bottomMarginSlider_valueChanged(int value);

    void on_m_resetDefaultLayoutButton_clicked();

signals:
    void updatedScene();

private:
    Ui::SceneLayout* ui;
    CM::SceneLayoutSettings* sceneLayout{nullptr};
};
#endif // DIALOG_H
