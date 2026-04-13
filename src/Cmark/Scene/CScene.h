#ifndef CAMERAMARK_CSCENE_H
#define CAMERAMARK_CSCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>

#include "Base/Type.h"
#include "SceneLayoutSettings.h"
#include "File/LogoManager.h"

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
 * @brief 记录显示的 Exif 信息和其布局位置
 */
struct ShowExifInfo
{
    ShowExifTexPositionIndex layout;  ///< 显示信息的位置
    std::vector<ExifKey> keys;        ///< 该位置显示的信息列表
};

class CScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit CScene(QObject *parent = nullptr);

    CScene(const CScene &) = delete;
    CScene(CScene &&) = delete;
    CScene &operator=(const CScene &) = delete;
    CScene &operator=(CScene &&) = delete;
    ~CScene() override;

    /** @brief 设置当前显示的所有文字信息（Exif） */
    void resetTexItemsPlainText(const CM::ExifInfoMap &exifInfoMap);

    /** @brief 更新 Logo 图 */
    void resetLogoPixmap(const std::shared_ptr<QPixmap> &logo, CameraIndex cameraIndex = CameraIndex::None);

    /** @brief 更新预览显示的图片 */
    void resetPreviewImageTarget(const QPixmap &pixmap, size_t imageIndexCode);

    /** @brief 应用布局，更新场景中所有 item */
    void applyLayout(const std::shared_ptr<SceneLayoutSettings> &layout = nullptr);

    /** @brief 刷新布局计算（子类可重写） */
    virtual void updateLayout();

    /** @brief 获取当前场景布局设置 */
    std::shared_ptr<SceneLayoutSettings> layoutSettings() const;

    /** @brief 重置场景显示状态 */
    void resetStatus();

    /** @brief 设置是否仅显示图片（隐藏 Logo、EXIF 文字、边距等） */
    void setImageOnlyMode(bool enable);

    /** @brief 是否处于仅图片模式 */
    [[nodiscard]] bool isImageOnlyMode() const;

protected:
    /** @brief 更新显示图片的布局信息 */
    virtual void updateShowImage();

    /** @brief 更新 Logo 位置 */
    virtual void updateLogoPosition();

    /** @brief 更新所有文字项位置 */
    virtual void updateTexItemsPosition();

    /** @brief 更新右侧文字与 Logo 之间的分割线 */
    virtual void updateSplitRect();

    /** @brief 更新边距矩形的尺寸与位置 */
    virtual void updateMarginItems();

    /** @brief 根据显示标志和仅图模式更新 overlay 项可见性 */
    virtual void updateOverlayItemsVisibility();

    /** @brief 当前场景有效显示矩形（仅图模式下为图片区域） */
    virtual QRectF effectiveSceneRect() const;

    PreViewImageItem *m_showImageItem{};
    QGraphicsPixmapItem *m_logoItem{};

    QGraphicsRectItem *m_left{};
    QGraphicsRectItem *m_right{};
    QGraphicsRectItem *m_top{};
    QGraphicsRectItem *m_bottom{};

    QGraphicsRectItem *m_splitRectItem{};

    std::unordered_map<ShowExifTexPositionIndex, QGraphicsTextItem *> m_textItem;
    std::vector<ShowExifInfo> m_showInfos;
    ExifInfoMap m_targetImageExifInfoMap;

    std::shared_ptr<SceneLayoutSettings> m_sceneLayout;
    QFont m_plainTextFont;

    bool m_showItemFlags{true};
    bool m_imageOnlyMode{false};

private:
    void init();
    void initTexItems();
    void initLogoItem();
    void initTargetImageItem();
    void initMargin();
    void initSplitRect();

signals:
    void noScenesChanged(bool showOverlays);
};

} // namespace CM

#endif // CAMERAMARK_CSCENE_H
