#ifndef CAMERAMARK_LIFESIZEIMAGESCENE_H_
#define CAMERAMARK_LIFESIZEIMAGESCENE_H_

#include "CScene.h"

#include <QGraphicsScene>
#include <functional>

namespace CM
{
/** @brief 等比例展示图片场景，用于导出/保存场景为图片 */
class LifeSizeImageScene : public CScene
{
  public:
    explicit LifeSizeImageScene(QObject* parent = nullptr);
    ~LifeSizeImageScene() override = default;

    LifeSizeImageScene(const LifeSizeImageScene&) = delete;
    LifeSizeImageScene(LifeSizeImageScene&&) = delete;
    LifeSizeImageScene& operator=(const LifeSizeImageScene&) = delete;
    LifeSizeImageScene& operator=(LifeSizeImageScene&&) = delete;

    /** @brief 将当前场景保存为图片 */
    void saveSceneAsImage(const std::function<void(QGraphicsScene*)>& saveFunc);
};

}  // namespace CM

#endif  // CAMERAMARK_LIFESIZEIMAGESCENE_H_
