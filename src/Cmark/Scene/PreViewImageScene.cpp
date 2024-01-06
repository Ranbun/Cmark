#include <CMark.h>
#include "PreViewImageScene.h"
#include "PreViewImageItem.h"
#include <QGraphicsView>

namespace CM
{
    void PreViewImageScene::updateSceneRect()
    {
        const auto& [FixImageSizeW, FixImageSizeH] = SceneLayoutSettings::fixPreViewImageSize();

        const auto newWidth = FixImageSizeW;
        const auto imageRatio = m_ShowImageItem->imageRatio();
        const auto newHeight = static_cast<int>(std::floor(static_cast<float>(newWidth) / imageRatio));
        m_SceneLayout->setImageSize({ static_cast<int>(newWidth),newHeight });
        m_SceneLayout->update();

        applyLayout(m_SceneLayout);
        {
            const auto sceneRect = this->itemsBoundingRect();
            setSceneRect(sceneRect);
        }
    }

}
