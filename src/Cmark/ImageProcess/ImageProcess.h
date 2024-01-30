#ifndef CAMERAMARK_IMAGEPROCESS_H
#define CAMERAMARK_IMAGEPROCESS_H

#include <QObject>
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


    class FileProcess: public QObject
    {

    public:
        FileProcess() = default;
        ~FileProcess() override = default;

        FileProcess(const FileProcess&) = delete;
        FileProcess(const FileProcess&&) = delete;

        FileProcess& operator=(const FileProcess&) = delete;
        FileProcess& operator=(const FileProcess&&) = delete;

        static size_t generateFileIndexCode(const std::string& path)
        {
            return hash<std::string>(path);
        }

    private:
        template<typename T>
        static size_t hash(const T& key)
        {
            constexpr std::hash<T> hasher;
            const size_t hashValue = hasher(key);
            return hashValue;
        }

    };


    class ImageProcess final : public FileProcess
    {
    public:
        ImageProcess() = default;
        ~ImageProcess() override = default;

        ImageProcess(const ImageProcess&) = delete;
        ImageProcess(const ImageProcess&&) = delete;

        ImageProcess& operator=(const ImageProcess&) = delete;
        ImageProcess& operator=(const ImageProcess&&) = delete;

        static std::shared_ptr<QByteArray> loadFile(const QString & fileName);

        static std::shared_ptr<QImage> toQImage(const std::shared_ptr<QByteArray>& imageData,const QString & format = "JPG");

        static std::shared_ptr<QImage> loadImage(const QString & fileName);

        static void save(const std::shared_ptr<QPixmap>& pixmap, const QString& fileName);

        static void save(const std::shared_ptr<QImage> &image, const QString& fileName);

        static float imageRatio(const QPixmap &pixmap);

    };

} // CM

#endif //CAMERAMARK_IMAGEPROCESS_H
