#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <filesystem>
#include <QWidget>

class QGraphicsScene;

namespace CM
{

    class DisplayWidget : public QWidget
    {
    public:
        explicit DisplayWidget(QWidget * parent = nullptr);
    public:
        void Open(const std::filesystem::path& path);
        void AddImage(std::filesystem::path & path);

    protected:
        void paintEvent(QPaintEvent * event) override;

    private:
        QGraphicsScene * m_scene{nullptr};

    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
