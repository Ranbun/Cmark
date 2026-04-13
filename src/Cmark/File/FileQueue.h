#ifndef FILEQUEUE_H_
#define FILEQUEUE_H_

#include <QFileInfo>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <vector>

namespace CM
{
class FileQueue
{
public:
    FileQueue(int maxSize = 100)
        : m_MaxSize(maxSize)
        , m_Finished(false) {

        };
    ~FileQueue()
    {
    }

    void push(const QFileInfo& fileInfo)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        while (m_Queue.size() >= m_MaxSize && !m_Finished)
        {
            m_NotFull.wait(locker);
        }

        if (m_Finished)
            return;

        m_Queue.push(fileInfo);
        m_NotEmpty.notify_one();
    }

    void push(const std::vector<QFileInfo>& fileInfos)
    {
        for (auto& info : fileInfos)
        {
            push(info);
        }
    }

    bool pop(QFileInfo& fileInfo)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        while (m_Queue.empty() && !m_Finished)
        {
            m_NotEmpty.wait(locker);
        }

        if (m_Queue.empty() && m_Finished)
        {
            return false;
        }
        fileInfo = m_Queue.front();
        m_Queue.pop();

        m_NotFull.notify_one();
        return true;
    }

    void clear();

    void setFinished()
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_Finished = true;
        m_NotEmpty.notify_all();
        m_NotFull.notify_all();
    }

private:
    std::atomic<bool> m_Finished{false};
    std::queue<QFileInfo> m_Queue;

    std::mutex m_mutex;
    std::condition_variable m_NotEmpty;
    std::condition_variable m_NotFull;

    int m_MaxSize;  ///< 最大任务数量
};
}  // namespace CM

#endif  // FILEQUEUE_H_
