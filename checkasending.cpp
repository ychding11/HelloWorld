#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <set>
using namespace std;

#define LINE_TO_STRING(x) #x 
#define FILE_AND_LINE (__FILE__":"LINE_TO_STRING(__LINE__)) 

#define PERFORMANCE_METER

typedef enum tagLogLevel
{
LOG_LEVEL_ERR,
LOG_LEVEL_INFO,
LOG_LEVEL_DBG,
LOG_LEVEL_ALL,
} LogLevel;

LogLevel gCurLoglevel = LOG_LEVEL_ERR;

#define LOG_D(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_DBG) fprintf(stdout,"[ DEBUG ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_E(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_ERR) fprintf(stdout,"[ ERROR ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_I(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_INFO) fprintf(stdout,"[ INFO ] "fmt,##__VA_ARGS__ ); } while(0)



/* reference location:
 * http://www.cplusplus.com/reference/cstdio/scanf/?kw=scanf
 */
int check_asending(void)
{
	int preNum,curNum;
	preNum = 0;
	while (scanf("%d", &curNum) != EOF)
	{
		if (curNum < preNum)
		{
			printf("%d %d \t NOT asending!\n", preNum, curNum);
			return 0; //disorder here
		}
		preNum = curNum;
		
	}
	return 1;
}


int main(int argc, char** argv)
{  
  int ret = check_asending();
  printf("asending: %d\n", ret);
  return 0;
}

