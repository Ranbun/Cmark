#ifndef CAMERAMARK_IMAGEPROPERTYDOCKWIDGET_H
#define CAMERAMARK_IMAGEPROPERTYDOCKWIDGET_H

#include <QDockWidget>
#include <QLabel>

namespace CM
{
class ImagePropertyDockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit ImagePropertyDockWidget(QWidget* parent = nullptr);

    ImagePropertyDockWidget(const ImagePropertyDockWidget&) = delete;
    ImagePropertyDockWidget(ImagePropertyDockWidget&&) = delete;

    ImagePropertyDockWidget& operator=(const ImagePropertyDockWidget&) = delete;
    ImagePropertyDockWidget& operator=(ImagePropertyDockWidget&&) = delete;

    ~ImagePropertyDockWidget() override = default;

signals:
    void sigShowProperty(const std::string& path);

protected:
    void resizeEvent(QResizeEvent* event) override;

private slots:
    void onSigShowProperty(const std::string& path);

private:
    void init();
    void initUi();
    void initLayout();
    void initConnect();

private:
    QLabel* m_FilePathTitleLabel{};
    QLabel* m_FilePathLabel{};
    QLabel* m_DimensionTitle{};
    QLabel* m_Dimension{};
    QLabel* m_CameraMakeLabel{};
    QLabel* m_CameraModelLabel{};
    QLabel* m_FocalLengthLabel{};
    QLabel* m_FStopLabel{};
    QLabel* m_LensModelLabel{};

    QLabel* m_ShutterSpeedLabel{};
    QLabel* m_ISOSpeedLabel{};

    QString m_FilePath{};
};

}  // namespace CM

#endif  // CAMERAMARK_IMAGEPROPERTYDOCKWIDGET_H
