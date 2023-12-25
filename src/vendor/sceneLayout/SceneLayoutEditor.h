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


class SceneLayoutEditor final : public QDialog
{
    Q_OBJECT
public:
    explicit SceneLayoutEditor(QWidget* parent = nullptr);

    SceneLayoutEditor(const SceneLayoutEditor& other) = delete;
    SceneLayoutEditor(const SceneLayoutEditor && other) = delete;

    SceneLayoutEditor& operator=(const SceneLayoutEditor&) = delete;
    SceneLayoutEditor& operator=(const SceneLayoutEditor&&) = delete;

    void show(const std::shared_ptr<CM::SceneLayoutSettings>& setting);
    void update(const std::shared_ptr<CM::SceneLayoutSettings>& setting);

    ~SceneLayoutEditor() override;

protected:

    void InitConnect();


private slots:
    void on_m_rightMarginSlider_valueChanged(int value);
    void on_m_LeftMarginSlider_valueChanged(int value);
    void on_m_topMarginSlider_valueChanged(int value);
    void on_m_bottomMarginSlider_valueChanged(int value);

    void on_m_resetDefaultLayoutButton_clicked();

signals:  
    void updatedScene();
    void sigShowLayoutSettingPanel(const std::shared_ptr<CM::SceneLayoutSettings>& setting);

private:
    Ui::SceneLayout* ui;
    std::shared_ptr<CM::SceneLayoutSettings> m_SceneLayout;
};
#endif // DIALOG_H
