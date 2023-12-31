#include <CMark.h>

#include "DisplayWidget.h"

#include "Loader/EXIFResolver.h"
#include "Scene/LifeSizeImageScene.h"
#include "Scene/PreViewImageScene.h"

#include "sources/PictureManager.h"

#include <SceneLayoutEditor.h>
#include "sources/LogoManager.h"

#include <QFileDialog>
#include <QGraphicsView>
#include <QImage>
#include <QMessageBox>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QDateTime>

namespace CM
{
    namespace
    {
        QString ImageSaveDefaultName()
        {
            const QDateTime currentDateTime = QDateTime::currentDateTime();
            auto outputName = currentDateTime.toString("yyyy-MM-dd__HHHmmMssS");

            constexpr std::hash<std::string> nameGenerator;
            const auto nameCode  = nameGenerator(outputName.toStdString());
            outputName = outputName + "__" + QString::number(nameCode);
            return {outputName};
        }



    }

    DisplayWidget::DisplayWidget(QWidget* parent)
        : QWidget(parent)
          , m_PreviewSceneLayoutSettingPanel(std::make_shared<SceneLayoutEditor>())
          , m_PreviewImageScene(new PreViewImageScene)
          , m_AddLogoScene(new LifeSizeImageScene)
          , m_View(new QGraphicsView)
    {
        m_PreviewSceneLayoutSettingPanel->setHidden(true);

        m_View->setScene(m_PreviewImageScene);
        connect(this, &DisplayWidget::sigCreated, this, [ parent= this, view = m_View ]()
        {
            view->setParent(parent);
        });

        m_View->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_View->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_View->setAlignment(Qt::AlignCenter);

        m_View->resize(960, 720);
        m_PreviewImageScene->setSceneRect(0, 0, m_View->rect().width(), m_View->rect().height());

        emit sigCreated();

        InitConnect();
    }

    void DisplayWidget::open(const std::filesystem::path& path) const
    {
        assert(this);
    }

    void DisplayWidget::paintEvent(QPaintEvent* event)
    {
        QWidget::paintEvent(event);
    }

    void DisplayWidget::preViewImage(const std::filesystem::path& path)
    {
        using PictureManagerInterFace = CM::PictureManager;
        EXIFResolver resolver;

        /// load image
        const auto imageIndexCode = PictureManagerInterFace::loadImage(path.string());

        const auto imageExifInfoIndex = resolver.resolver(path);
        /// check resolver result
        const auto& [res,message] = EXIFResolver::check(resolver.checkCode(imageExifInfoIndex));
        if (!res)
        {
            QMessageBox::about(this, "Warning", message.c_str());
            return;
        }

        /// get resolved image infos
        const auto exifInfos = resolver.getExifInfo(imageExifInfoIndex);

        /// 加载logo
        const auto cameraIndex = LogoManager::resolverCameraIndex(exifInfos.lock()->Make);
        LogoManager::loadCameraLogo(cameraIndex);
        const auto previewImageLogo = LogoManager::getCameraMakerLogo(cameraIndex);

        /// get loaded image
        const auto preViewImage = PictureManager::getImage(imageIndexCode);

        /// get enable exif item
        const auto infos = EXIFResolver::resolverImageExif(exifInfos);

        /// 设置预览场景显示的资源
        {
            const auto scene = dynamic_cast<PreViewImageScene*>(m_PreviewImageScene);
            scene->resetStatus();
            scene->resetPreviewImageTarget(*preViewImage, imageIndexCode);
            scene->resetTexItemsPlainText(infos);
            scene->resetLogoPixmap(previewImageLogo, cameraIndex);
        }

        /// 设置单张图片存储的显示资源
        {
            const auto logoScene = dynamic_cast<LifeSizeImageScene*>(m_AddLogoScene);
            logoScene->resetStatus();
            logoScene->resetPreviewImageTarget(*preViewImage, imageIndexCode);
            logoScene->resetTexItemsPlainText(infos);
            logoScene->resetLogoPixmap(previewImageLogo, cameraIndex);
        }

        /// 构建一个resizeEvent make it to applyLayout all item
        /// TODO: we need update scene in here
        const auto rEvent = new QResizeEvent(this->size(), this->size());
        this->resizeEvent(rEvent);
        delete rEvent;

    }

    void DisplayWidget::resizeEvent(QResizeEvent* event)
    {
        const auto windowSize = event->size();
        m_View->resize(windowSize); ///< resize view

        dynamic_cast<PreViewImageScene*>(m_PreviewImageScene)->updateSceneRect();

        /// 设置视图显示的场景的大小
        /// 设置视图观察的场景的观察点
        {
            const auto bound = m_PreviewImageScene->sceneRect();
            m_View->setSceneRect(bound); // 设置场景矩形
            m_View->fitInView(bound, Qt::KeepAspectRatio);
        }
        QWidget::resizeEvent(event);
    }

    void DisplayWidget::saveScene(const SceneIndex sceneIndex)
    {
        auto saveAsFile = [](const std::shared_ptr<QImage>& image, const QString& filePath)
        {
            const QFileInfo fileInfo(filePath);
            const auto suffix = fileInfo.suffix();

            if (const bool res = image->save(filePath, suffix.toStdString().c_str());
                !res)
            {
                throw std::runtime_error("save scene failed!");
            }

            std::cout << "Image Save success!" << std::endl;
        };

        auto save = [this,saveAsFile](QGraphicsScene* scene)
        {
            if (!scene) return;

            scene->clearSelection(); // Selections would also render to the file

            const auto rect = scene->sceneRect();
            const auto iSize = rect.size().toSize();
            auto image = std::make_shared<QImage>(iSize, QImage::Format_ARGB32);

            image->fill(Qt::white); // Start all pixels white

            QPainter painter(image.get());
            scene->render(&painter);
            painter.end();

            QFileDialog getFileDialog(this, tr("Save File"),
                                "./" + ImageSaveDefaultName(),
                                      tr("Images (*.png);;Images (*.xpm);;Images (*.jpg);;All Files (*)"));
            getFileDialog.setOption(QFileDialog::DontUseCustomDirectoryIcons);

            auto fileName = ImageSaveDefaultName() + ".png";


            switch (getFileDialog.exec())
            {
                case  QFileDialog::Accepted:
                    {
                        auto suffix = getFileDialog.selectedNameFilter();
                        fileName = getFileDialog.selectedFiles().first();
                        if (!fileName.isEmpty() && !suffix.isEmpty())
                        {
                            suffix = suffix.split(QRegExp("[()*]"), Qt::SkipEmptyParts).last();
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
                save(m_PreviewImageScene);
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
        /// TODO: maybe remove it
        connect(m_PreviewSceneLayoutSettingPanel.get(), &SceneLayoutEditor::updatedScene, [this]()
        {
            const auto scene = dynamic_cast<CScene*>(m_PreviewImageScene);
            scene->applyLayout(nullptr);
        });

        connect(this, &DisplayWidget::sigPreViewLayoutSettingsPanel, [this]()
        {
            const auto layoutSettings = dynamic_cast<PreViewImageScene*>(m_PreviewImageScene)->layoutSettings();
            emit m_PreviewSceneLayoutSettingPanel->sigShowLayoutSettingPanel(layoutSettings);
        });

        connect(this, &DisplayWidget::sigOpen, this, [this](const std::string& path)
        {
            open(path);
        }, Qt::QueuedConnection);


        connect(this, &DisplayWidget::sigPreViewImage, [this](const std::string& filePath)
        {
            preViewImage(filePath);
        });
    }
} // CM
