#include "DisplayWidget.h"
#include "../Load/FileLoad.h"
#include "../Load/EXIFResolver.h"

#include <filesystem>
#include <mutex>

#include <QImage>
#include <QPixmap>

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include <QVBoxLayout>
#include <QResizeEvent>

namespace CM
{
    DisplayWidget::DisplayWidget(QWidget *parent)
    : QWidget(parent)
    , m_scene(new PreViewImageScene)
    , m_view (new QGraphicsView)
    , m_previewImageItem(new PreViewImageItem)
    {
        m_view->setScene(m_scene);
        connect(this,&DisplayWidget::Created,this, [ parent= this, view = m_view ]()
        {
            view->setParent(parent);
        });

        m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        m_view->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        m_scene->addItem(m_previewImageItem);
        ((PreViewImageScene*)(m_scene))->setPixmapItem(m_previewImageItem);

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
        // m_scene->clear();
        const auto& pictureData = CM::FileLoad::Load(path.string());
        EXIFResolver resolver;
        EXIFResolver::check(resolver.resolver(pictureData));
        const auto& result = resolver.getInfos();

        /// load image
        const QImage loadedImage = QImage::fromData(pictureData.data(), pictureData.size());
        auto preViewImage = QPixmap::fromImage(loadedImage);

        auto pixmapItem = dynamic_cast<PreViewImageItem*>(m_previewImageItem);
        pixmapItem->resetPixmap(preViewImage);

        auto infos = EXIFResolver::resolverImageExif(result);
        auto scene = dynamic_cast<PreViewImageScene*>(m_scene);
        scene->updateTexItems(infos);

    }

    void DisplayWidget::resizeEvent(QResizeEvent *event)
    {
        const auto windowSize = event->size();
        m_view->resize(windowSize);   ///< resize view

        ((PreViewImageScene *) m_scene)->updateSceneRect(m_view);

        /// update preview image size
        if(this->isActiveWindow())
        {
            ((PreViewImageItem *) m_previewImageItem)->update();
        }

        ((PreViewImageScene *) m_scene)->updateTexItems();

        QWidget::resizeEvent(event);
    }

    void DisplayWidget::saveScene(SceneIndex sceneIndex)
    {
        auto save = [](QGraphicsScene * scene){
            scene->clearSelection();                                                       // Selections would also render to the file
            QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
            image.fill(Qt::white);                                              // Start all pixels transparent

            QPainter painter(&image);
            scene->render(&painter);
            bool res = image.save("file_name.png");
            if(!res)
            {
                std::runtime_error("save scene failed!");
            }
        };

        switch (sceneIndex)
        {
            case SceneIndex::NONE:
                break;
            case SceneIndex::PREVIEW_SCENE:
            {
                save(m_scene);
            }
            break;
            default:
                break;
        }
    }
} // CM
