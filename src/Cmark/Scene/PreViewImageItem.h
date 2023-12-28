#ifndef CAMERAMARK_PREVIEWIMAGEITEM_H_
#define CAMERAMARK_PREVIEWIMAGEITEM_H_

#include "Scene/PreViewImageScene.h"
#include "SceneLayoutSettings.h"

#include <QGraphicsPixmapItem>

namespace CM
{
    class PreViewImageItem final : public QGraphicsPixmapItem
    {
    public:
        explicit PreViewImageItem(QGraphicsItem* parent, const std::shared_ptr<SceneLayoutSettings>& layout);

        PreViewImageItem(const PreViewImageItem& other) = delete;
        PreViewImageItem(const PreViewImageItem&& other) = delete;

        PreViewImageItem & operator=(const PreViewImageItem& other) = delete;
        PreViewImageItem& operator=(const PreViewImageItem&& other) = delete;

        ~PreViewImageItem() override;

        /**
         * @brief 重新设置显示的图片
         * @param imageIndex 图片的索引
         */
        void resetPixmap(size_t imageIndex);

        /**
         * @brief 更新当前的pixmap item(更新pixmap的大小和在场景中的位置)
         */
        void applyLayout(const std::shared_ptr<SceneLayoutSettings>& layout);

        /**
         * @brief 更新显示pixmap的大小
         */
        void resizeImage(int w, int h);

        /**
         * @brief 图片的宽高比
         * @return float
         */
        [[nodiscard]] [[maybe_unused]]  const float& imageRatio() const;


        bool showSplitLine();

    signals:
        void sigShowSplitLine(bool val);


    protected:
        static QPixmap scaledPixmap(const QPixmap& image, int w, int h);
        static QPixmap scaledPixmap(const std::shared_ptr<QPixmap>& image, int w, int h);

        /**
         * @brief 获取当前显示的图片的QPixmap对象
         * @return QPixmap
         */
        [[nodiscard]] [[maybe_unused]] std::shared_ptr<QPixmap> target() const;

        /**
         * @brief 更新记录的图片的宽高比
         * @param tar 显示的图片
         */
        [[maybe_unused]] void updateImageRatio(const std::shared_ptr<QPixmap>& tar);

    private:
        std::weak_ptr<SceneLayoutSettings> m_SceneLayout;///< 当前的场景布局
        float m_ImageRatio{3.0 / 4.0f};  ///< 显示的图片的宽高比
    };
} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H_
