#include "DisplayWidget.h"
#include "../Load/FileLoad.h"
#include "../Load/EXIFResolver.h"

#include <filesystem>
#include <mutex>

#include <QImage>
#include <QPixmap>
#include <QFileDialog>

#include <QGraphicsView>
#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include <QVBoxLayout>
#include <QResizeEvent>
#include "LogoManager.h"

namespace CM
{
    DisplayWidget::DisplayWidget(QWidget *parent)
    : QWidget(parent)
    , m_previewImageScene(new PreViewImageScene)
    , m_addLogoScene(new PreViewImageScene)
    , m_view (new QGraphicsView)
    {
        m_view->setScene(m_previewImageScene);
        connect(this,&DisplayWidget::Created,this, [ parent= this, view = m_view ]()
        {
            view->setParent(parent);
        });

        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_view->setAlignment(Qt::AlignCenter);

        emit Created();
    }

    void DisplayWidget::Open(const std::filesystem::path& path) const
    {
        assert(this);
    }

    void DisplayWidget::paintEvent(QPaintEvent *event)
    {
        QWidget::paintEvent(event);
    }

    void DisplayWidget::PreViewImage(const std::filesystem::path & path)
    {
        const auto &pictureData = CM::FileLoad::Load(path.string());
        EXIFResolver resolver;
        EXIFResolver::check(resolver.resolver(pictureData));
        const auto &result = resolver.getInfos();

        auto cameraIndex = LogoManager::resolverCameraIndex(result.Make);
        LogoManager::loadCameraLogo(cameraIndex);
        auto previewImagelogo = LogoManager::getCameraMakerLogo(cameraIndex);

        /// load image
        const QImage loadedImage = QImage::fromData(pictureData.data(), pictureData.size());
        auto preViewImage = QPixmap::fromImage(loadedImage);

        auto infos = EXIFResolver::resolverImageExif(result);
        auto scene = dynamic_cast<PreViewImageScene *>(m_previewImageScene);

        scene->updatePreviewPixmap(preViewImage);
        scene->updateTexItems(infos);
        scene->updateLogoPixmap(*previewImagelogo);

#if _DEBUG >> 1
        auto logoScene = dynamic_cast<PreViewImageScene *>(m_addLogoScene);
        logoScene->setSceneRect(0,0,result.ImageWidth,result.ImageHeight);
        logoScene->updatePreviewPixmap(preViewImage);
        scene->updateTexItems(infos);
        scene->updateLogoPixmap(*previewImagelogo);
#endif
    }

    void DisplayWidget::resizeEvent(QResizeEvent *event)
    {
        const auto windowSize = event->size();
        m_view->resize(windowSize);   ///< resize view
        ((PreViewImageScene *) m_previewImageScene)->updateSceneRect(m_view, m_previewImageScene->itemsBoundingRect());

        {
            const auto bound = m_previewImageScene->itemsBoundingRect();
            const auto rect = m_previewImageScene->sceneRect();
            const auto newRect = QRectF(0,0,rect.width(),bound.height());
            m_view->setSceneRect(newRect); // 设置场景矩形
            m_view->centerOn(newRect.center());
        }
        QWidget::resizeEvent(event);
    }

    void DisplayWidget::saveScene(SceneIndex sceneIndex)
    {
        auto saveAsFile = [](const std::shared_ptr<QImage>& image,const QString & filePath)
        {
            bool res = image->save(filePath);
            if(!res)
            {
                std::runtime_error("save scene failed!");
            }
            else
            {
                std::cout << "Image Save success!" << std::endl;
            }
        };

        auto save = [this,saveAsFile](QGraphicsScene * scene)
        {
            if(!scene) return ;

            scene->clearSelection();                                                       // Selections would also render to the file

            const auto rect = scene->sceneRect();
            const auto bound = scene->itemsBoundingRect();
            scene->setSceneRect({0,0,rect.width(),bound.height()});

            std::shared_ptr<QImage> image = std::make_shared<QImage>(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);

            image->fill(Qt::white);   // Start all pixels white

            QPainter painter(image.get());
            scene->render(&painter);
            painter.end();

            auto fileName = QFileDialog::getSaveFileName(this,tr("Save File"),
                                                         "./untitled.png",
                                                         tr("Images (*.png *.xpm *.jpg)"));

            std::thread saveImage(saveAsFile,image,fileName);
            saveImage.detach();

            scene->setSceneRect(rect);

        };

        switch (sceneIndex)
        {
            case SceneIndex::NONE:
                break;
            case SceneIndex::PREVIEW_SCENE:
            {
                save(m_previewImageScene);
            }
            break;
            case SceneIndex::GENREATELOGO_SCENE:
            {
                save(m_addLogoScene);
            }
            break;
            default:
                break;
        }
    }
} // CM
