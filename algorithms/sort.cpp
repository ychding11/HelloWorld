#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>

using namespace std;

#define BUFSIZE 1024
#define LINE_TO_STRING(x) #x 
#define FILE_AND_LINE (__FILE__":"LINE_TO_STRING(__LINE__)) 

#define PERFORMANCE_METER

typedef enum tagLogLevel
{
LOG_LEVEL_ERR,
LOG_LEVEL_INFO,
LOG_LEVEL_DBG,
LOG_LEVLE_ALL,
} LogLevel;

LogLevel gCurLoglevel = LOG_LEVLE_ALL;

#define LOG_D(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_DBG) fprintf(stdout,"[ DEBUG ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_E(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_ERR) fprintf(stdout,"[ ERROR ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_I(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_INFO) fprintf(stdout,"[ INFO ] "fmt,##__VA_ARGS__ ); } while(0)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

typedef int DataType;

#define DATA_SET_SIZE (100 * 10000)

int gRawDataSet[DATA_SET_SIZE];
int gDataSetCount = 0;

void prepare_random_data()
{    
    LOG_I("+[ %s ]\n", __FUNCTION__);
    
    #ifdef PERFORMANCE_METER
    time_t tm1, tm2;
    time(&tm1); /* get current time */
    #endif

    srand(time(NULL)); 
    for (int i = 0; i < DATA_SET_SIZE; i++ )
    {        
        gRawDataSet[i] = rand();// % DATA_SET_SIZE;
    }

    #ifdef PERFORMANCE_METER
    time(&tm2);
    double seconds = difftime(tm2, tm1); /* return double */
    printf("[Prepare data time] = %.lf seconds!\n", seconds);
    #endif

    LOG_I("-[ %s ]\n", __FUNCTION__);
}

static bool is_sorted(DataType a[], int n)
{
    assert(a != NULL && n > 1);
    for (int i = 0; i < n - 1; i++)
    {    if (a[i] > a[i + 1]) return false;
    }
    return true;
}

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

void simple_insert_sort(DataType a[], int n)
{
    LOG_I("+[ %s ]\n", __FUNCTION__);
    int i, j;
    assert(a != NULL && n > 1);

    #ifdef PERFORMANCE_METER
    time_t tm1, tm2;
    time(&tm1); /* get current time */
    #endif

    /* do sorting here */
    for (j = 1; j < n; j++)
    {
        DataType temp = a[j];
        for (i = j - 1; i >= 0; i--)
        {    /* find a right place for a[j] */
            if (temp < a[i])
            {    a[i + 1] = a[i];  }
            else
            {    break;    }
        }
        a[i] = temp;
    }

    #ifdef PERFORMANCE_METER
    time(&tm2);
    double seconds = difftime(tm2, tm1); /* return double */
    printf("[Simple insert sort time] = %.lf seconds!\n", seconds);
    #endif

    LOG_I("-[ %s ]\n", __FUNCTION__);
}

void bubble_sort(DataType a[], int n)
{
    LOG_I("+[ %s ]\n", __FUNCTION__);
    int i, j;
    assert(a != NULL && n > 1);

    #ifdef PERFORMANCE_METER
    time_t tm1, tm2;
    time(&tm1); /* get current time */
    #endif

    /* do sorting here */
    int sorted = 0;
    for (i = n - 1; !sorted && i > 0; i--)
    {
        sorted = 1; 
        for (j = 0; j < i; j++)
        {
            if (a[j] > a[j + 1]) 
            {
                swap(a[j], a[j + 1]);
                sorted = 0;

            }
        }
    }

    #ifdef PERFORMANCE_METER
    time(&tm2);
    double seconds = difftime(tm2, tm1); /* return double */
    printf("[bubble sort time] = %.lf seconds!\tsorted %d elements!\n", seconds, n);
    #endif

    LOG_I("-[ %s ]\n", __FUNCTION__);
}

/* this is an internal function to help quick sort which cannot called from outside
 * so no need to check parameters.
 * do actual sorting here.
 * a[p] a[q] */
