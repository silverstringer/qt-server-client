#pragma once

#include<thread>
#include<vector>
#include<queue>
#include<mutex>
#include<condition_variable>
#include<functional>
#include<future>
#include "macros.h"

#define MAX_THREADS std::thread::hardware_concurrency() - 1;

class ThreadPool {
public:

    static ThreadPool &getInstance(int numThreads) {
        static ThreadPool instance(numThreads);

        return instance;
    }

    template<typename Func, typename... Args>
    inline auto push(Func &&f, Args &&... args) {

        typedef decltype(f(args...)) retType;
        std::packaged_task<retType()> task(std::move(std::bind(f, args...)));
        std::unique_lock<std::mutex> lock(JobMutex);
        std::future<retType> future = task.get_future();

        JobQueue.emplace(std::make_shared<AnyJob < retType> > (std::move(task)));
        thread.notify_one();

        return future;
    }


    inline void resize(int newTCount) {

        int tmp = MAX_THREADS;
        if (newTCount > tmp || newTCount < 1) {
            tmp = numThreads;
            numThreads = MAX_THREADS;
            Pool.resize(newTCount);
            for (int i = tmp; i != numThreads; ++i) {
                Pool.emplace_back(std::thread(&ThreadPool::threadManager, this));
                Pool.back().detach();
            }
        } else if (newTCount > numThreads) {
            uint8_t tmp = numThreads;
            numThreads = newTCount;
            Pool.resize(numThreads);
            for (int i = tmp; i != numThreads; ++i) {
                Pool.emplace_back(std::thread(&ThreadPool::threadManager, this));
                Pool.back().detach();
            }
        } else {
            numThreads = (uint8_t) newTCount;
            Pool.resize(newTCount);
        }


    }

    inline uint8_t getThreadCount() {
        return numThreads;
    }

private:

    class Job {
    private:
        std::packaged_task<void()> func;
    public:
        virtual ~Job() {}
        virtual void execute() = 0;
    };

    template<typename RetType>
    class AnyJob : public Job {
    private:
        std::packaged_task<RetType()> func;
    public:
        AnyJob(std::packaged_task<RetType()> func) : func(std::move(func)) {}

        void execute() {
            func();
        }
    };

    uint8_t numThreads;
    std::vector<std::thread> Pool;
    std::queue<std::shared_ptr<Job>> JobQueue;
    std::condition_variable thread;
    std::mutex JobMutex;

    inline void threadManager() {
        while (true) {

            std::unique_lock<std::mutex> lock(JobMutex);
            thread.wait(lock, [this] { return !JobQueue.empty(); });

            if (JobQueue.size() < 1)
                continue;

            (*JobQueue.front()).execute();

            JobQueue.pop();
        }
    }

    ThreadPool();
    inline ThreadPool(uint8_t numThreads) : numThreads(numThreads) {
        int tmp = MAX_THREADS;
        if (numThreads > tmp) {
            numThreads = tmp;
        }


        Pool.reserve(numThreads);
        for (int i = 0; i != numThreads; ++i) {
            Pool.emplace_back(std::thread(&ThreadPool::threadManager, this));
            Pool.back().detach();
        }
    }

    DECLARE_NO_COPY_CLASS(ThreadPool);
};