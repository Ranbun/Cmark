#include <CMark.h>

#include "DisplayWidget.h"

#include <Base/ImagePack.h>
#include <File/ImageProcess/ImageProcess.h>
#include <File/LogoManager.h>
#include <File/PictureManager.h>
#include <File/Resolver/EXIFResolver.h>
#include <Scene/PreViewImageScene.h>
#include <SceneLayoutSettings.h>
#include <QDateTime>
#include <QFileDialog>
#include <QGraphicsView>
#include <QImage>
#include <QResizeEvent>
#include <QString>
#include <QtConcurrent>
#include "Scene/LifeSizeImageScene.h"

namespace
{
auto ImageSaveDefaultName() -> QString
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    auto outputName = currentDateTime.toString("yyyy-MM-dd__HHHmmMssS");

    constexpr std::hash<std::string> nameGenerator;
    const auto nameCode = nameGenerator(outputName.toStdString());
    outputName = outputName + "__" + QString::number(nameCode);
    return {outputName};
}
}  // namespace

namespace CM
{
DisplayWidget::DisplayWidget(QWidget* parent)
    : QWidget(parent)
    , m_PreviewImageScene(new PreViewImageScene)
    , m_AddLogoScene(new LifeSizeImageScene)
    , m_View(new QGraphicsView)
    , m_currentScene(m_PreviewImageScene)
{
    qRegisterMetaType<std::string>("std::string");

    m_View->setScene(m_currentScene);
    connect(this, &DisplayWidget::sigCreated, this, [parent = this, view = m_View]() { view->setParent(parent); });

    m_View->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    m_View->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    m_View->setAlignment(Qt::AlignCenter);

    m_View->resize(960, 720);
    m_currentScene->setSceneRect(0, 0, m_View->rect().width(), m_View->rect().height());

    emit sigCreated();

    InitConnect();

    const auto current_scene = dynamic_cast<CScene*>(m_currentScene);
    current_scene->setImageOnlyMode(true);
}

void DisplayWidget::open(const std::string& path) const
{
    assert(this);
    /// TODO:: maybe we need do something!
}

void DisplayWidget::resizeEvent(QResizeEvent* event)
{
    const auto windowSize = event->size();
    m_View->resize(windowSize);  ///< resize view

    /// 设置视图显示的场景的大小
    /// 设置视图观察的场景的观察点
    {
        const auto bound = m_currentScene->sceneRect();
        m_View->setSceneRect(bound);  // 设置场景矩形
        m_View->fitInView(bound, Qt::KeepAspectRatio);
    }

    QWidget::resizeEvent(event);
}

void DisplayWidget::saveScene(const SceneIndex sceneIndex)
{
    auto saveAsFile = [](const std::shared_ptr<QImage>& image, const QString& filePath)
    { ImageProcess::save(image, filePath); };

    auto save = [this, saveAsFile](QGraphicsScene* scene)
    {
        if (!scene)
            return;

        scene->clearSelection();  // Selections would also render to the file

        const auto rect = scene->sceneRect();
        const auto iSize = rect.size().toSize();
        auto image = std::make_shared<QImage>(iSize, QImage::Format_ARGB32);

        image->fill(Qt::white);  // Start all pixels white

        QPainter painter(image.get());
        scene->render(&painter);
        painter.end();

        QFileDialog getFileDialog(this, tr("Save File"), "./" + ImageSaveDefaultName(),
                                  tr("Images (*.png);;Images (*.xpm);;Images (*.jpg);;All Files (*)"));
        getFileDialog.setOption(QFileDialog::DontUseCustomDirectoryIcons);

        auto fileName = ImageSaveDefaultName() + ".png";

        switch (getFileDialog.exec())
        {
            case QFileDialog::Accepted:
            {
                auto suffix = getFileDialog.selectedNameFilter();
                fileName = getFileDialog.selectedFiles().first();
                if (!fileName.isEmpty() && !suffix.isEmpty())
                {
                    suffix = suffix.split(QRegExp("[()*]"), QString::SplitBehavior::SkipEmptyParts).last();
                    fileName += suffix;
                }
            }
            break;

            case QFileDialog::Rejected:
            {
                return;
            }
            default:
                break;
        }

        std::thread saveImage(saveAsFile, image, fileName);
        saveImage.detach();
    };

    switch (sceneIndex)
    {
        case SceneIndex::None:
            break;
        case PreviewScene:
        {
            save(m_currentScene);
        }
        break;
        case GenerateLogoScene:
        {
            const auto logoScene = dynamic_cast<LifeSizeImageScene*>(m_AddLogoScene);
            logoScene->saveSceneAsImage(save);
        }
        break;
    }
}

void DisplayWidget::InitConnect()
{
    connect(this, &DisplayWidget::sigOpen, this, [this](const std::string& path) { open(path); }, Qt::QueuedConnection);

    connect(this, &DisplayWidget::sigShowImage,
            [this](const size_t code)
            {
                if (auto* previewScene = dynamic_cast<PreViewImageScene*>(m_currentScene))
                    emit previewScene->sigShowImage(code);
            });

    connect(&m_loadPreviewWatcher, &QFutureWatcher<size_t>::finished, this, &DisplayWidget::onLoadPreviewFinished);

    connect(this, &DisplayWidget::sigPreviewImage, this, &DisplayWidget::onPreviewImage);

    connect(this, &DisplayWidget::sigDisplayImageMode, this, &DisplayWidget::onDisplayImageMode);
}

void DisplayWidget::onLoadPreviewFinished()
{
    const auto fileIndexCode = m_loadPreviewWatcher.result();
    emit sigShowImage(fileIndexCode);
    emit sigShowPreviewItemProperty(m_showImagePath);
}

void DisplayWidget::onPreviewImage(const std::string& path)
{
    const auto future = QtConcurrent::run(
        [path]() -> size_t
        {
            using PictureManagerInterFace = CM::PictureManager;
            const auto fileIndexCode = ImageProcess::generateFileIndexCode(path);

            auto [w, h] = SceneLayoutSettings::fixPreViewImageSize();
            const auto data = ImageProcess::loadFile(QString::fromStdString(path));
            const ImagePack loadImagePack{fileIndexCode, data, path, std::make_shared<std::mutex>(), {w, h}};

            PictureManagerInterFace::loadImage(loadImagePack);
            EXIFResolver::resolver(loadImagePack);

            auto& logoMgr = LogoManager::instance();
            const auto cameraIndex =
                logoMgr.resolveCameraIndex(EXIFResolver::ExifItem(fileIndexCode, ExifKey::CameraMake));
            logoMgr.getCameraMakerLogo(cameraIndex);  ///< 懒加载 logo（getCameraMakerLogo 内部处理）

            return fileIndexCode;
        });

    m_showImagePath = path;
    m_loadPreviewWatcher.setFuture(future);
}

void DisplayWidget::onDisplayImageMode(const DisplayWidgetMode mode)
{
    if (mode == DisplayWidgetMode::IMAGE)
    {
        if (const auto current_scene = dynamic_cast<CM::PreViewImageScene*>(m_currentScene))
        {
            current_scene->setImageOnlyMode(true);
            current_scene->ForceUpdate();
        }
        else
        {
            const auto scene = dynamic_cast<CM::CScene*>(m_currentScene);
            scene->setImageOnlyMode(true);
        }
    }
    else
    {
        if (const auto current_scene = dynamic_cast<CM::PreViewImageScene*>(m_currentScene))
        {
            current_scene->setImageOnlyMode(false);
            current_scene->ForceUpdate();
        }
        else
        {
            const auto scene = dynamic_cast<CM::CScene*>(m_currentScene);
            scene->setImageOnlyMode(false);
        }
    }
}
}  // namespace CM