static void partition(DataType a[], int p, int q)
{
    if (p >= q) return; //end condition of recursion

    DataType target = a[p]; /* target can be selected by random */
    int i, j;
    i = p, j = q + 1;  /* initial : a[p, i] < target, [j, inf] > target*/
    while(1)
    {
        do {i++;} while(i <= q && a[i] < target);
        do {j--;} while(a[j] > target);
        if (i > j) break;
        if (i != j) swap(a[i], a[j]);
        //for (int k = p; k <= q; k++) printf("%d ", a[k]);
        //printf("\n");
    }
    if (j != p)    swap(a[j], a[p]); /* find the right place for a[p] or target !*/
    //printf("[>>> ");
    //for (int k = p; k <= q; k++) printf("%d ", a[k]);
    //printf(" ]\n");
    partition(a, p, j - 1);
    partition(a, j + 1, q);
}

/* clock() : The number of clock ticks elapsed since an epoch related to the particular program execution. so when the 
 * program is sleeping the time is not caculated! 
 * CLOCKS_PER_SEC is involved when caculating runtime in second unit.
 * quick sort is very fast. so to meter the perfomance time() is not practical.
 */
void quick_sort(DataType a[], int n)
{
    LOG_I("+[ %s ]\n", __FUNCTION__);
    int i, j;
    assert(a != NULL && n > 1);

    #ifdef PERFORMANCE_METER
    clock_t clk1, clk2;
    clk1 = clock(); /* get current clcok ticks elapsed since epoch */
    #endif
        
    partition(a, 0, n - 1); /* do sorting */

    #ifdef PERFORMANCE_METER
    clk2 = clock();
    float seconds = ((float)(clk2 - clk1)) / CLOCKS_PER_SEC; /* calculate in seconds units */
    printf("[quick sort time] = %d ticks, %.4f seconds!\tsorted %d elements!\n", clk2 - clk1, seconds, n);
    #endif

    LOG_I("-[ %s ]\n", __FUNCTION__);
}

/* generate m distinct sorted random number in [0, n) scope 
 * rand(): Returns a pseudo-random integral number in the range between 0 and RAND_MAX
 * This number is generated by an algorithm that returns a sequence of apparently non-related numbers each time it is called. 
 * This algorithm uses a seed to generate the series, which should be initialized to some distinctive value using function srand.
 * http://www.cplusplus.com/reference/cstdlib/rand/?kw=rand  for details.
 * 
 * double difftime (time_t end, time_t beginning); Calculates the difference in seconds between beginning and end.
 * this is an inexact method to meter the performance!
 */
void gen_distinct_rand(int m, int n)
{
    int i, j;
    LOG_I("+[ %s ]\n", __FUNCTION__);

    #ifdef PERFORMANCE_METER
    time_t tm1, tm2;
    time(&tm1); /* get current time */
    #endif

    srand(time(NULL)); /* initialize random seed */
    for (i = 0; i < n; i++) //generate sorted random number i
    {            
        if (rand() % (n - i) < m)
        {    
            gRawDataSet[gDataSetCount++] = i; 
            printf("%u\n", i); 
            m--;
        }
    }

    #ifdef PERFORMANCE_METER
    time(&tm2);
    double seconds = difftime(tm2, tm1); /* return double */
    printf("[Run time] %.lf s!\n", seconds);
    #endif

    LOG_I("-[ %s ]\n", __FUNCTION__);
}

#define BIT_SORT_MASK   0X1F   //
#define BIT_SORT_SHIFT     5   // 2 ^ 5 == 32
#define BIT_PER_WORD      32
#define MAX_INPUT_NUM     (1000 * 10000)
#define BIT_MAP_SIZE      (1 + MAX_INPUT_NUM / BIT_PER_WORD)
int BitMap[BIT_MAP_SIZE] = { 0 };

