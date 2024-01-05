#include <CMark.h>
#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include <QGraphicsView>

namespace CM
{
    void PreViewImageScene::updateSceneRect(const QGraphicsView *view, const QRect &newSceneRect)
    {
        QRect rect = view ? view->geometry(): newSceneRect;
        auto createRect = [&rect](const int offset)->QRect
        {
            const auto r = QRect({0,0,rect.width() - offset,rect.height() - offset});
            auto w = r.width();
            auto h = r.height();
            return r;
        };

        if(view)  /// ? setSceneRect((2)): setSceneRect(createRect(0));
        {
            auto gRec = createRect(0);

            const auto& [left, right, top, bottom] = m_SceneLayout->getMargin();
            const auto r = gRec;
            auto w = r.width();
            auto newWidth = r.width() - left - right;
            const auto imageRatio = m_ShowImageItem->imageRatio();   ///< w / h = newwidth / newHeght => newHeght = newWidth * h / w;
            auto newHeight = static_cast<int>(std::floor(static_cast<float>(newWidth) / imageRatio));
            {
                newHeight = gRec.height() - m_SceneLayout->logoWithImageSpace() - m_SceneLayout->logoSize().h - m_SceneLayout->getMargin().m_Bottom;
                newWidth = imageRatio * newHeight;
            }
            /// need more fit way to calc width & height

            m_SceneLayout->setImageSize({ static_cast<int>(newWidth),newHeight });
            m_SceneLayout->update();

            const auto sceneH = m_SceneLayout->imageSize().h + m_SceneLayout->logoWithImageSpace() + m_SceneLayout->logoSize().h + m_SceneLayout->getMargin().m_Bottom;
            if(gRec.height() > sceneH)
            {
                gRec.setHeight(sceneH);
            }
            setSceneRect(gRec);

        }

        applyLayout(m_SceneLayout);
        {
            auto s = m_ShowImageItem->boundingRect();

            auto sc = m_ShowImageItem->sceneBoundingRect();
            auto sp = m_ShowImageItem->scenePos();

            auto offset = m_ShowImageItem->offset();

            if(!m_ShowImageItem->pixmap().isNull())
            {
                auto  s = m_ShowImageItem->pixmap().size();
                qDebug()<<s;
            }
            putchar(10);

        }
    }

}
