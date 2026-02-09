#include <CMark.h>

#include <QGraphicsView>

#include "DisplayImageScene.h"
#include "PreViewImageItem.h"

#include "File/PictureManager.h"
#include <File/Resolver/EXIFResolver.h>

namespace CM
{
    void DisplayImageScene::updateSceneRect()
    {
        applyLayout(m_SceneLayout);
        {
            const auto sceneRect = this->itemsBoundingRect();
            setSceneRect(sceneRect);
        }
    }

    void DisplayImageScene::showImage(const size_t fileIndexCode)
    {
        /// 设置预览场景显示的资源
        const auto preViewImage = PictureManager::getImage(fileIndexCode);
        resetPreviewImageTarget(*preViewImage, fileIndexCode);
    }

    void DisplayImageScene::initConnect()
    {
        connect(this,&DisplayImageScene::sigShowImage,[this](auto && code)
        {
            showImage(std::forward<decltype(code)>(code));
        });
    }
}
