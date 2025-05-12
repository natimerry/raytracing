#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{
  public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    template <typename F>
    void enqueue(F&& f)
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            normal_tasks.emplace(std::forward<F>(f));
        }
        condition.notify_one();
    }

    static ThreadPool& global()
    {
        static ThreadPool instance(std::thread::hardware_concurrency()-1);
        return instance;
    }

  private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> normal_tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

#endif // THREADPOOL_HPP