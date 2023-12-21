#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <QWidget>
#include <filesystem>


class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;

class SceneLayoutEditor;

namespace CM
{
    class PreViewImageScene;
}

namespace  CM
{
    enum SceneIndex
    {
        NONE,
        PREVIEW_SCENE,
        GENERATELOGO_SCENE
    };
}

namespace CM
{
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
        void Open(const std::filesystem::path & path) const;

        /**
         * @brief 预览文件路径下的图片
         * @param path 文件路径
         */
        void PreViewImage(const std::filesystem::path & path);

        /**
         * @brief save scene as Image
         * @param sceneIndex 要存储的场景的索引
         */
        void saveScene(SceneIndex sceneIndex);

    protected:
        void paintEvent(QPaintEvent * event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:

        std::shared_ptr<SceneLayoutEditor> m_previewSceneLayoutSettingPanel;

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
        void PreViewLayoutSettingsPanel();

    private:
        void InitConnect();

    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
