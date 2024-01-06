#include <CMark.h>

#include "LifeSizeImageScene.h"

namespace CM
{

    LifeSizeImageScene::LifeSizeImageScene(QObject * parent)
    : CScene(parent)
    {

    }

    LifeSizeImageScene::~LifeSizeImageScene() = default;

    void LifeSizeImageScene::saveSceneAsImage(const std::function<void(QGraphicsScene *)> &saveFunc)
    {
        m_SceneLayout->update();
        applyLayout(nullptr);

        const auto sceneRect = itemsBoundingRect();
        setSceneRect(sceneRect);

        saveFunc(this);
    }
} // CM
