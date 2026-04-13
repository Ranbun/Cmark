#include "Consumer.h"

#include <Log/CLog.h>
#include <qtextoption.h>
#include <QPainter>
#include "Base/ImagePack.h"
#include "ImageProcess/ImageProcess.h"
#include "LogoManager.h"
#include "Resolver/EXIFResolver.h"
#include "Scene/CScene.h"
#include "SceneLayoutSettings.h"

namespace CM
{
Consumer::Consumer(std::shared_ptr<FileQueue> queue, BatchImageProcessor* processor)
    : m_Queue(queue)
    , m_ImageProcessor(processor)
{
}
void Consumer::run()
{
    QFileInfo fileInfo;
    while (m_Queue->pop(fileInfo))
    {
        processFile(fileInfo);
    }

    if (m_ImageProcessor)
    {
        QMetaObject::invokeMethod(m_ImageProcessor.data(), "onConsumerFinished", Qt::QueuedConnection);
    }
}

void Consumer::processFile(const QFileInfo& fileInfo) const
{
    Q_UNUSED(this);

    CLog::Info(fileInfo.absoluteFilePath());
    if (!fileInfo.exists())
    {
        CLog::Warning(QString("File: ") + fileInfo.filePath() + QString(" does not exist!"));
        return;
    }
    const auto& [w, h] = SceneLayoutSettings::fixPreViewImageSize();
    const auto fileIndexCode = ImageProcess::generateFileIndexCode(fileInfo.filePath().toStdString());
    const auto data = ImageProcess::loadFile(fileInfo.filePath());

    if (!data)
        return;

    auto loadedImage = std::make_shared<QImage>();
    const ImagePack loadImagePack{
        fileIndexCode, data, fileInfo.filePath().toStdString(),  {w, h}};
    const auto format = fileInfo.suffix().toUpper();
    loadedImage = ImageProcess::toQImage(data, format);

    if (w != -1 && h != -1)
    {
        const auto imageSize = QSizeF(loadedImage->size());
        const auto imageRatio = imageSize.width() / imageSize.height();
        const auto newHeight = static_cast<int>(std::floor(static_cast<float>(w) / imageRatio));

        *loadedImage = loadedImage->scaled(w, newHeight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }

    EXIFResolver::resolver(loadImagePack);

    data->clear();

#if _DEBUG
    CLog::Info(QString("File: ") + fileInfo.filePath() + QString(" load success!"));
#endif

    auto& logoMgr = LogoManager::instance();
    const auto cameraLogoIndex = logoMgr.resolveCameraIndex(EXIFResolver::ExifItem(fileIndexCode, ExifKey::CameraMake));
    const auto logoImage = logoMgr.getCameraMakerLogo(cameraLogoIndex);
    const auto logo =
        logoImage ? std::make_shared<QPixmap>(QPixmap::fromImage(*logoImage)) : std::make_shared<QPixmap>();

    /// 获取绘制的图片
    const auto preViewImage = std::make_shared<QPixmap>(QPixmap::fromImage(*loadedImage));

    SceneLayoutSettings layout;
    auto size = preViewImage->size();
    layout.setImageSize({size.width(), size.height()});
    layout.update();

    auto newH = size.height() + layout.logoWithImageSpace() + layout.logoSize().h + layout.getMargin().m_Bottom +
                layout.getMargin().m_Top;
    auto newW = size.width() + layout.getMargin().m_Left + layout.getMargin().m_Right;
    const auto output = std::make_shared<QPixmap>(newW, newH);

    output->fill(Qt::white);

    /// 计算绘制的字体& 绘制的图标的位置
    size = output->size();
    const auto x = size.width() - layout.logoWithSplitLineSpace() * 2 - layout.logoSize().w -
                   layout.rightTextMaxWidth() - layout.rightTextOffset() - layout.splitRectWidth();
    const auto y = layout.getMargin().m_Top + preViewImage->size().height() + layout.logoWithImageSpace();

    QPainter pen(&(*output));

    auto& [logoW, logoH] = layout.logoSize();

    pen.drawPixmap(0, 0, *preViewImage);

    /// draw logo
    pen.drawPixmap(x, y, logo->scaled({logoW, logoH}, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    /// 绘制文字
    const auto exifInfosmap = EXIFResolver::getExifInfo(fileIndexCode);
    struct outputLayoutInfo
    {
        ShowExifTexPositionIndex m_LayoutIndex;
        std::string m_Infos;
    };
    std::vector<ShowExifInfo> showInfos;
    std::vector<outputLayoutInfo> outputLayoutInfos;
    {
        showInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::LeftTop, {ExifKey::CameraModel}});
        showInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::LeftBottom, {ExifKey::ImageDate}});
        showInfos.emplace_back(ShowExifInfo{ShowExifTexPositionIndex::RightTop, {ExifKey::LensModel}});
        showInfos.emplace_back(
            ShowExifInfo{ShowExifTexPositionIndex::RightBottom,
                         {ExifKey::FocalLength, ExifKey::FStop, ExifKey::ExposureTime, ExifKey::ISOSpeed}});

        for (const auto& [layoutIndex, keys] : showInfos)
        {
            if (keys.empty())
            {
                outputLayoutInfos.emplace_back(outputLayoutInfo{layoutIndex, ""});
                continue;
            }

            std::vector<std::string> exifInfos;
            std::for_each(keys.begin(), keys.end(),
                          [&](const ExifKey& key)
                          {
                              if (exifInfosmap.count(key))
                              {
                                  exifInfos.emplace_back(exifInfosmap.at(key));
                              }
                          });

            auto res = std::accumulate(exifInfos.begin(), exifInfos.end(), std::string(),
                                       [](const std::string& a, const std::string& b)
                                       {
                                           if (a.empty() && b.empty())
                                           {
                                               return std::string();
                                           }
                                           const auto tail = b.empty() ? std::string() : b;
                                           auto temp = a;
                                           if (!b.empty() && !a.empty())
                                           {
                                               temp += " ";
                                           }
                                           return temp + tail;
                                       });

            outputLayoutInfos.emplace_back(outputLayoutInfo{layoutIndex, std::move(res)});
        }
    }

