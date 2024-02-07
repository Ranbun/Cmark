#ifndef CAMERAMARK_BATCHIMAGEPROCESS_H
#define CAMERAMARK_BATCHIMAGEPROCESS_H

#include <QList>
#include <QString>
#include <QDir>
#include <QSet>
#include <QFileInfoList>

namespace CM
{
    class ThreadPool;
}

namespace CM
{
    class BatchImageProcessor
    {
    public:
        explicit BatchImageProcessor(QString rootPath);

        QFileInfoList &availabeleFileInfos();

        void Run();

    private:
        void scanDirectory(const QString &path);


    private:
        QString m_Root;
        const QList<QString> availableFileType{"jpeg", "jpg"};
        QFileInfoList m_ImageFileLists;
        QSet<QString> m_ScannedDirectories;

        ThreadPool * m_Pool {};


    };
}


#endif
