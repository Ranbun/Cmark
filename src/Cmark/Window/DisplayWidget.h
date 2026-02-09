#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <QWidget>
#include <QFutureWatcher>

class QGraphicsView;
class QGraphicsScene;

namespace CM
{
    class PreViewImageScene;
}

namespace CM
{
    enum SceneIndex
    {
        None,
        PreviewScene,
        GenerateLogoScene
    };
}

namespace CM
{
    class DisplayWidget final : public QWidget
    {
        Q_OBJECT

    public:
        explicit DisplayWidget(QWidget* parent = nullptr);

    public:
        /**
         * @brief 打开空的工程
         * @param path 路径
         * @note connect with sigOpen
         */
        void open(const std::string& path) const;

        /**
         * @brief save scene as Image
         * @param sceneIndex 要存储的场景的索引
         */
        void saveScene(SceneIndex sceneIndex);

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        QGraphicsScene* m_PreviewImageScene{nullptr}; ///< 预览使用的场景
        QGraphicsScene* m_currentScene{nullptr};      ///< 预览使用的场景

        QGraphicsScene* m_AddLogoScene{nullptr};  ///< 存储源尺寸大小的带logo的图片的场景
        QGraphicsView* m_View{nullptr};           ///< 当前显示的视图

        QFutureWatcher<size_t> m_loadPreviewWatcher; ///< watcher for load preview image

        std::string m_showImagePath;

    signals:
        void sigOpen(const std::string & path);
        void sigCreated();
        /**
         * @brief show image
         *
         * @param fileIndexCode generate by filename as code
         */
        void sigShowImage(size_t fileIndexCode);
        void sigPreviewImage(const std::string& filePath);
        void sigShowPreviewItemProperty(const std::string & showImagePath);

    private slots:
        /**
         * @brief in file loaded
         *
         */
        void onLoadPreviewFinished();

        /**
         * @brief deal sigPreviewImage signal
         *
         * @param path file path
         */
        void onPreviewImage(const std::string& path);

    private:
        void InitConnect();

    };
} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
