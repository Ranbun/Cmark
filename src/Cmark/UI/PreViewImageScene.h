#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H
#define CAMERAMARK_PREVIEWIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "../Base/Type.h"
#include "../Base/CPoint.h"

#include "SceneLayout.h"

#include <algorithm>

namespace CM
{
    using Size = CPoint;

    struct ImageInfoItemPack
    {
        CM::ExifKey m_key;                   ///< info's index
        std::string m_title;                 ///< add it pre in info
        QGraphicsTextItem * m_field;         ///< Items
        CPoint pos{-1, -1};            ///< position
        std::string m_infos{};               ///< show infos
        bool m_visible{false};               ///< item's visibility
    };

    enum class ExifLayout
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
        ExifLayout layout;   ///< 显示信息的位置
        std::vector<ExifKey> m_keys;  ///< 显示的信息 - 某一个位置可以有多条信息
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
        void updateSceneRect(QGraphicsView *view, const QRectF &sceneSize);

        /**
         * @brief 当前显示的所有文字信息
         * @param map 所有的文字信息
         */
        void updateTexItems(const CM::ExifList & map);

        /**
         * @brief 更新所有文字的位置
         */
        void updateTexItems();

        /**
         * @brief 更新logo
         * @param logo logo的pixmap对象
         */
        void updateLogoPixmap(const QPixmap & logo);

        /**
         * @brief 更新logo的位置
         */
        void updateLogoPos();

        /**
         * @brief 更新预览显示的图片
         * @param pixmap 图片对象
         */
        void updatePreviewPixmap(const QPixmap & pixmap);

        /**
         * @brief 存储加载的图片
         */
        void saveLoadedPixmap();

    private:
        QGraphicsPixmapItem * m_showImageItem;
        QGraphicsPixmapItem * m_logoItem;

        QGraphicsRectItem * m_left;
        QGraphicsRectItem * m_right;
        QGraphicsRectItem * m_top;
        QGraphicsRectItem * m_bottom;

        std::vector<ImageInfoItemPack> m_infos;
        std::vector<showExifInfo> m_showInfos;

        SceneLayout m_sceneLayout;  ///< 记录场景的布局


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
        void InitPreviewImageItem();

        /**
         * @brief init scene margin
         */
        void InitMargin();


    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H
