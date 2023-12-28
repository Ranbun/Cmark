#ifndef CAMERAMARK_CSCENE_H
#define CAMERAMARK_CSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "Base/Type.h"
#include "SceneLayoutSettings.h"

#include "UI/LogoManager.h"

namespace CM
{
    class PreViewImageItem;
}

namespace CM
{
    enum class ShowExifTexPositionIndex
    {
        LeftTop,
        LeftBottom,
        RightTop,
        RightBottom
    };

    /**
     * @brief 记录显示的Exif信息和它的布局位置
     */
    struct ShowExifInfo
    {
        ShowExifTexPositionIndex m_Layout; ///< 显示信息的位置
        std::vector<ExifKey> m_Keys; ///< 显示的信息 - 某一个位置可以有多条信息
        /// TODO: make it editor in widget
    };

    enum class GraphicsItemDataIndex : uint8_t
    {
        CameraIndex,
        PixmapIndex
    };

    class CScene : public QGraphicsScene
    {
    public:
        explicit CScene(QObject* parent = nullptr);

        CScene(const CScene& scene) = delete;
        CScene(const CScene&& scene) = delete;

        CScene & operator=(const CScene& scene) = delete;
        CScene& operator=(const CScene&& scene) = delete;

        CScene(CScene&& scene) = delete;

        ~CScene() override;

        /**
         * @brief 当前显示的所有文字信息
         * @param exifInfoMap 所有的文字信息
         */
        void resetTexItemsPlainText(const CM::ExifInfoMap& exifInfoMap);

        /**
         * @brief 更新logo
         * @param logo logo的pixmap对象
         * @param cameraIndex 相机的Logo索引
         */
        void resetLogoPixmap(const std::shared_ptr<QPixmap>& logo, CameraIndex cameraIndex = CameraIndex::None);

        /**
         * @brief 更新预览显示的图片
         * @param pixmap 图片对象
         * @param imageIndexCode 设置的图片的索引 通过索引可以获取图片
         */
        void resetPreviewImageTarget(const QPixmap& pixmap, size_t imageIndexCode) const;

        /**
         * @brief 根据布局更新场景中所有item
         * @param layout 设置的布局
         */
        void applyLayout(const std::shared_ptr<SceneLayoutSettings>& layout = nullptr);

        /**
         * @brief applyLayout scene  layout
         */
        virtual void updateLayout();

        /**
         * @brief 获取当前窗口布局文件
         * @return shared_otr
         */
        std::shared_ptr<SceneLayoutSettings> layoutSettings() const;

    protected:
        /**
         * @brief 更新显示的图片的信息
         */
        virtual void updateShowImage();

        /**
         * @brief 更新logo的位置
         */
        virtual void updateLogoPosition();

        /**
         * @brief 更新所有文字的位置
         */
        virtual void updateTexItemsPosition();

        /**
         * @brief 更新右侧显示的文本与logo之间的分割线
         */
        virtual void updateSplitRect();

        /**
         * @brief 更新margin rect的位置与大小
         */
        virtual void updateMarginItems();

    private:
        /**
         * @brief 初始化场景显示的Item
         */
        void Init();

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

    protected:
        PreViewImageItem* m_ShowImageItem{}; ///< 预览加载的图片Item对象
        QGraphicsPixmapItem* m_LogoItem{}; ///< 加载的logo Item对象

        QGraphicsRectItem* m_Left{}; ///< 左边框Rect对象 画笔为transparent
        QGraphicsRectItem* m_Right{}; ///< 右边框Rect对象 画笔为transparent
        QGraphicsRectItem* m_Top{}; ///< 上边框Rect对象 画笔为transparent
        QGraphicsRectItem* m_Bottom{}; ///< 下边框Rect对象 画笔为transparent

        QGraphicsRectItem* m_SplitRectItem{}; ///< logo与右侧文字之间的分割线对象

        std::unordered_map<ShowExifTexPositionIndex, QGraphicsTextItem*> m_TextItem; ///< 所有显示的Exif信息对应的GraphicsTextItem

        std::vector<ShowExifInfo> m_ShowInfos; ///< 最终显示到屏幕上的Exif信息
        ExifInfoMap m_TargetImageExifInfoLists; ///< 解析的当前图片的所有Info信息

        std::shared_ptr<SceneLayoutSettings> m_SceneLayout{nullptr}; ///< 记录场景的布局
    };
} // CM

#endif //CAMERAMARK_CSCENE_H
