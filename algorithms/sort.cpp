/*
 * Copyright (C) 2014-2015  Yaochuang Ding - <ych_ding@163.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to 
 * deal in the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution, and in the same 
 *    place and form as other copyright, license and disclaimer information.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */
 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <vector>
#include <unordered_map>

#include <glog/logging.h>

#include "profiler.h"

//#define ENTER_FUNCTION logger << ">>>>" << __FUNCTION__ << std::endl
//#define EXIT_FUNCTION  logger << "<<<<" << __FUNCTION__ << std::endl

#define ENTER_FUNCTION 
#define EXIT_FUNCTION 

#define BUFSIZE 1024

#define PERFORMANCE_METER

typedef int DataType;

#define DATA_SET_SIZE (100 * 10000)
int gRawDataSet[DATA_SET_SIZE];
int gDataSetCount = 0;

/*************************************************
 * Function: Generate n random integers 
 *   
 * Param[in]:  int, number of random data. 
 *   
 * Retrun: void 
 * Ideas:  
 * Notice:   
*************************************************/
void prepare_random_data(int n)
{    
    ENTER_FUNCTION;
    
    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Generate random data");
    #endif

    srand(time(NULL)); 
    for (int i = 0; i < n; i++ )
    {        
        gRawDataSet[i] = rand() % (n * 10); // % DATA_SET_SIZE;
    }

    EXIT_FUNCTION;
}

/* 
 * generate m distinct sorted random number in [0, n) scope.
 *
 * rand(): Returns a pseudo-random integral number in the range [ 0, RAND_MAX]
 * This number is generated by an algorithm that returns a sequence 
 * of apparently non-related numbers each time it is called. 
 * This algorithm uses a seed to generate the series, which should be 
 * initialized to some distinctive value using function srand.
 * http://www.cplusplus.com/reference/cstdlib/rand/?kw=rand  for details.
 * 
 * double difftime (time_t end, time_t beginning)
 * Calculates the difference in seconds between beginning and end.
 * this is an inexact method to meter the performance!
 */
void gen_distinct_rand(int m, int n)
{
    ENTER_FUNCTION;
    
    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Generate distinct sorted random data");
    #endif

    int i, j;
    srand(time(NULL)); 
    for (i = 0; i < n; i++) //generate sorted random number i
    {            
        if (rand() % (n - i) < m)
        {    
            m--;
            gRawDataSet[gDataSetCount++] = i; 
            //logger << i << std::endl;      
        }
    }

    EXIT_FUNCTION;
}


/*************************************************
 * Function: Judge whether is an array is sorted. 
 *   
 * Param[in]:  array pointer 
 * Param[out]: int, array size 
 *   
 * Retrun: bool value depending on whether the array
 * is sorted.
 * Ideas:  
 * Notice:  User-defined types need to overload 
 * operator < and operator =
*************************************************/
template <class DataType>
static bool isArraySorted(const DataType a[], int n)
{
    CHECK(a != NULL && n > 1);
    for (int i = 0; i < n - 1; i++)
    {    
        if (a[i + 1] < a[i])
        { 
            return false;
        }
    }
    return true;
}

#if 0
/*
 * CAUTION: a b should not refer to the same location
 * The code should be avoid, it is unsafe.
 */
static void swap(DataType &a, DataType &b)
{
    //printf("%d <===> %d \n", a, b);
    a = a ^ b; //printf("a = %d\n", a);
    b = a ^ b; //printf("b = %d\n", b);
    a = a ^ b; //printf("a = %d\n", a);
}
#endif

template <class DataType>
static void swap(DataType &a, DataType &b)
{
   DataType temp = a; a = b; b = temp;
}

