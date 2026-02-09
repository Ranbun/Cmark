#ifndef CAMERAMARK_BATCHIMAGEPROCESS_H
#define CAMERAMARK_BATCHIMAGEPROCESS_H

#include <QFileInfoList>
#include <QSet>
#include <QString>

namespace CM
{
    class ThreadPool;
}

namespace CM
{
    class BatchImageProcessor final : public QObject
    {
        Q_OBJECT
    public:
        explicit BatchImageProcessor(QString rootPath);
        ~BatchImageProcessor() override;

        BatchImageProcessor(const BatchImageProcessor&) = delete;
        BatchImageProcessor(const BatchImageProcessor &&) = delete;

        BatchImageProcessor& operator=(const BatchImageProcessor&) = delete;
        BatchImageProcessor& operator=(const BatchImageProcessor&&) = delete;
        QFileInfoList & availableFileInfos();

        /**
         * @brief do task
         */
        void Run();

    signals:
        void workFinished();                     // 通知任务结束

    protected:
        void scanDirectory(const QString &path);

    private:
        QString m_Root;
        QList<QString> m_AvailableFileType{"jpeg", "jpg"};
        QFileInfoList m_ImageFileLists;
        QSet<QString> m_ScannedDirectories;
        ThreadPool * m_Pool {};

    };
}


#endif
