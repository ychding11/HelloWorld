#include "parallel.h"
#include "stats.h"
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include <iostream>
#include <cstdio>
#include <condition_variable>
#include <functional>
#include <cmath>
#include <cstdio>
#include <algorithm>

#include "pathtracer.h"

class ParallelForLoop; // forward declare.
static std::vector<std::thread> threads;
static std::mutex workListMutex;
static bool shutdowThread = false;
static std::condition_variable workListCondition;
static ParallelForLoop *workList = nullptr;

thread_local int threadIndex;

// Bookkeeping variables to help with the implementation of
// MergeWorkerThreadStats().
static std::atomic<bool> reportWorkerStats{ false };

// Number of workers that still need to report their stats.
static std::atomic<int> reporterCount;

// After kicking the workers to report their stats, the main thread waits
// on this condition variable until they've all done so.
static std::condition_variable reportDoneCondition;
static std::mutex reportDoneMutex;

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
        ,mChunkSize(chunkSize)
	{
		DLOG(INFO) << "ParallelForLoop, max idex=" << mMaxIndex << " Constructed.";
	}

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
		//DLOG(INFO) << "ParallelForLoop, max idex=" << mMaxIndex << " Finished.";
        return mNextIndex >= mMaxIndex && mActiveWorkers == 0;
    }

};

void workerThread(int tIndex)
{
	LOG(INFO) << "Thread " << tIndex << " starting.";
    threadIndex = tIndex;
    
    std::unique_lock<std::mutex> lock(workListMutex);
    while (!shutdowThread)
    {
		if (reportWorkerStats) //atomic
		{
			ReportThreadStats();
			if (--reporterCount == 0)
				// Once all worker threads have merged their stats, wake up
				// the main thread.
				reportDoneCondition.notify_one();
			// Now sleep again.
			workListCondition.wait(lock);
		}
       else if (!workList) 
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
			DLOG(INFO) << "Thread " << threadIndex << "starts work on [" << indexStart << ", " << indexEnd << "]";
            for (uint64_t i = indexStart; i < indexEnd; ++i)
            {
                if (loop.mFunc) loop.mFunc(i);
            }
            lock.lock();
            loop.mActiveWorkers--;
            if (loop.Finished()) workListCondition.notify_all();
       }
    }
	LOG(INFO) << "Thread " << tIndex << " exiting.";
}

// PUBLIC interface
void ParallelFor(std::function<void(int64_t)> func, int64_t count, int chunkSize)
{
   if (threads.size() == 0 || count < chunkSize) 
   {
       for (int64_t i = 0; i < count; ++i) func(i);
       return;
   }

   // Create ParallelFor object and add it into work list.
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

        lock.unlock(); // release lock after update loop parameter.
        
		DLOG(INFO) << "Main Thread " << "starts work on [" << indexStart << ", " << indexEnd << "]";
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
	LOG(INFO) << "Parallel module init Ok.";
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
	LOG(INFO) << "Parallel module cleanup Ok.";
}

void MergeWorkerThreadStats()
{
	std::unique_lock<std::mutex> lock(workListMutex);
	std::unique_lock<std::mutex> doneLock(reportDoneMutex);
	// Set up state so that the worker threads will know that we would like
	// them to report their thread-specific stats when they wake up.
	reportWorkerStats = true;
	reporterCount = threads.size();

	// Wake up the worker threads.
	workListCondition.notify_all();

	// Wait for all of them to merge their stats.
	reportDoneCondition.wait(lock, []() { return reporterCount == 0; });

	reportWorkerStats = false;
}