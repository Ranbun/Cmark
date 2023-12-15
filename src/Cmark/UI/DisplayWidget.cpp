#include <CMark.h>

#include "DisplayWidget.h"

#include "Loader/FileLoad.h"
#include "Loader/EXIFResolver.h"
#include "Scene/PreViewImageScene.h"
#include "Scene/PreViewImageItem.h"
#include "Scene/LifeSizeImageScene.h"

#include "LogoManager.h"

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
    DisplayWidget::DisplayWidget(QWidget * parent)
            : QWidget(parent)
            , m_previewImageScene(new PreViewImageScene)
            , m_addLogoScene(new LifeSizeImageScene)
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

            m_view->resize(640,480);
            m_previewImageScene->setSceneRect(0,0,m_view->rect().width(),m_view->rect().height());

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
        EXIFResolver resolver;
        auto loadFileIndex = resolver.resolver(path);

        QImage readerFile;
        readerFile.fill(Qt::transparent);
        auto readFileToImage = [&readerFile](const std::filesystem::path & path)
        {
            QImageReader reader(path.string().c_str());
            reader.setAutoTransform(true);
            readerFile = reader.read();
        };
        std::thread readImage(readFileToImage, path);

        const auto & [res,message] = EXIFResolver::check(resolver.checkCode(loadFileIndex));
        if(!res)
        {
            QMessageBox::about(this,"Warning",message.c_str());
            return ;
        }

        auto exifInfos = resolver.getExifInfo(loadFileIndex);

        /// 加载logo
        auto cameraIndex = LogoManager::resolverCameraIndex(exifInfos.lock()->Make);
        LogoManager::loadCameraLogo(cameraIndex);
        auto previewImageLogo = LogoManager::getCameraMakerLogo(cameraIndex);

        /// wait load image thread
        if(readImage.joinable())
        {
            readImage.join();
        }

        /// convert to QPixmap
        QPixmap preViewImage = QPixmap::fromImage(readerFile);

        /// get enable exif item
        auto infos = EXIFResolver::resolverImageExif(exifInfos);

        /// 设置预览场景显示的资源
        auto scene = dynamic_cast<PreViewImageScene *>(m_previewImageScene);
        {
            scene->resetPreviewImageTarget(preViewImage);
            scene->resetTexItemsPlainText(infos);
            scene->resetLogoPixmap(previewImageLogo, Nikon);
        }

        /// 设置单张图片存储的显示资源
        auto logoScene = dynamic_cast<LifeSizeImageScene *>(m_addLogoScene);
        logoScene->resetPreviewImageTarget(preViewImage);
        logoScene->resetTexItemsPlainText(infos);
        logoScene->resetLogoPixmap(previewImageLogo, Nikon);

        /// 构建一个resizeEvent make it to applyLayout all item
        auto revent = new QResizeEvent(this->size(),this->size());
        this->resizeEvent(revent);
        delete revent;
    }

    void DisplayWidget::resizeEvent(QResizeEvent *event)
    {
        const auto windowSize = event->size();
        m_view->resize(windowSize);   ///< resize view

        ((PreViewImageScene *) m_previewImageScene)->updateSceneRect(m_view, {});

        /// 设置视图显示的场景的大小
        /// 设置视图观察的场景的观察点
        {
            const auto bound = m_previewImageScene->itemsBoundingRect();
            m_view->setSceneRect(bound); // 设置场景矩形
            m_view->centerOn(bound.center());
        }
        /// make scene fit in view
        m_view->fitInView(m_previewImageScene->itemsBoundingRect(),Qt::KeepAspectRatio);

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
            scene->setSceneRect(bound);

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
            case SceneIndex::GENERATELOGO_SCENE:
            {
                auto logoScene = dynamic_cast<LifeSizeImageScene*>(m_addLogoScene);
                logoScene->saveSceneAsImage(save);
            }
            break;
            default:
                break;
        }
    }


} // CM
