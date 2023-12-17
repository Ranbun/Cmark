#include <CMark.h>

#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include <QGraphicsView>

namespace CM
{
    void PreViewImageScene::updateSceneRect(QGraphicsView *view, const QRect &newSceneRect)
    {
        QRect rect = view ? view->geometry() : newSceneRect;
        auto createRect = [&rect](int offset)->QRect
        {
            return {0,0,rect.width() - offset,rect.height() - offset};
        };
        view ? setSceneRect(createRect(2)): setSceneRect(createRect(0));

        const auto & [left,right,top,bottom] = m_sceneLayout.getMargin();
        auto newWidth = sceneRect().toRect().width() - left - right;
        auto ImageRatio = m_showImageItem->ImageRatio();
        auto newHeight = static_cast<int>(ImageRatio * static_cast<float>(newWidth));
        m_sceneLayout.setImageSize({newWidth,newHeight});

        m_sceneLayout.update();

        applyLayout();
    }

}
