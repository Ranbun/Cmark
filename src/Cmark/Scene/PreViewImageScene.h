#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H_
#define CAMERAMARK_PREVIEWIMAGESCENE_H_

#include "CScene.h"

namespace CM
{
    class PreViewImageItem;

    class PreViewImageScene :public CScene
    {
    public:
        explicit PreViewImageScene(QObject * parent = nullptr)
        : CScene(parent)
        {

        }

        ~PreViewImageScene() override = default;
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H_