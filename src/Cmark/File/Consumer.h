#ifndef CAMERAMARK_CONSUMER_H
#define CAMERAMARK_CONSUMER_H
#include <qpointer.h>

#include <QRunnable>
#include <memory>

#include "BatchImageProcess.h"

namespace CM
{
class FileQueue;

class Consumer : public QRunnable
{
public:
    Consumer(std::shared_ptr<FileQueue> queue, BatchImageProcessor *processor);
    void run() override;

private:
    void processFile(const QFileInfo &fileInfo) const;

private:
    std::shared_ptr<FileQueue> m_Queue;
    QPointer<BatchImageProcessor> m_ImageProcessor;
};

}  // namespace CM

#endif  // CAMERAMARK_CONSUMER_H
