#ifndef PARALLEL_H_
#define PARALLEL_H_

#include <functional>

int numSystemCores(void);
int maxThreadIndex(void);
void parallelInit(void);
void parallelCleanup(void);
void ParallelFor(std::function<void(int64_t)> func, int64_t count, int chunkSize);
void MergeWorkerThreadStats();

#endif