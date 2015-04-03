#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
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

#define DATA_SET_SIZE (100 * 1000)
int gRawDataSet[DATA_SET_SIZE];
int gDataSetCount = 0;

/* generate m distinct random number in [0, n) scope 
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
	for (i = 0; i < n; i++)
	{	
		srand(time(NULL)); /* initialize random seed */
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


int main(int argc, char** argv)
{
  int m, n;
  LOG_I("+[ %s ]\n", __FUNCTION__);
  printf("%s\n",FILE_AND_LINE);
  if (argc != 3)
  {
    	printf("usage error!\n"
		"%s m n \n", argv[0]); 
   	 return -1;
  }
	m = atoi(argv[1]);
	n = atoi(argv[2]);
	LOG_D("%d\t%d\n", m, n);
  	gen_distinct_rand(m, n);
 
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

