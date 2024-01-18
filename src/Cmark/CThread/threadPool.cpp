#include "threadPool.h"

namespace CM
{
    ThreadPool::ThreadPool(size_t numThreads)
            : m_stop(false)
    {
        /// create thread
        for(size_t i = 0; i < numThreads; i++)
        {
            workers.emplace_back([this]()
                                 {
                                     while(true)
                                     {
                                         std::function<void()> task;
                                         {
                                             std::unique_lock<std::mutex> lock(queueMutex);
                                             condition.wait(lock,[this]{return m_stop || !tasks.empty();});  ///< 如果当前没有任务 - 线程阻塞等待
                                             if(m_stop && tasks.empty())
                                             {
                                                 return ;
                                             }
                                             task = std::move(tasks.front());
                                             tasks.pop();
                                         }
                                         task();
                                     }
                                 });
        }
    }

    ThreadPool::~ThreadPool()
    {
        {
            std::unique_lock<std::mutex> local(queueMutex);
            m_stop = true;
        }

        condition.notify_all();
        for(auto & worker: workers)
        {
            if(worker.joinable())
            {
                worker.join();
            }
        }
    }
} // CM
