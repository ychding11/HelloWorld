#include "parallel.h"
#include <thread>
#include <vector>
#include <mutex>
#include <iostream>
#include <cstdio>
#include <condition_variable>
#include <functional>


class ParallelForLoop; // forward declare.
static std::vector<std::thread> threads;
static std::mutex workListMutex;
static bool shutdowThread = false;
static std::condition_variable workListCondition;
static ParallelForLoop *workList = nullptr;

thread_local int threadIndex;

int numSystemCores(void)
{
    return std::max(1u, std::thread::hardware_concurrency());
}

int maxThreadIndex(void)
{
    return numSystemCores();
}

class ParallelForLoop
{
public:
    ParallelForLoop(std::function<void(int64_t)> func, int64_t maxIndex, int chunkSize)
        :mFunc(func)
        ,mMaxIndex(maxIndex)
        ,mChunkSize(chunkSize) { }

public:

    const int64_t mMaxIndex;
    const int mChunkSize;
          int64_t mNextIndex = 0;
          int mActiveWorkers = 0;
    std::function<void(int64_t)> mFunc;
    ParallelForLoop *next = nullptr;

public:
    bool Finished(void) const
    {
        return mNextIndex >= mMaxIndex && mActiveWorkers == 0;
    }

};

void workerThread(int tIndex)
{
    printf("- Thread %2d begins running.\n", tIndex);
    threadIndex = tIndex;
    
    std::unique_lock<std::mutex> lock(workListMutex);
    while (!shutdowThread)
    {
       if (!workList) 
       {
           workListCondition.wait(lock);
       }
       else
       {
            // Thread wakes and does job here.
            ParallelForLoop &loop = *workList;
            int64_t indexStart = loop.mNextIndex;
            int64_t indexEnd   = std::min(indexStart + loop.mChunkSize, loop.mMaxIndex);
            loop.mNextIndex = indexEnd;
            if (loop.mNextIndex == loop.mMaxIndex) workList = loop.next;
            loop.mActiveWorkers++;

            lock.unlock();
            
            for (uint64_t i = indexStart; i < indexEnd; ++i)
            {
                if (loop.mFunc) loop.mFunc(i);
            }
            lock.lock();
            loop.mActiveWorkers--;
            if (loop.Finished()) workListCondition.notify_all();
       }
    }
    printf("- Thread %2d exits.\n", tIndex);
}

void ParallelFor(std::function<void(int64_t)> func, int64_t count, int chunkSize)
{
   if (threads.size() == 0 || count < chunkSize) 
   {
       for (int64_t i = 0; i < count; ++i) func(i);
       return;
   }

   ParallelForLoop loop(std::move(func), count, chunkSize);
   workListMutex.lock();
   loop.next = workList;
   workList = &loop;
   workListMutex.unlock();

   std::unique_lock<std::mutex> lock(workListMutex);
   workListCondition.notify_all();

   while (!loop.Finished())
   {
        int64_t indexStart = loop.mNextIndex;
        int64_t indexEnd   = std::min(indexStart + loop.mChunkSize, loop.mMaxIndex);
        loop.mNextIndex = indexEnd;
        if (loop.mNextIndex == loop.mMaxIndex) workList = loop.next;
        loop.mActiveWorkers++;

        lock.unlock();
        
        for (uint64_t i = indexStart; i < indexEnd; ++i)
        {
            if (loop.mFunc) loop.mFunc(i);
        }
        lock.lock();
        loop.mActiveWorkers--;
   }
}

void parallelInit(void)
{
    if (threads.size() > 0) return;
    int nThreads = maxThreadIndex();
    threadIndex = 0;
    for (int i = 0; i < nThreads - 1; ++i)
        threads.push_back(std::thread(workerThread, i+1));
}

void parallelCleanup(void)
{
    if (threads.size() == 0) return;

    {
        std::lock_guard<std::mutex> lock(workListMutex);
        shutdowThread = true;
        workListCondition.notify_all();
    }

    for (std::thread &thread : threads) thread.join();
    threads.erase(threads.begin(), threads.end());
    shutdowThread = false;
}

