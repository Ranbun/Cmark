#ifndef CAMERAMARK_DISPLAYIMAGESCENE_H_
#define CAMERAMARK_DISPLAYIMAGESCENE_H_

#include "CScene.h"

namespace CM
{
    class DisplayImageScene final :public CScene
    {
        Q_OBJECT
    public:
        explicit DisplayImageScene(QObject * parent = nullptr)
        : CScene(parent)
        {
            initConnect();
        }
        DisplayImageScene(const DisplayImageScene&) = delete;
        DisplayImageScene(const DisplayImageScene&&) = delete;
        DisplayImageScene& operator=(const DisplayImageScene&) = delete;
        DisplayImageScene& operator=(const DisplayImageScene&&) = delete;
        ~DisplayImageScene() override = default;

        void updateSceneRect();

    private:
        void showImage(size_t fileIndexCode);
        void initConnect();

    signals:
        void sigShowImage(size_t code);

    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H_
