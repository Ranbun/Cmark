#include "PreViewImageScene.h"

#include "PreViewImageItem.h"
#include "File/PictureManager.h"
#include "File/Resolver/EXIFResolver.h"
#include "File/ImageProcess/ImageProcess.h"

#include <CMark.h>
#include <QGraphicsView>

#include <cmath>

namespace CM
{
    PreViewImageScene::PreViewImageScene(QObject* parent)
        : CScene(parent)
    {
        initConnect();
    }

    void PreViewImageScene::updateSceneRect()
    {
        applyLayout(m_sceneLayout);
        setSceneRect(effectiveSceneRect());
    }
    void PreViewImageScene::ForceUpdate()
    {
        emit  sigShowImage(m_currentFileIndex);
    }

    void PreViewImageScene::showImage(const size_t fileIndexCode)
    {
        m_currentFileIndex = fileIndexCode;

        // 1. 加载图片与 EXIF 数据
        const auto preViewImage = PictureManager::getImage(fileIndexCode);
        const auto exifInfos = EXIFResolver::getExifInfo(fileIndexCode);
        const auto cameraMake = exifInfos.count(ExifKey::CameraMake) ? exifInfos.at(ExifKey::CameraMake) : "";
        const auto cameraIndex = LogoManager::resolverCameraIndex(cameraMake);
        const auto previewImageLogo = LogoManager::getCameraMakerLogo(cameraIndex);

        // 2. 更新场景显示内容
        resetStatus();
        resetPreviewImageTarget(*preViewImage, fileIndexCode);
        resetTexItemsPlainText(exifInfos);
        resetLogoPixmap(previewImageLogo, cameraIndex);

        // 3. 根据图片比例计算并设置布局尺寸
        const auto [fixHeight, fixWidth] = SceneLayoutSettings::fixPreViewImageSize();
        const auto imageRatio = ImageProcess::imageRatio(*preViewImage);
        const auto newHeight = static_cast<int>(std::floor(static_cast<float>(fixWidth) / imageRatio));
        const auto sceneLayout = layoutSettings();
        sceneLayout->setImageSize({fixWidth, newHeight});
        sceneLayout->update();

        // 4. 更新场景矩形并适配视图
        updateSceneRect();
        const auto bound = sceneRect();
        for (const auto& view : views())
        {
            view->setSceneRect(bound);
            view->fitInView(bound, Qt::KeepAspectRatio);
            view->repaint();
        }
    }

    void PreViewImageScene::initConnect()
    {
        connect(this, &PreViewImageScene::sigShowImage, this, [this](size_t code) { showImage(code); });
    }
} // namespace CM
