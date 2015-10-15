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

#define MAX_WORD_LEN 128

int cmp_char(const void *a, const void *b)
{
	return *(char*)a - *(char*)b; // without type conversion
}

int main()
{
	char key[MAX_WORD_LEN], word[MAX_WORD_LEN];
	while ((scanf("%s", word)) != EOF)
	{
		strcpy(key, word);
		qsort(key, strlen(word), sizeof(char), cmp_char);
		printf("%s %s\n", key, word);
	}

}
