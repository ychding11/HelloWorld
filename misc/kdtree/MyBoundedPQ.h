/**************************************************************
 * File: MyBoundedPQ.h
 * Author: yaochuang.ding(ych_ding@163.com)
 *
 * An implementation of the bounded priority queue abstraction.
 * A bounded priority queue is in many ways like a regular priority
 * queue.  It stores a collection of elements tagged with a real-
 * valued priority, and allows for access to the element whose
 * priority is the smallest.  However, unlike a regular priority
 * queue, the number of elements in a bounded priority queue has
 * a hard limit that is specified in the constructor.  Whenever an
 * element is added to the bounded priority queue such that the
 * size exceeds the maximum, the element with the highest priority
 * value will be ejected from the bounded priority queue.  In this
 * sense, a bounded priority queue is like a high score table for
 * a video game that stores a fixed number of elements and deletes
 * the least-important entry whenever a new value is inserted.
 *
 * When creating a bounded priority queue, you must specify the
 * maximum number of elements to store in the queue as an argument
 * to the constructor.  For example:
 *
 * BoundedPQueue<int> bpq(15); // Holds up to fifteen values.
 *
 * The maximum size of the bounded priority queue can be obtained
 * using the maxSize() function, as in
 *
 * size_t k = bpq.maxSize();
 *
 * Beyond these restrictions, the bounded priority queue behaves
 * similarly to other containers.  You can query its size using
 * size() and check whether it is empty using empty().  You
 * can enqueue an element into the bounded priority queue by
 * writing
 *
 * bpq.enqueue(elem, priority);
 *
 * Note that after enqueuing the element, there is no guarantee 
 * that the value will actually be in the queue.  If the queue
 * is full and the new element's priority exceeds the largest
 * priority in the container, it will not be added.
 *
 * You can dequeue elements from a bounded priority queue using
 * the dequeueMin() function, as in
 *
 * int val = bpq.dequeueMin();
 *
 * The bounded priority queue also allows you to query the min
 * and max priorities of the values in the queue.  These values
 * can be queried using the best() and worst() functions, which
 * return the smallest and largest priorities in the queue,
 * respectively.
 */

#ifndef BOUNDED_PQUEUE_INCLUDED
#define BOUNDED_PQUEUE_INCLUDED

#include <map>
#include <algorithm>
#include <limits>

using namespace std;

template <typename T>
class BoundedPQueue
{
public:
    /**
     * Constructor: BoundedPQueue(size_t maxSize);
     * Usage: BoundedPQueue<int> bpq(15);
     * --------------------------------------------------
     * Constructs a new, empty BoundedPQueue with
     * maximum size equal to the constructor argument.
     */
    explicit BoundedPQueue(size_t maxSize);

    /**
     * void enqueue(const T& value, double priority);
     * Usage: bpq.enqueue("Hi!", 2.71828);
     * --------------------------------------------------
     * Enqueues a new element into the BoundedPQueue with
     * the specified priority. If this overflows the maximum
     * size of the queue, the element with the highest
     * priority will be deleted from the queue. Note that
     * this might be the element that was just added.
     */
    void enqueue(const T& value, double priority);

    /**
     * T dequeueMin();
     * Usage: int val = bpq.dequeueMin();
     * --------------------------------------------------
     * Returns the element from the BoundedPQueue with the
     * smallest priority value, then removes that element
     * from the queue.
     */
    T min();

    /**
     * size_t size() const;
     * bool empty() const;
     * Usage: while (!bpq.empty()) { ... }
     * --------------------------------------------------
     * Returns the number of elements in the queue and whether
     * the queue is empty, respectively.
     */
    size_t size() const;
    bool empty() const;
    
    /**
     * size_t maxSize() const;
     * Usage: size_t queueSize = bpq.maxSize();
     * --------------------------------------------------
     * Returns the maximum number of elements that can be
     * stored in the queue.
     */
    size_t maxSize() const;
    
