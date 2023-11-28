#ifndef CAMERAMARK_PREVIEWIMAGESCENE_H
#define CAMERAMARK_PREVIEWIMAGESCENE_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>


namespace CM
{
    struct Point
    {
        int x;
        int y;
        int z;
    };

    struct ImageInfoItem
    {
        QGraphicsTextItem * m_field;
        Point pos;  ///< position
        std::string m_infos;
    };

    class PreViewImageScene : public QGraphicsScene
    {
    public:
        PreViewImageScene(QObject * parent = nullptr);
        ~PreViewImageScene() override = default;
    private:
        std::vector<ImageInfoItem> m_infos;
    };

} // CM

#endif //CAMERAMARK_PREVIEWIMAGESCENE_H
