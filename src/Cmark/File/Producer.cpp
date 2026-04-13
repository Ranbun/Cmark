#include "Producer.h"

#include <QDir>
#include <utility>

namespace CM
{
Producer::Producer(std::shared_ptr<FileQueue> queue, QString rootPath, const QList<QString> &filters)
    : m_Queue(std::move(queue))
    , m_Filters(filters)
    , m_Root(std::move(rootPath))
{

}
Producer::~Producer()
{
}

void Producer::run()
{
    QSet<QString> scannedDirs;
    scanDirectory(m_Root, scannedDirs);

    m_Queue->setFinished();
}
void Producer::scanDirectory(const QString &path, QSet<QString> &scannedDirs)
{
    const QDir dir(path);
    if (!dir.exists())
    {
        return ;
    }

    QStringList entries = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString & entry : entries)
    {
        auto entryPath = dir.filePath(entry);
        QFileInfo fileInfo(entryPath);

        if (fileInfo.isDir() && !scannedDirs.contains(entryPath))
        {
            scannedDirs.insert(entryPath);
            scanDirectory(entryPath, scannedDirs);
        }
        else if (fileInfo.isFile())
        {
            const auto fileType = fileInfo.suffix().toLower();
            if (m_Filters.contains(fileType))
            {
                m_Queue->push(fileInfo);
            }
        }
    }

}
}  // namespace CM