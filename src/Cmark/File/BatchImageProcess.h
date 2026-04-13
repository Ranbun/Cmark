#ifndef CAMERAMARK_BATCHIMAGEPROCESS_H
#define CAMERAMARK_BATCHIMAGEPROCESS_H

#include <QSet>
#include <QString>
#include <memory>
#include "FileQueue.h"

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

        void Run();
public slots:
        void onConsumerFinished();
    signals:
        void workFinished();

    private:
        QString m_Root;
        QList<QString> m_AvailableFileType{"jpeg", "jpg"};
        QFileInfoList m_ImageFileLists;
        QSet<QString> m_ScannedDirectories;
        ThreadPool * m_Pool {};

        std::shared_ptr<CM::FileQueue> m_Queue;

        uint32_t m_ActiveConsumers = 0;

    };
}


#endif
