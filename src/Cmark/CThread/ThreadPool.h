#ifndef CAMERAMARK_THREADPOOL_H
#define CAMERAMARK_THREADPOOL_H

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <future>
#include <memory>
#include <functional>

namespace CM
{
    class ThreadPool
    {
    public:
        explicit ThreadPool(size_t numThreads);

        ThreadPool(const ThreadPool &) = delete;
        ThreadPool(const ThreadPool &&) = delete;

        ThreadPool& operator=(const ThreadPool &) = delete;
        ThreadPool& operator=(const ThreadPool &&) = delete;

        ~ThreadPool();

        template<class F, class... Args>
        auto enqueue(F && f, Args&&... args) -> std::future<decltype(f(args...))>
        {
            using return_type = decltype(f(args...));
            auto task = std::make_shared< std::packaged_task<return_type()>>(
                    std::bind(std::forward<F>(f),std::forward<Args>(args)...)
                    );

            std::future<return_type> res = task->get_future();
            {
                std::unique_lock<std::mutex> local;
                if(m_stop)
                {
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                }

                tasks.emplace([task](){
                    (*task)();
                });

                condition.notify_one();
                return res;
            }
        }

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool m_stop;
    };
} // CM

#endif //CAMERAMARK_THREADPOOL_H
