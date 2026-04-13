#include "ImagePropertyDockWidget.h"

#include <Base/Type.h>
#include <File/ImageProcess/ImageProcess.h>
#include <File/Resolver/EXIFResolver.h>

#include <QLabel>
#include <QResizeEvent>
#include <QVBoxLayout>

namespace CM
{
namespace ToolFunc
{

QString ElidedFilePathStr(const QString &prefixStr, const QString &str, const QFont &font, const QSize &widgetSize)
{
    const auto w = widgetSize.width();

    const QFontMetrics metrics(font);
    const auto calcWordsW = QFontMetrics(font);
    const auto wordsW = calcWordsW.horizontalAdvance(prefixStr);
    auto elided = metrics.elidedText(str, Qt::ElideRight, w - wordsW);
    return elided;
}

}  // namespace ToolFunc

ImagePropertyDockWidget::ImagePropertyDockWidget(QWidget *parent)
    : QDockWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
    setFeatures(NoDockWidgetFeatures);
    setMinimumHeight(200);
    setFixedWidth(280);

    setWindowTitle("");
    setTitleBarWidget(new QWidget());

    init();
}

void ImagePropertyDockWidget::initConnect()
{
    QObject::connect(this, &ImagePropertyDockWidget::sigShowProperty, this,
                     [this](const std::string &fileName) { onSigShowProperty(fileName); });
}

void ImagePropertyDockWidget::initLayout()
{
    const auto vbox = new QVBoxLayout(nullptr);

    {
        /// file path
        const auto filePathHBox = new QHBoxLayout;
        filePathHBox->setSpacing(5);
        filePathHBox->setMargin(0);
        filePathHBox->addSpacing(5);
        filePathHBox->addWidget(m_FilePathTitleLabel);
        filePathHBox->addWidget(m_FilePathLabel);
        filePathHBox->addStretch(1);
        vbox->addItem(filePathHBox);
    }

    {
        const auto dimensionHBox = new QHBoxLayout;
        dimensionHBox->setSpacing(5);
        dimensionHBox->setMargin(0);
        dimensionHBox->addSpacing(5);
        dimensionHBox->addWidget(m_DimensionTitle);
        dimensionHBox->addWidget(m_Dimension);
        dimensionHBox->addStretch(1);
        vbox->addItem(dimensionHBox);
    }

    auto addExifRow = [vbox](const QString &title, QLabel *valueLabel)
    {
        const auto hbox = new QHBoxLayout;
        hbox->setSpacing(5);
        hbox->setMargin(0);
        hbox->addSpacing(5);
        hbox->addWidget(new QLabel(title));
        hbox->addWidget(valueLabel);
        hbox->addStretch(1);
        vbox->addItem(hbox);
    };
    addExifRow("Camera Make:", m_CameraMakeLabel);
    addExifRow("Camera Model:", m_CameraModelLabel);
    addExifRow("Focal Length:", m_FocalLengthLabel);
    addExifRow("F-Stop:", m_FStopLabel);
    addExifRow("Lens Model:", m_LensModelLabel);
    addExifRow("ISO:", m_ISOSpeedLabel);
    addExifRow("ShutterSpeed:", m_ShutterSpeedLabel);

    vbox->setSpacing(5);
    vbox->setMargin(0);
    vbox->addStretch(1);

    const auto w = new QWidget(this);
    w->setLayout(vbox);
    setWidget(w);
}

void ImagePropertyDockWidget::init()
{
    initUi();
    initLayout();
    initConnect();
}

void ImagePropertyDockWidget::initUi()
{
    m_FilePathTitleLabel = new QLabel("File Path: ");
    m_FilePathTitleLabel->setFixedWidth(66);
    m_FilePathLabel = new QLabel();
    m_FilePathLabel->setFixedWidth(180);
    m_DimensionTitle = new QLabel("Dimension: ");
    m_Dimension = new QLabel();
    m_CameraMakeLabel = new QLabel();
    m_CameraModelLabel = new QLabel();
    m_FocalLengthLabel = new QLabel();
    m_FStopLabel = new QLabel();
    m_LensModelLabel = new QLabel();
    m_ISOSpeedLabel = new QLabel();
    m_ShutterSpeedLabel = new QLabel();
}

void ImagePropertyDockWidget::resizeEvent(QResizeEvent *event)
{
    m_FilePathLabel->setMaximumWidth(width() - m_FilePathTitleLabel->maximumWidth());

    const auto currentSize = event->size();
    const auto res =
        ToolFunc::ElidedFilePathStr(m_FilePathTitleLabel->text(), m_FilePath, m_FilePathLabel->font(), currentSize);
    m_FilePathLabel->setText(res);

    QDockWidget::resizeEvent(event);
}

static QString exifValue(const ExifInfoMap &map, ExifKey key)
{
    const auto it = map.find(key);
    return it != map.end() ? QString::fromStdString(it->second) : QString("-");
}

void ImagePropertyDockWidget::onSigShowProperty(const std::string &path)
{
    m_FilePath = QString::fromStdString(path);

    const auto currentSize = this->size();
    const auto res =
        ToolFunc::ElidedFilePathStr(m_FilePathTitleLabel->text(), m_FilePath, m_FilePathLabel->font(), currentSize);
    m_FilePathLabel->setText(res);
    m_FilePathLabel->setToolTip(m_FilePath);

    const auto fileIndexCode = ImageProcess::generateFileIndexCode(path);
    const auto exifInfos = EXIFResolver::getExifInfo(fileIndexCode);

    std::string imageW{};
    if (exifInfos.count(ExifKey::ImageWidth) > 0)
    {
        imageW = exifInfos.at(ExifKey::ImageWidth);
    }
    std::string imageH{};
    if (exifInfos.count(ExifKey::ImageHeight) > 0)
    {
        imageH = exifInfos.at(ExifKey::ImageHeight);
    }
    m_Dimension->setText(QString::fromStdString(imageW + "x" + imageH));

    m_CameraMakeLabel->setText(exifValue(exifInfos, ExifKey::CameraMake));
    m_CameraModelLabel->setText(exifValue(exifInfos, ExifKey::CameraModel));
    m_FocalLengthLabel->setText(exifValue(exifInfos, ExifKey::FocalLength));
    m_FStopLabel->setText(exifValue(exifInfos, ExifKey::FStop));
    m_LensModelLabel->setText(exifValue(exifInfos, ExifKey::LensModel));

    m_ISOSpeedLabel->setText(exifValue(exifInfos, ExifKey::ISOSpeed));
    m_ShutterSpeedLabel->setText(exifValue(exifInfos, ExifKey::ShutterSpeed));
}

}  // namespace CM
