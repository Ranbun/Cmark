#include <CMark.h>
#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include <QGraphicsView>

namespace CM
{
    void PreViewImageScene::updateSceneRect(const QGraphicsView *view, const QRect &newSceneRect)
    {
        QRect rect = view ? view->geometry() : newSceneRect;
        auto createRect = [&rect](const int offset)->QRect
        {
            return {0,0,rect.width() - offset,rect.height() - offset};
        };
        view ? setSceneRect(createRect(2)): setSceneRect(createRect(0));

        const auto & [left,right,top,bottom] = m_SceneLayout->getMargin();
        const auto newWidth = sceneRect().toRect().width() - left - right;
        const auto imageRatio = m_ShowImageItem->imageRatio();
        const auto newHeight = static_cast<int>(imageRatio * static_cast<float>(newWidth));
        m_SceneLayout->setImageSize({newWidth,newHeight});
        m_SceneLayout->update();

        applyLayout(m_SceneLayout);
    }

}
