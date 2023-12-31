#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H_
#define CAMERAMARK_PREVIEWIMAGESCENE_H_

#include "CScene.h"

namespace CM
{
    class PreViewImageItem;

    class PreViewImageScene final :public CScene
    {
    public:
        explicit PreViewImageScene(QObject * parent = nullptr)
        : CScene(parent)
        {

        }

        PreViewImageScene(const PreViewImageScene&) = delete;
        PreViewImageScene(const PreViewImageScene&&) = delete;

        PreViewImageScene& operator=(const PreViewImageScene&) = delete;
        PreViewImageScene& operator=(const PreViewImageScene&&) = delete;


        ~PreViewImageScene() override = default;

        void updateSceneRect();
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H_
