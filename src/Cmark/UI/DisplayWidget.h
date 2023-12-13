#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <QWidget>
#include <iostream>
#include <filesystem>

class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;

namespace CM
{
    class PreViewImageScene;

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

        /**
         * @brief 打开空的工程
         * @param path 路径
         */
        void Open(const std::filesystem::path& path) const;

        /**
         * @brief 预览文件路径下的图片
         * @param path 文件路径
         */
        void PreViewImage(const std::filesystem::path & path);

        /**
         * @brief save scene as Image
         */
        void saveScene(SceneIndex sceneIndex);

    protected:
        void paintEvent(QPaintEvent * event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        /**
         * @brief 预览使用的场景
         */
        QGraphicsScene * m_previewImageScene{nullptr};

        /**
         * @brief 存储源尺寸大小的带logo的图片的场景
         */
        QGraphicsScene * m_addLogoScene{nullptr};

        /**
         * @brief 当前显示的视图
         */
        QGraphicsView * m_view{nullptr};

    signals:
        void Created();

        /// TODO: define signal to adjust Scene Layout


    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
