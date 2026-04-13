#ifndef PRODUCER_H_
#define PRODUCER_H_

#include <QSet>
#include <QString>
#include <QRunnable>
#include "FileQueue.h"

namespace CM
{
    class Producer : public QRunnable
    {
    public:
        explicit Producer(std::shared_ptr<FileQueue> queue, QString rootPath, const QList<QString>& filters);
        ~Producer() override;
        void run() override;

    protected:
        void scanDirectory(const QString& path, QSet<QString>& scannedDirs);

    private:
        QString m_Root;
        std::shared_ptr<CM::FileQueue> m_Queue;
        QList<QString> m_Filters;

    };
    }  // namespace CM

#endif // PRODUCER_H_
