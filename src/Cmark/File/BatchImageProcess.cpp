#include "BatchImageProcess.h"

#include <CThread/ThreadPool.h>
#include <File/ImageProcess/ImageProcess.h>
#include <File/LogoManager.h>
#include <File/Resolver/EXIFResolver.h>
#include <File/ResourcesTool.h>
#include <Log/CLog.h>
#include <Scene/CScene.h>
#include <SceneLayoutSettings.h>
#include <QDir>
#include <QPainter>
#include <utility>

#include "Consumer.h"
#include "FileQueue.h"
#include "Producer.h"

namespace CM
{

BatchImageProcessor::BatchImageProcessor(QString rootPath)
    : QObject(nullptr)
    , m_Root(std::move(rootPath))
    , m_AvailableFileType({"jpeg", "jpg"})
{
}

BatchImageProcessor::~BatchImageProcessor()
{
    delete m_Pool;
    m_Pool = nullptr;
}

void BatchImageProcessor::Run()
{
    m_Queue = std::make_shared<CM::FileQueue>();
    // scan files
    auto *producer = new Producer(m_Queue, m_Root, m_AvailableFileType);
    QThreadPool::globalInstance()->start(producer);

    const int ideaThreadCount = QThreadPool::globalInstance()->maxThreadCount();
    const int consumerThreadCount = qMax(ideaThreadCount - 1, 1);

    m_ActiveConsumers = consumerThreadCount;

    /// 创建任务线程
    for (auto index = 0; index < consumerThreadCount; ++index)
    {
        auto *consumer = new Consumer(m_Queue, this);
        QThreadPool::globalInstance()->start(consumer);
    }
}

void BatchImageProcessor::onConsumerFinished()
{
    if (--m_ActiveConsumers == 0)
    {
        emit workFinished();
    }
}

}  // namespace CM
