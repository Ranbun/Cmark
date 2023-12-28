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
         * @param previewPixmap 显示的图片
         */
        void resetPixmap(const QPixmap& previewPixmap);

        /**
         * @brief 更新当前的pixmap item(更新pixmap的大小和在场景中的位置)
         */
        void applyLayout();

        /**
         * @brief 更新显示pixmap的大小
         */
        void updatePixmapSize();

        /**
         * @brief 更新pixmap的位置
         */
        void updatePixmapPosition();

        /**
         * @brief 图片的宽高比
         * @return float
         */
        [[nodiscard]] const float& imageRatio() const;

        /**
         * @brief 当前显示的目标 记录预览图片的元数据大小
         * @return QPixmap
         */
        [[nodiscard]] const QPixmap& target() const;

    protected:
        static QPixmap scaledPixmap(const QPixmap& image, int w, int h);

    private:


        QPixmap m_Pixmap;   ///< 显示的图片的原图

        std::weak_ptr<SceneLayoutSettings> m_SceneLayout;///< 当前的场景布局

        float m_ImageRatio{3.0 / 4.0f};  ///< 显示的图片的宽高比
    };
} // CM

#endif //CAMERAMARK_PREVIEWIMAGEITEM_H_
