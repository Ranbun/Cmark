#include "LifeSizeImageScene.h"

#include <CMark.h>

namespace CM
{
    LifeSizeImageScene::LifeSizeImageScene(QObject* parent)
        : CScene(parent)
    {
    }

    void LifeSizeImageScene::saveSceneAsImage(const std::function<void(QGraphicsScene*)>& saveFunc)
    {
        m_sceneLayout->update();
        applyLayout(nullptr);
        setSceneRect(effectiveSceneRect());
        saveFunc(this);
    }
} // namespace CM
