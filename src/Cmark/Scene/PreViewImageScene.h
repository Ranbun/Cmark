#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H_
#define CAMERAMARK_PREVIEWIMAGESCENE_H_

#include "CScene.h"

namespace CM
{
    class PreViewImageItem;

    /**
     * @brief 预览图片场景，用于显示单张图片及 EXIF 等信息
     */
    class PreViewImageScene final : public CScene
    {
        Q_OBJECT
    public:
        explicit PreViewImageScene(QObject* parent = nullptr);
        ~PreViewImageScene() override = default;

        PreViewImageScene(const PreViewImageScene&) = delete;
        PreViewImageScene(PreViewImageScene&&) = delete;
        PreViewImageScene& operator=(const PreViewImageScene&) = delete;
        PreViewImageScene& operator=(PreViewImageScene&&) = delete;

        /** @brief 根据当前 items 边界更新场景矩形 */
        void updateSceneRect();

    signals:
        /** @brief 请求显示指定索引的图片 */
        void sigShowImage(size_t code);

    private:
        void showImage(size_t fileIndexCode);
        void initConnect();
    };

} // namespace CM

#endif // CAMERAMARK_PREVIEWIMAGESCENE_H_
