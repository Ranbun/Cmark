#ifndef CAMERAMARK_IMAGEPROCESS_H
#define CAMERAMARK_IMAGEPROCESS_H

#include <QObject>
#include <QFileInfoList>
#include <QFileInfo>
#include <QPixmap>
#include <QDateTime>

#include <memory>
#include <QBuffer>
#include <QImageReader>
#include <QImageWriter>

#if _DEBUG
#include <QDebug>
#endif

namespace CM
{
    namespace
    {
        QString ImageSaveDefaultName()
        {
            const QDateTime currentDateTime = QDateTime::currentDateTime();
            auto outputName = currentDateTime.toString("yyyy-MM-dd__HHHmmMssS");

            constexpr std::hash<std::string> nameGenerator;
            const auto nameCode  = nameGenerator(outputName.toStdString());
            outputName = outputName + "__" + QString::number(nameCode);
            return {outputName};
        }
    }


    class ImageProcess : public QObject
    {
    public:
        ImageProcess() = default;
        ~ImageProcess() override = default;

        ImageProcess(const ImageProcess&) = delete;
        ImageProcess(const ImageProcess&&) = delete;

        ImageProcess& operator=(const ImageProcess&) = delete;
        ImageProcess& operator=(const ImageProcess&&) = delete;


        static std::shared_ptr<QByteArray> loadFile(const QString & fileName)
        {
            QFile file(fileName);

            if(file.open(QIODevice::ReadOnly))
            {
                return std::make_shared<QByteArray>();
            }

            auto data = std::make_shared<QByteArray>(file.readAll());
            file.close();
            return data;
        }

        static std::shared_ptr<QImage> toQImage(const std::shared_ptr<QByteArray>& imageData,const QString & format = "JPG")
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

        static std::shared_ptr<QImage> loadImage(const QString & fileName)
        {
            const QFileInfo fileInfo(fileName);

            const auto format = fileInfo.suffix().toLower();
            const auto data = loadFile(fileInfo.filePath());
            return toQImage(data,format);
        }


        static void save(const std::shared_ptr<QPixmap>& pixmap, const QString& fileName)
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

        static void save(const std::shared_ptr<QImage> &image, const QString& fileName)
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

    };

} // CM

#endif //CAMERAMARK_IMAGEPROCESS_H
