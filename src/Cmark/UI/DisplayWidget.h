#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <filesystem>
#include <QWidget>

#include <iostream>

class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;

namespace CM
{
    enum SceneIndex
    {
        NONE,
        PREVIEW_SCENE,
        GENERATELOGO_SCENE
    };

    class DisplayWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit DisplayWidget(QWidget * parent = nullptr);
    public:
        void Open(const std::filesystem::path& path) const;
        void PreViewImage(const std::filesystem::path & path);

        /**
         * @brief save scene as Image
         */
        void saveScene(SceneIndex sceneIndex);

        [[deprecated]] void saveLoadedPixmap();

    protected:
        void paintEvent(QPaintEvent * event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        QGraphicsScene * m_previewImageScene{nullptr};
        QGraphicsScene * m_addLogoScene{nullptr};
        QGraphicsView * m_view{nullptr};

    signals:
        void Created();

    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
