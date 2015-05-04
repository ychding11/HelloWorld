#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <ctime>
#include <cassert>
using namespace std;


typedef enum tagLogLevel
{
LOG_LEVEL_ERR,
LOG_LEVEL_INFO,
LOG_LEVEL_DBG,
LOG_LEVLE_ALL,
} LogLevel;

LogLevel gCurLoglevel = LOG_LEVEL_ERR; //set the current log level

#define LOG_D(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_DBG ) fprintf(stdout,"[ DEBUG ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_E(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_ERR ) fprintf(stdout,"[ ERROR ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_I(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_INFO) fprintf(stdout,"[ INFO ]  "fmt,##__VA_ARGS__ ); } while(0)

typedef int DataType;

#define SEARCH_BUFFER_SIZE (100 * 10000)
DataType a[SEARCH_BUFFER_SIZE];
int n = 0;

int binSearch(DataType t)
{
	int l, u, m;
	int idx;
	l = -1; u = n;
	while (l + 1 != u)
	{
		m = (l + u) / 2;
		if (a[m] < t)
		{	l = m; }
		else
		{	u = m; }
	}
	idx = u;
	if (idx >= n || a[idx] != t)
	{	return -1;	}
	return idx;
}

void prepareData()
{
	DataType temp;
	while (scanf("%d", &temp) != EOF)
	{
		a[n++] = temp;
	}
}

int main(int argc, char** argv)
{ 
  LOG_I("+[ %s ]\n", __FUNCTION__);
  DataType a;
  if (argc != 1)
  {
  	printf("Usage Error! %s target\n", argv[0]);
  }
  a = atoi(argv[1]);
  prepareData();
  int ret = binSearch(a);
  if (ret < 0) printf("%d does not exist in file!\n", a);
  else 		   printf("%d exist in file offset: %d\n", a, ret);
  
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

