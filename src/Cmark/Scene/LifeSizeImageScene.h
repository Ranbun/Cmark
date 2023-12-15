#ifndef CAMERAMARK_LIFESIZEIMAGESCENE_H
#define CAMERAMARK_LIFESIZEIMAGESCENE_H

#include "CScene.h"

namespace CM
{
    class LifeSizeImageScene : public  CScene
    {
    public:
        LifeSizeImageScene(QObject * parent = nullptr);
        ~LifeSizeImageScene() override;
        void saveSceneAsImage(const std::function<void(QGraphicsScene *)> &saveFunc);
    };

} // CM

#endif //CAMERAMARK_LIFESIZEIMAGESCENE_H
