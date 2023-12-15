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
        updateLayout();
        applyLayout();
        saveFunc(this);
    }
} // CM