void set(unsigned int i) {             BitMap[i >> BIT_SORT_SHIFT] |=  (1 << (i & BIT_SORT_MASK));}
void clr(unsigned int i) {             BitMap[i >> BIT_SORT_SHIFT] &= ~(1 << (i & BIT_SORT_MASK));}
int test(unsigned int i) { return      BitMap[i >> BIT_SORT_SHIFT] &   (1 << (i & BIT_SORT_MASK));}

/* bit sort requires that no duplicate element in unsorted set
 * 1. read unsorted set & build the bit map
 * 2. check the bit map orderly, output element according to checking
 * */
void bit_sort(DataType a[], int n)
{
    
    for (int i = 0; i < n; i++)
    { set(a[i]); }
    for (int i = 0; i < MAX_INPUT_NUM; i++)
    { 
        if (test(i)) 
        { printf(" %d ", i); }
    }
}

enum 
{
    SORT_TYPE_SIMPLE_INSERT,
    SORT_TYPE_BUBBLE,
    SORT_TYPE_QUICK_SORT,
    SORT_TYPE_BIT_SORT,
    SORT_TYPE_COUNT,
};
const char* sort_type_name [SORT_TYPE_COUNT] = 
{
    "Simple insert Sort",
    "Bubble Sort",
    "Quick Sort",
    "Bit Sort",
};

typedef void (*SortFunction)(DataType a[], int n);

SortFunction sort_func_tbl[SORT_TYPE_COUNT] = 
{
    simple_insert_sort,
    bubble_sort,
    quick_sort,
    bit_sort,
};

static void display(DataType a[], int n)
{
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    for (int i = 0; i < n; i++)
    {
        if ((i) % 10 == 0) printf("+");
        printf("%4d ", a[i]);
        if ((i + 1) % 10 == 0) printf("\n");
    }
    printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

/* This a priority queue 
 * class template definition 
 * */
template <class T>
class PriQueue
{
private:
    T *_p;
    int _n, _maxsize;
    void swap(int i, int j)
    { T temp = _p[i]; _p[i] = _p[j]; _p[j] = temp;    }

public:
    PriQueue(int m):_maxsize(m), _n(0){_p = new T[_maxsize + 1];}

    void insert(T t) /* implementation in class definition will be inline */
    {
        int i, p;
        _p[++_n] = t; /* insert elements from tail, shift up to maintain the heap structure */
        for (i = _n; i > 1 && _p[p = i / 2] > _p[i]; i = p) 
        { swap(p, i); }
    }

    T extract_min()
    {
        int i, c;
        T t = _p[1];
        _p[1] = _p[_n--]; /* remove the min containing in _p[1], and replace it with elements in tail, shit down to maintain */
        for (i = 1; (c = 2 * i) <= _n; i = c)
        {
            if ( (c + 1) <= _n && _p[c + 1] < _p[c]) c++;
            if ( _p[i] < _p[c]) break;
            swap(i, c);
        }
        return t;
    }
};

int main(int argc, char** argv)
{
    int m, n;
    LOG_I("+[ %s ]\n", __FUNCTION__);

    if (argc != 3)
    {
        printf("usage error!\n"
                "%s [sort type]  [iteration number]\n", argv[0]); 
        return -1;
    }
    m = atoi(argv[1]);
    n = atoi(argv[2]);
    LOG_D("%s %d times\n", sort_type_name[m], n);
    for (int i = 0; i < n; i++)
    {
        prepare_random_data();
        printf("Before\t Sorted ?  %d\n", is_sorted(gRawDataSet,DATA_SET_SIZE));
        //display(gRawDataSet,DATA_SET_SIZE);
        sort_func_tbl[m](gRawDataSet, DATA_SET_SIZE);
        printf("After\t Sorted ?  %d\n", is_sorted(gRawDataSet,DATA_SET_SIZE));
        //display(gRawDataSet,DATA_SET_SIZE);
        printf("=============================================================\n");    
    }
    LOG_I("-[ %s ]\n", __FUNCTION__);
    return 0;
}

