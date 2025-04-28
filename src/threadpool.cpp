#include "threadpool.hpp"

ThreadPool::ThreadPool(size_t num_threads)
    : stop(false)
{
    for (size_t i = 0; i < num_threads; ++i)
    {
        workers.emplace_back([this] {
            while (true)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);

                    // Wait until there's a task or we need to stop
                    this->condition.wait(lock, [this] {
                        return this->stop || !this->normal_tasks.empty() || !this->low_priority_tasks.empty();
                    });

                    // If stopping and no tasks left, exit
                    if (this->stop && this->normal_tasks.empty() && this->low_priority_tasks.empty())
                        return;

                    // Prioritize normal tasks, then low-priority tasks
                    if (!this->normal_tasks.empty())
                    {
                        task = std::move(this->normal_tasks.front());
                        this->normal_tasks.pop();
                    }
                    else if (!this->low_priority_tasks.empty())
                    {
                        task = std::move(this->low_priority_tasks.front());
                        this->low_priority_tasks.pop();
                    }
                }

                // Execute the task
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}