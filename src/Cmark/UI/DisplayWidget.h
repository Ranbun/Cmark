#ifndef CAMERAMARK_DISPLAYWIDGET_H
#define CAMERAMARK_DISPLAYWIDGET_H

#include <filesystem>
#include <QWidget>

#include <iostream>

class QGraphicsView;
class QGraphicsScene;


namespace CM
{
    class DisplayWidget : public QWidget
    {
        Q_OBJECT
    public:
        explicit DisplayWidget(QWidget * parent = nullptr);
    public:
        void Open(const std::filesystem::path& path);
        void AddImage(const std::filesystem::path & path) const;

    protected:
        void paintEvent(QPaintEvent * event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        QGraphicsScene * m_scene{nullptr};
        QGraphicsView * m_view{nullptr};

    signals:
        void Created();

    };

} // CM

#endif //CAMERAMARK_DISPLAYWIDGET_H
