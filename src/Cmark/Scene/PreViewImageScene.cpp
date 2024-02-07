#include <CMark.h>

#include <QGraphicsView>

#include "PreViewImageScene.h"
#include "PreViewImageItem.h"

#include "File/PictureManager.h"
#include <File/Resolver/EXIFResolver.h>
#include <File/ImageProcess/ImageProcess.h>

namespace CM
{
    void PreViewImageScene::updateSceneRect()
    {
        applyLayout(m_SceneLayout);
        {
            const auto sceneRect = this->itemsBoundingRect();
            setSceneRect(sceneRect);
        }
    }

    void PreViewImageScene::showImage(const size_t fileIndexCode)
    {
        /// 设置预览场景显示的资源
        const auto exifInfos = EXIFResolver::getExifInfo(fileIndexCode);
        const auto cameraIndex = LogoManager::resolverCameraIndex(exifInfos.count(ExifKey::CameraMake)? exifInfos.at(ExifKey::CameraMake): "");
        const auto previewImageLogo = LogoManager::getCameraMakerLogo(cameraIndex);
        const auto preViewImage = PictureManager::getImage(fileIndexCode);
        resetStatus();
        resetPreviewImageTarget(*preViewImage, fileIndexCode);
        resetTexItemsPlainText(exifInfos);
        resetLogoPixmap(previewImageLogo, cameraIndex);

        {
            const auto &[FixImageSizeW, FixImageSizeH] = SceneLayoutSettings::fixPreViewImageSize();
            const auto newWidth = FixImageSizeW;
            const auto imageRatio = ImageProcess::imageRatio(*preViewImage);
            const auto newHeight = static_cast<int>(std::floor(static_cast<float>(newWidth) / imageRatio));
            const auto sceneLayout = layoutSettings();
            sceneLayout->setImageSize({static_cast<int>(newWidth), newHeight});
            sceneLayout->update();
        }
        updateSceneRect();

        /// fit view show
        const auto bound = sceneRect();
        for(const auto & view: views())
        {
            view->setSceneRect(bound); // 设置场景矩形
            view->fitInView(bound, Qt::KeepAspectRatio);
        }

    }

    void PreViewImageScene::initConnect()
    {
        connect(this,&PreViewImageScene::sigShowImage,[this](auto && code)
        {
            showImage(std::forward<decltype(code)>(code));
        });
    }
}
