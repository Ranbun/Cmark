#include <CMark.h>
#include "PreViewImageScene.h"
#include "PreViewImageItem.h"
#include <QGraphicsView>

namespace CM
{
    void PreViewImageScene::updateSceneRect()
    {
        applyLayout(m_SceneLayout);
        {
            const auto sceneRect = this->itemsBoundingRect();
            setSceneRect(sceneRect);
        }
    }
}