    /**
     * double best() const;
     * double worst() const;
     * Usage: double highestPriority = bpq.worst();
     * --------------------------------------------------
     * best() returns the smallest priority of an element
     * stored in the container (i.e. the priority of the
     * element that will be dequeued first using dequeueMin).
     * worst() returns the largest priority of an element
     * stored in the container.  If an element is enqueued
     * with a priority above this value, it will automatically
     * be deleted from the queue.  Both functions return
     * numeric_limits<double>::infinity() if the queue is
     * empty.
     */
    double best()  const;
    double worst() const;

    bool hasNext();
    T    next() ;

private:
 
T    extractTop();
void insert(const T& value, double priority);

#define MAX_SIZE (512 + 1)
    const size_t maximumSize;
    size_t curIndex;
    size_t bestValueIndex;
    std::pair<double, T> elems[MAX_SIZE];
};


////////////////////////////////////////////////
// BoundedPQueue class implementation details //
////////////////////////////////////////////////

/*
 * Constructor accepts and stores the maximum size.
 */
template <typename T>
BoundedPQueue<T>::BoundedPQueue(size_t maxSize)
: maximumSize(maxSize)
, curIndex(0)
, bestValueIndex(0)
{
   if (MAX_SIZE < maximumSize); // fault
}

template <typename T>
T BoundedPQueue<T>::extractTop()
{
    std::pair<double, T> elem = elems[1];
    elems[1] = elems[curIndex--];
    size_t i, c;
    for (i = 1; (c = i << 1) <= curIndex; i = c)
    {
        if ((c + 1) <= curIndex && elems[c + 1] > elems[c]) ++c;
        if (elems[i] > elems[c]) break;
        std::pair<double, T> temp = elems[c];
        elems[c] = elems[i];
        elems[i] = temp;
    }
    return elem;
}

template <typename T>
void BoundedPQueue<T>::enqueue(const T& value, double priority)
{
    if (curIndex + 1 > maximumSize)
    {
        extractTop();
    }
    insert(value, priority);
}

template <typename T>
void BoundedPQueue<T>::insert(const T& value, double priority)
{
    // Add the element to max-top heap 
    std::pair<double, T> elem = std::make_pair(priority, value);
    elems[++curIndex] = elem;
    size_t i, p;
    for (i = curIndex; i > 1 && elems[p = i / 2].first < elems[i].first; i = p)
    {
        std::pair<double, T> temp = elems[p];
        elems[p] = elems[i];
        elems[i] = temp;
    }
}

/*
 * Please ensure queue is not empty, when call the function. 
 */
template <typename T>
T BoundedPQueue<T>::min()
{
    if (bestValueIndex <= 0)
    {
        best();
    }
    T result = elems[bestValueIndex].second; 
    return result;
}

template <typename T>
size_t BoundedPQueue<T>::size() const
{
    return curIndex;
}

template <typename T>
bool BoundedPQueue<T>::empty() const
{
    return curIndex == 0; 
}

template <typename T>
size_t BoundedPQueue<T>::maxSize() const
{
    return maximumSize;
}

template <typename T>
double BoundedPQueue<T>::best() const
{
    size_t index = 0;
    double bestValue = numeric_limits<double>::infinity();
    for (size_t i = curIndex / 2; i <= curIndex; ++i)
    {
        if (elems[i].first < bestValue)
        {
            index = i; 
            bestValue = elems[i].first;
        }
    }
    bestValueIndex = index;
    return bestValue;
}

template <typename T>
double BoundedPQueue<T>::worst() const
{
    return empty()? numeric_limits<double>::infinity() : elems[1].first;
}

template <typename T>
bool BoundedPQueue<T>::hasNext()
{
    return curIndex > 0;
}

template <typename T>
T  BoundedPQueue<>::next()
{
    return elems[curIndex--];
}

#endif // BOUNDED_PQUEUE_INCLUDED