/* print element, each line 10 elements. */
template <class DataType>
static void display(DataType a[], int n)
{
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (int i = 0; i < n; i++)
    {
       // if ((i) % 10 == 0) printf("+");
        printf("%4d ", a[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

/*************************************************
 * Function: Simple insert sort 
 *   
 * Param[inout]:  array 
 * Param[in]:  int, array size 
 *   
 * Retrun: void 
 * Ideas:  
 * Notice: Sorted array is in asending order. 
*************************************************/
template <class DataType>
void simpleInsertSort(DataType a[], int n)
{
    CHECK(a != NULL && n > 1);
   
    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Simple Insert Sort");
    #endif

    int i, j;
    for (j = 1; j < n; j++) //< [j, n) is unsorted
    {
        DataType v = a[j];
        for (i = j - 1; i >= 0; i--) //< [0, j-1] is sorted
        {    
            if (v < a[i])
            {    a[i + 1] = a[i];  }
            else 
            {    break;    }
        }
        a[i + 1] = v; /* right place for a[j] */
    }
}

/*************************************************
 * Function: Bubble Sort 
 *   
 * Param[out]: array 
 * Param[in]:  array size 
 *   
 * Retrun: void 
 * Ideas:  
 * Notice: Sorting in asending order 
*************************************************/
template<class DataType>
void bubble_sort(DataType a[], int n)
{
    CHECK(a != NULL && n > 1);

    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Bubble Sort");
    #endif

    int i, j, sorted = 0;
    for (i = n - 1; !sorted && i > 0; i--) /* each iteration find the right element to a[i]*/
    {
        sorted = 1; /* before checking, suppose [0, i-1] is sorted */
        for (j = 0; j < i; j++)
        {
            if (a[j] > a[j + 1]) /* unsorted element */
            {
                swap(a[j], a[j + 1]);
                sorted = 0;
            }
        }
    }
}

/*************************************************
 *  quick sort helper.
 *  no need to check parameters.

 *  do actual quick sorting here by recursion.
 *  partition elements in a[p,q] into two parts.
*************************************************/
/***************************************************************
 * Function: It is a recursion function.
 *           It is a quick sort helper.
 *   
 * Param[inout]: array  
 * Param[in]: array's left side index   
 * Param[in]: array's right side index   
 *   
 * Retrun: void 
 * Ideas: 
 *       divides the array into two parts by a pivot recursively.
 * Notice:
 *       recursion terminate condition 
***************************************************************/
template <class DataType>
static void partition(DataType a[], int p, int q)
{
    if (p >= q) return; //< recursion terminate condition 
    DataType target = a[p]; //< pivot be selected randomly is better
    int i =p, j = q + 1;

    //< initial condition: a[p, i] < target, [j, inf] > target
    while(true)
    {
        do {i++;} while(i <= q && a[i] < target);
        do {j--;} while(a[j] > target);
        if (i > j) break;
        if (i != j) swap(a[i], a[j]);
        //for (int k = p; k <= q; k++) printf("%d ", a[k]);
        //printf("\n");
    }
    /* a[j] <= target, a[j] is the right place for pivot */
    if (j != p)
    {  
        //LOG(INFO) << "put " << target << " in " << j << " position." << std::endl;
        swap(a[j], a[p]); 
    }
    
    #ifdef ADEBUG
    logger << "[ ";
    for (int k = p; k <= q; k++) logger << a[k] << " ";
    logger << "]";
    #endif
    
    partition(a, p, j - 1);
    partition(a, j + 1, q);
}

/*************************************************
 *  quick sort template.
 *  NOTE: quick sort is very fast. so to meter the
 *        performance, call time() is not practical.
*************************************************/
/*************************************************
 * Function: Quick sort 
 *   
 * Param[inout]: array   
 * Param[out]:   array size 
 *   
 * Retrun: void 
 * Ideas:  
 * Notice:   
 *  clock() : RETURN The number of clock ticks
 *  elapsed since an epoch related to the particular
 *  program execution. So when program is sleeping
 *  the time is not caculated! CLOCKS_PER_SEC is
 *  involved when using second as unit.
*************************************************/
template <class DataType>
void quick_sort(DataType a[], int n)
{
    CHECK(a != nullptr && n > 1);

    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Quick Sort");
    #endif
    
    partition(a, 0, n - 1); /* sort array recursivly by partition */
}

/********************************************************************
 *  Priority Queue
 *  It is a MIN Heap.
 *  Interfaces:
 *  1. insert().
 *  2. extract_min().
 *  NOTE: Implementation in class declaration will be regarded as inline function.
 *        template should be put into header files.
********************************************************************/
template <class T> 
class PriQueue
{
private:
    T *_p;  //< should we use a unique_ptr<T[]> here ?
    int _n, _maxsize;
    void swap(int i, int j)
    { 
        T temp = _p[i]; _p[i]  = _p[j]; _p[j]  = temp;
    }

public:
    PriQueue(int m)
    : _maxsize(m), _n(0)
    {
        _p = new T[_maxsize + 1]; //< how about allocation fail ?
    }

    ~PriQueue()
    {
        delete[] _p; //< delete a nullptr is valid
    }
    
    void insert(T t) 
    {
        int i, p;
        _p[++_n] = t; /* insert element from tail and shift up to maintain heap structure */
        for (i = _n; i > 1 && _p[p = i / 2] > _p[i]; i = p) { swap(p, i); }
    }

    T extract_min()
    {
        T t = _p[1]; /* min value is at _p[1] */
        _p[1] = _p[_n--]; /* override _p [1] with element at tail and shift down to maintain */
        int i, c;
        for (i = 1; (c = 2 * i) <= _n; i = c)
        {
            if ( (c + 1) <= _n && _p[c + 1] < _p[c]) c++; /* find min child */
            if ( _p[i] < _p[c]) break; /* heap maintain ok */
            swap(i, c);
        }
        return t;
    }
};

/*************************************************
 *  heap sort based on priority queue.
 *  algorithm ideas:
 *  1. read data & build priority queue.
 *  2. extract min element from queue one by one
 *     & write value to an array.
*************************************************/
template <class DataType>
void heapSort(DataType a[], int n)
{
    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Heap Sort");
    #endif

    CHECK(a != NULL && n > 1);
    PriQueue<DataType> prique(n);
    for (int i = 0; i < n; i++)
    {
        prique.insert(a[i]);
        //LOG(INFO) << "Insert " << a[i] << " into min heap." << std::endl;
    }
    for (int i = 0; i < n; i++)
    {
        a[i] = prique.extract_min();
        //LOG(INFO) << "Extract " << a[i] << " from min heap." << std::endl;
    }
}

#define BIT_SORT_MASK   0X1F   //
#define BIT_SORT_SHIFT     5   // 2 ^ 5 == 32
#define BIT_PER_WORD      32
#define MAX_INPUT_NUM     (1000 * 10000)
#define BIT_MAP_SIZE      ((31 + MAX_INPUT_NUM) / BIT_PER_WORD)
int BitMap[BIT_MAP_SIZE] = { 0 };

void set(unsigned int i) {             BitMap[i >> BIT_SORT_SHIFT] |=  (1 << (i & BIT_SORT_MASK));}
void clr(unsigned int i) {             BitMap[i >> BIT_SORT_SHIFT] &= ~(1 << (i & BIT_SORT_MASK));}
int test(unsigned int i) { return      BitMap[i >> BIT_SORT_SHIFT] &   (1 << (i & BIT_SORT_MASK));}

/*********************************************************************
 *  bit sort requires that NO duplicate elements in unsorted set.
 *  algorithm ideas:
 *  1. read unsorted set & build the bit map
 *  2. check the each bit in bit map and output element according to checking bit.
*********************************************************************/
void bit_sort(DataType a[], int n)
{
    #ifdef PERFORMANCE_METER
	core::CPUProfiler profiler("Bit Sort");
    #endif

    CHECK(a != NULL && n > 1);
	memset(BitMap,0,sizeof(BitMap));

    for (int i = 0; i < n; i++)
    { 
        set(a[i]); 
    }

	int k = 0;
    for (int i = 0; i < MAX_INPUT_NUM; i++)
    { 
        if (test(i)) 
        { 
            //printf(" %d ", i); 
			a[k++] = i;
        }
    }
	if (k < n);
	{
		LOG(INFO) << "There are duplicate " << (n-k) << " elements in array.";
	}
}

/*************************************************
 *  ListNode struct.
 *  NOTE: member is public.
*************************************************/
struct ListNode 
{
    int val;
    ListNode *next;
    
    ListNode(int x = 0) 
    : val(x) , next(nullptr)
    {}
};

/*************************************************
 *  MergeSort class.
 *  implement merge sort algorthm on linked list.
*************************************************/
class MergeSort
{
private:
    /*************************************************
     *  merge two sorted linked list.
    *************************************************/
    ListNode* merge(ListNode *a, ListNode *b)
    {
        ListNode node(0), *t = &node; //helper varible
        while (a && b)
        {
            if (a->val < b->val)
            {
                t->next = a; a = a->next;
                t = t->next;
            }
            else
            {
                t->next = b; b = b->next;
                t = t->next;
            }
        }
        t->next = a ? a : b;
        return node.next;
    }

    /**********************************************************
     * find median of a linked list.
     * [1 2 3] return 2
     * [1 2] return 1
	 * result is supposed to [head, median], [median+1, tail]
    **********************************************************/   
    ListNode* median(ListNode *h)
    {
        if (!h || !h->next) //< empty Node or single Node
        {
            return nullptr; 
        }
        ListNode *slow, *fast, *prv;
        slow = fast = h;
        prv = nullptr;
        while (fast)
        {
            prv = slow;
            slow = slow->next;
            fast = fast->next ? fast->next->next : nullptr;
        }
        return prv;
    }
    
public:
    /*************************************************
     * implement merge sort.
	 * It is based on single Linked List
	 * It is a recursive implementation.
    *************************************************/
    ListNode *sortList(ListNode *head) 
    {
		#ifdef PERFORMANCE_METER
		core::CPUProfiler profiler("Merge Sort");
		#endif

        if (!head || !head->next) //< It is a termination condition
        {
            return head;
        }
        ListNode *mid  = median(head);
        ListNode *back = mid->next; mid->next = NULL;
        ListNode *front = head;
        return merge(sortList(front), sortList(back)); //recursion    
    }
};

/*************************************************
 *  Helper
 *  build a single linked list by order.
 *  [1 2 3] ---> {1-->2-->3}
*************************************************/
ListNode* buildList(const std::vector<int> &nums)
{
    ListNode node(0), *tempTail = &node; //< helper variable
    for (unsigned int i = 0; i < nums.size(); i++)
    {
        ListNode *temp = new ListNode(nums[i]);
        if (temp)
        {
            tempTail->next = temp;
            tempTail = temp;
        }
        else
        {
			LOG(FATAL) << "Out of memory.";
            return node.next;
        }
    }
    return node.next;
}

/*************************************************
 *  Test Function
 *  check correctness of merge sort.
*************************************************/
void mergeSortTester(int n)
{
    CHECK(n > 0);
    std::vector<int> nums(n, 0);
    for (int i = 0; i < n; i++) //generate data 
    {
        nums[i] = rand() % n;
    }
    ListNode *head = buildList(nums);
    MergeSort ms;
    ListNode *head2 = ms.sortList(head); //apply algorithm
    while (head2 && head2->next) //check result
    {
        if (head2->val > head2->next->val)
        {
			LOG(ERROR) << "Merge Sort Fails, Unsorted " << head2->val << " " << head2->next->val;
            return;
        }
        head2 = head2->next;
    }
	LOG(INFO) << "Merge Sort algorithm correct. ";
    //release resources
    
}

/*************************************************
 *  Tester
 *  test algorithms based on array.
*************************************************/
enum ESortType //can improve with c++11
{
    SORT_TYPE_SIMPLE_INSERT = 0,
    SORT_TYPE_BUBBLE,
    SORT_TYPE_QUICK_SORT,
    SORT_TYPE_BIT_SORT,
    SORT_TYPE_Heap_SORT,
    SORT_TYPE_Merge_SORT,
    SORT_TYPE_COUNT,
};

const char* sort_type_name [ESortType::SORT_TYPE_COUNT] = 
{
    "Simple insert Sort",
    "Bubble Sort",
    "Quick Sort",
    "Bit Sort",
    "Heap Sort"
};

static std::unordered_map<ESortType, std::string> gSortAlgorithmNames{
    {SORT_TYPE_SIMPLE_INSERT, "Simple Insert Sort"},
    {SORT_TYPE_BUBBLE, "Bubble Sort"},
    {SORT_TYPE_QUICK_SORT, "Quick Sort"},
    {SORT_TYPE_BIT_SORT, "Bit Sort"},
    {SORT_TYPE_Heap_SORT, "Heap Sort"},
    {SORT_TYPE_Merge_SORT, "Merge Sort"},
};

typedef void (*SortFunction)(DataType a[], int n);

SortFunction sort_func_tbl[SORT_TYPE_COUNT] = 
{
    simpleInsertSort,
    bubble_sort,
    quick_sort,
    bit_sort,
    heapSort,
};


void SortTest(ESortType typeSort, int numElement, int numIteration)
{
    CHECK(typeSort >= SORT_TYPE_SIMPLE_INSERT && typeSort < SORT_TYPE_COUNT);
    LOG(INFO) << "type = " << gSortAlgorithmNames[typeSort] << " Iteration = " << numIteration << " Elements = " << numElement << std:: endl;

    if (typeSort == SORT_TYPE_Merge_SORT) //< test algorithm based on linked list
    {
        for (int i = 0; i < numIteration; i++)
        {
            mergeSortTester(numElement);
        }
    }
    else  //test algorithm based on array.
    {
        for (int i = 0; i < numIteration; i++)
        {
            prepare_random_data(numElement);
            printf("Before\t Sorted ?  %d\n", isArraySorted(gRawDataSet,numElement));
            //display(gRawDataSet,n);
            sort_func_tbl[typeSort](gRawDataSet, numElement);
            printf("After\t Sorted ?  %d\n", isArraySorted(gRawDataSet,numElement));
            //display(gRawDataSet,n);
            printf("=============================================================\n");    
        }
    }
}

//< It's just a definition
std::unordered_map<std::string, core::ProfilerEntry> core::CPUProfiler::ProfilerData;

/*************************************************
 *  Tester
 *  a simple tester
*************************************************/
int main(int argc, char** argv)
{
    ENTER_FUNCTION;

    if (argc != 4)
    {
		LOG(ERROR) << "Usage:" << argv[0] << " [sort type] [input size] [iteration number]";
        return -1;
    }
    int typeSort, numElement, numIteration;
    typeSort = atoi(argv[1]);
    numElement = atoi(argv[2]);
    numIteration = atoi(argv[3]);
	CHECK(numElement <= DATA_SET_SIZE);
	CHECK(numElement > 0 && numIteration > 0 && typeSort < ESortType::SORT_TYPE_COUNT);

	core::CPUProfiler::begin();

	for (int i = 0; i < ESortType::SORT_TYPE_COUNT; ++i)
		SortTest(ESortType(i), numElement, numIteration);

	LOG(INFO) << core::CPUProfiler::end();
    EXIT_FUNCTION;
    return 0;
}

