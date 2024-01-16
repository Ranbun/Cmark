#include "ImageProcess.h"

namespace CM
{
    std::shared_ptr<QByteArray> ImageProcess::loadFile(const QString& fileName)
    {
        QFile file(fileName);

        if(!file.open(QIODevice::ReadOnly))
        {
            return std::make_shared<QByteArray>();
        }

        auto data = std::make_shared<QByteArray>(file.readAll());
        file.close();
        return data;
    }

    std::shared_ptr<QImage> ImageProcess::toQImage(const std::shared_ptr<QByteArray>& imageData, const QString& format)
    {
        QBuffer ReadAsImageBuffer(imageData.get());
        {
            ReadAsImageBuffer.open(QIODevice::ReadOnly);
            ReadAsImageBuffer.seek(0);
        }

        const auto imageReader = std::make_shared<QImageReader>(&ReadAsImageBuffer, format.toStdString().c_str());
        imageReader->setAutoTransform(true);
        return std::make_shared<QImage>(imageReader->read());
    }

    std::shared_ptr<QImage> ImageProcess::loadImage(const QString& fileName)
    {
        const QFileInfo fileInfo(fileName);

        const auto format = fileInfo.suffix().toLower();
        const auto data = loadFile(fileInfo.filePath());
        return toQImage(data,format);
    }

    void ImageProcess::save(const std::shared_ptr<QPixmap>& pixmap, const QString& fileName)
    {
        auto name = fileName;
        if (name.isEmpty())
        {
            name = ImageSaveDefaultName() + ".png";
        }

        QBuffer buffer;
        buffer.open(QIODevice::WriteOnly);

        const QFileInfo fileInfo(name);
        const auto format = fileInfo.suffix().toLower().toStdString();

        if (pixmap->save(&buffer, format.c_str()))
        {
            if (QFile file(name);
                file.open(QIODevice::WriteOnly))
            {
                file.write(buffer.data());
                file.close();
                qDebug() << "Image saved successfully.";
            }
        }
    }

    void ImageProcess::save(const std::shared_ptr<QImage>& image, const QString& fileName)
    {
        auto name = fileName;
        if (name.isEmpty())
        {
            name = ImageSaveDefaultName() + ".png";
        }

            
        if (QFile file(name);
            file.open(QIODevice::WriteOnly))
        {
            const QFileInfo fileInfo(name);
            const auto format = fileInfo.suffix().toLower().toStdString();

            if(QImageWriter imageWriter(&file, format.c_str());
                imageWriter.write(*image))
            {
                qDebug() << "Image saved successfully.";
            }

            file.close();
        }
    }
} // CM
