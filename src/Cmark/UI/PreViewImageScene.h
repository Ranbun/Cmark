#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H
#define CAMERAMARK_PREVIEWIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include <CMark.h>

#include "Base/Type.h"
#include "Base/CPoint.h"
#include "SceneLayout.h"


namespace CM
{

    enum class showExifTexPositionIndex
    {
        left_top,
        left_bottom,
        right_top,
        right_bottom
    };

    /**
     * @brief 记录显示的Exif信息和它的布局位置
     */
    struct showExifInfo
    {
        showExifTexPositionIndex layout;            ///< 显示信息的位置
        std::vector<ExifKey> m_keys;                ///< 显示的信息 - 某一个位置可以有多条信息
        /// TODO: make it editor in widget
    };

    class PreViewImageScene : public QGraphicsScene
    {
    public:
        explicit PreViewImageScene(QObject * parent = nullptr);
        ~PreViewImageScene() override = default;

        /**
         * @brief 初始化场景显示的Item
         */
        void Init();

        /**
         * @brief 根据当前使用的视图更新场景的大小(此时场景和视图具有1对一关系)
         * @param view 当前使用的视图
         */
        void updateSceneRect(QGraphicsView *view, const QRect &newSceneRect);

        /**
         * @brief 当前显示的所有文字信息
         * @param exifInfoMap 所有的文字信息
         */
        void updateTexItems(const CM::ExifInfoMap & exifInfoMap);

        /**
         * @brief 更新所有文字的位置
         */
        void updateTexItemsPosition();

        /**
         * @brief 更新logo
         * @param logo logo的pixmap对象
         */
        void resetLogoPixmap(const QPixmap & logo);

        /**
         * @brief 更新logo的位置
         */
        void updateLogoPosition();

        /**
         * @brief 更新预览显示的图片
         * @param pixmap 图片对象
         */
        void resetPreviewImageTarget(const QPixmap & pixmap);

        /**
         * @brief 更新margin rect的位置与大小
         */
        void updateMarginItems();

        void updateSplitRect();

        /**
         * @brief 获取预览显示的图片的原图
         * @return 加载的图片的pixmap
         */
        const QPixmap & originalImageTarget();

    private:
        QGraphicsPixmapItem * m_showImageItem;  ///< 预览加载的图片
        QGraphicsPixmapItem * m_logoItem;       ///< 加载的logo

        QGraphicsRectItem * m_left{};
        QGraphicsRectItem * m_right{};
        QGraphicsRectItem * m_top{};
        QGraphicsRectItem * m_bottom{};

        QGraphicsRectItem * m_splitRectItem{};

        std::unordered_map<showExifTexPositionIndex,QGraphicsTextItem*> m_textItem;

        std::vector<showExifInfo> m_showInfos;  ///< 最终显示到屏幕上的Exif信息
        ExifInfoMap m_targetImageExifInfoLists;    ///< 解析的当前图片的所有Info信息

        SceneLayout m_sceneLayout;              ///< 记录场景的布局

    private:
        /**
         * @brief init text item
         */
        void InitTexItems();

        /**
         * @brief init logo item
         */
        void InitLogoItem();

        /**
         * @brief init preview image
         */
        void InitTargetImageItem();

        /**
         * @brief init scene margin
         */
        void InitMargin();

        /**
         * @brief  分割线
         */
        void InitSplitRect();

    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H