    const auto& [l, r, t, b] = layout.getMargin();
    auto [layoutImageSizeW, layoutImageSizeH] = layout.imageSize();

    QTextOption textOption;
    textOption.setAlignment(Qt::AlignLeft | Qt::AlignTop);  // 居中对齐
    pen.setPen(Qt::black);

    for (auto [index, infos] : outputLayoutInfos)
    {
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(11);
        font.setPixelSize(13);

        switch (index)
        {
            case ShowExifTexPositionIndex::LeftTop:
            {
                font.setBold(true);
                pen.setFont(font);
                /// 计算位置
                QFontMetrics metrics(pen.font());
                [[maybe_unused]] int textWidth = metrics.horizontalAdvance(infos.c_str());
                int textHeight = metrics.height();

                QPoint position(layout.leftTextOffset(),
                                t + preViewImage->size().height() + layout.logoWithImageSpace() + textHeight);
                pen.drawText(position, infos.c_str());
            }
            break;
            case ShowExifTexPositionIndex::LeftBottom:
            {
                pen.setFont(font);
                // 计算位置
                QFontMetrics metrics(pen.font());
                int textWidth = metrics.horizontalAdvance(infos.c_str());
                int textHeight = metrics.height();
                QPoint position(
                    layout.leftTextOffset(),
                    t + preViewImage->size().height() + layout.logoWithImageSpace() + layout.logoSize().h - textHeight);

                auto rect = QRect(position, QSize{textWidth, textHeight});
                pen.fillRect(rect, Qt::white);
                pen.drawText(rect, infos.c_str(), textOption);
            }
            break;
            case ShowExifTexPositionIndex::RightTop:
            {
                font.setBold(true);
                pen.setFont(font);
                QFontMetrics metrics(pen.font());
                [[maybe_unused]] int textWidth = metrics.horizontalAdvance(infos.c_str());
                int textHeight = metrics.height();
                QPoint position(l + layoutImageSizeW + r - layout.rightTextMaxWidth() - layout.rightTextOffset(),
                                t + layoutImageSizeH + layout.logoWithImageSpace() + textHeight);

                pen.drawText(position, infos.c_str());
            }
            break;
            case ShowExifTexPositionIndex::RightBottom:
            {
                pen.setFont(font);
                // 计算位置
                QFontMetrics metrics(pen.font());
                int textWidth = metrics.horizontalAdvance(infos.c_str());
                int textHeight = metrics.height();
                QPoint position(
                    l + layoutImageSizeW + r - layout.rightTextMaxWidth() - layout.rightTextOffset(),
                    t + preViewImage->size().height() + layout.logoWithImageSpace() + layout.logoSize().h - textHeight);

                auto rect = QRect(position, QSize{textWidth, textHeight});
                pen.fillRect(rect, Qt::white);
                pen.drawText(rect, infos.c_str(), textOption);
            }
            break;
        }
    }

    pen.end();

    /// 保存图片
    ImageProcess::save(output, fileInfo.fileName());
}
}  // namespace CM