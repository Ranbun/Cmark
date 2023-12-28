#include <CMark.h>

#include "DisplayWidget.h"

#include "Loader/FileLoad.h"
#include "Loader/EXIFResolver.h"
#include "Scene/PreViewImageScene.h"
#include "Scene/PreViewImageItem.h"
#include "Scene/LifeSizeImageScene.h"

#include "Core/PictureManager.h"

#include "LogoManager.h"
#include <SceneLayoutEditor.h>

#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QResizeEvent>

namespace CM
{
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

        m_View->resize(640, 480);
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
        EXIFResolver resolver;
        const auto loadFileIndex = resolver.resolver(path);

        QImage readerFile;
        readerFile.fill(Qt::transparent);
        auto readFileToImage = [&readerFile](const std::filesystem::path& path)
        {
            QImageReader reader(path.string().c_str());
            reader.setAutoTransform(true);
            readerFile = reader.read();
        };
        std::thread readImage(readFileToImage, path);

        const auto& [res,message] = EXIFResolver::check(resolver.checkCode(loadFileIndex));
        if (!res)
        {
            QMessageBox::about(this, "Warning", message.c_str());
            return;
        }

        const auto exifInfos = resolver.getExifInfo(loadFileIndex);

        /// 加载logo
        const auto cameraIndex = LogoManager::resolverCameraIndex(exifInfos.lock()->Make);
        LogoManager::loadCameraLogo(cameraIndex);
        const auto previewImageLogo = LogoManager::getCameraMakerLogo(cameraIndex);

        /// wait load image thread
        if (readImage.joinable())
        {
            readImage.join();
        }

        /// convert to QPixmap
        const std::shared_ptr<QPixmap> preViewImage = std::make_shared<QPixmap>(std::move(QPixmap::fromImage(readerFile)));


        constexpr std::hash<std::shared_ptr<QPixmap>> hasher;
        const auto imageIndexCode = hasher(preViewImage);

        PictureManager::insert(std::pair<size_t,std::shared_ptr<QPixmap>>{ imageIndexCode,preViewImage });

        /// get enable exif item
        const auto infos = EXIFResolver::resolverImageExif(exifInfos);

        /// 设置预览场景显示的资源
        {
            const auto scene = dynamic_cast<PreViewImageScene*>(m_PreviewImageScene);
            scene->resetPreviewImageTarget(*preViewImage, imageIndexCode);
            scene->resetTexItemsPlainText(infos);
            scene->resetLogoPixmap(previewImageLogo, CameraIndex::Nikon);
        }

        /// 设置单张图片存储的显示资源
        const auto logoScene = dynamic_cast<LifeSizeImageScene*>(m_AddLogoScene);
        logoScene->resetPreviewImageTarget(*preViewImage, imageIndexCode);
        logoScene->resetTexItemsPlainText(infos);
        logoScene->resetLogoPixmap(previewImageLogo, CameraIndex::Nikon);

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

        dynamic_cast<PreViewImageScene*>(m_PreviewImageScene)->updateSceneRect(m_View, {});

        /// 设置视图显示的场景的大小
        /// 设置视图观察的场景的观察点
        {
            const auto bound = m_PreviewImageScene->itemsBoundingRect();
            m_View->setSceneRect(bound); // 设置场景矩形
            m_View->centerOn(bound.center());
        }
        /// make scene fit in view
        m_View->fitInView(m_PreviewImageScene->itemsBoundingRect(), Qt::KeepAspectRatio);

        QWidget::resizeEvent(event);
    }

    void DisplayWidget::saveScene(const SceneIndex sceneIndex)
    {
        auto saveAsFile = [](const std::shared_ptr<QImage>& image, const QString& filePath)
        {
            if (const bool res = image->save(filePath);
                !res)
            {
                throw std::runtime_error("save scene failed!");
            }
            else
            {
                std::cout << "Image Save success!" << std::endl;
            }
        };

        auto save = [this,saveAsFile](QGraphicsScene* scene)
        {
            if (!scene) return;

            scene->clearSelection(); // Selections would also render to the file

            const auto rect = scene->sceneRect();
            const auto bound = scene->itemsBoundingRect();
            scene->setSceneRect(bound);

            auto image = std::make_shared<QImage>(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);

            image->fill(Qt::white); // Start all pixels white

            QPainter painter(image.get());
            scene->render(&painter);
            painter.end();

            auto fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                         "./untitled.png",
                                                         tr("Images (*.png *.xpm *.jpg)"));

            std::thread saveImage(saveAsFile, image, fileName);
            saveImage.detach();

            scene->setSceneRect(rect);
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
        default:
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
