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

#define DATA_SET_SIZE (1000 * 10000)
int gRawDataSet[DATA_SET_SIZE];
int gDataSetCount = 0;

void prepare_random_data()
{	LOG_I("+[ %s ]\n", __FUNCTION__);
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
	{	if (a[i] > a[i + 1]) return false;
	}
	return true;
}

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
class PriQueue{
private:
	T *_p;
	int _n, _maxsize;
	void swap(int i, int j)
	{ T temp = _p[i]; _p[i] = _p[j]; _p[j] = temp;	}

public:
	PriQueue(int m):_maxsize(m), _n(0){_p = new T[_maxsize + 1];} /* heap sort : one extra space needed! */
	~PriQueue() { if (NULL != _p) delete [] _p; }
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

/* use template in template, */
template <class T>
static void prioque_sort(T a[], int m)
{
	LOG_I("+[ %s ]\n", __FUNCTION__);
	PriQueue<T> prique(m);		
	
	#ifdef PERFORMANCE_METER
	clock_t clk1, clk2;
	clk1 = clock(); /* get current clcok ticks elapsed since epoch */
	#endif

	for (int j = 0; j < m; j++)
	{	
		prique.insert(a[j]); 
	}
	for (int j = 0; j < m; j++)
	{	
		a[j] = prique.extract_min(); 
	}

	#ifdef PERFORMANCE_METER
	clk2 = clock();
	float seconds = ((float)(clk2 - clk1)) / CLOCKS_PER_SEC; /* calculate in seconds units */
	printf("[prique sort time] = %ld ticks, %.4f seconds!\tsorted %d elements!\n", clk2 - clk1, seconds, m);
	#endif

	LOG_I("-[ %s ]\n", __FUNCTION__);	
}

int main(int argc, char** argv)
{
  	int m, n;
  	LOG_I("+[ %s ]\n", __FUNCTION__);
  	printf(" %s:%d\n",__FILE__, __LINE__);
  	if (argc != 3)
  	{
    		printf("Usage Error!\n"
			"%s [sort type]  [test count]\n", argv[0]); 
   	 	return -1;
 	 }
	m = atoi(argv[1]);  /* data count*/
	n = atoi(argv[2]);  /* test count */
	LOG_D("%d data  %d times\n", m, n);

	for (int i = 0; i < n; i++)
	{
		PriQueue<int> prique(m);
		prepare_random_data();
		printf("Before\t Sorted ?  %d\n", is_sorted(gRawDataSet,m));
		//display(gRawDataSet,DATA_SET_SIZE);
		prioque_sort(gRawDataSet, m);
 		printf("After\t Sorted ?  %d\n", is_sorted(gRawDataSet,m));
		//display(gRawDataSet,DATA_SET_SIZE);
		printf("===================== Test Case: %d Complete ======================\n", i);	
	}
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

