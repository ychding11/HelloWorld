#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <ctime>
#include <cassert>
using namespace std;

#define BUFSIZE 1024
#define LINE_TO_STRING(x) #x
#define FILE_AND_LINE (__FILE__":"LINE_TO_STRING(__LINE__))
typedef enum tagLogLevel
{
LOG_LEVEL_ERR,
LOG_LEVEL_INFO,
LOG_LEVEL_DBG,
LOG_LEVLE_ALL,
} LogLevel;

LogLevel gCurLoglevel = LOG_LEVEL_INFO;

#define LOG_D(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_DBG) fprintf(stdout,"[ DEBUG ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_E(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_ERR) fprintf(stdout,"[ ERROR ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_I(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_INFO) fprintf(stdout,"[ INFO ] "fmt,##__VA_ARGS__ ); } while(0)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

uint8_t to_lowcase(char ch)
{
	if (ch >= 'a' && ch <= 'z') return ch;
	else if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 'a';
        else return -1;
}

/* use a function template will be better! */
void swap(int &a, int &b)
{
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}

/* fgets takes newline as a valid character */
static uint32_t count_line(char *filename)
{
	int lines = 0;
	char buf[BUFSIZE] = { 0 };
  	FILE *fp = NULL;
	fp = fopen(filename, "r");
	if (NULL == fp)
	{
		printf("Open file error! [ %s ]\n", filename);
		return -1;
	}
	while (fgets(buf, BUFSIZE, fp)) 
	{
		if ( strlen(buf) == BUFSIZE - 1 && 
		     buf[BUFSIZE-2] != '\n')
		{ /* do nothing , no new line in buffer */}
		else
		{ lines++; }
	}
	return lines;
}

/* next is a string hash used to count world frequency */
typedef struct node{
char *word;
int count;
struct node *next;
} node;

#define NHASH 29989  // a prime 
node *bucket[NHASH];
#define MULT 31 /* ?? */

static unsigned int hash(char *str)
{
	unsigned int n = 0;
	for (; *str; str++)
	{ n = MULT * n + *str; }
	return n % NHASH;
}

/* insert a word into hash bucket, the hash function is above hash() */
void insert_word(char *word)
{
	assert(word != NULL);
	unsigned int index = hash(word);
	node *p;
	for(p = bucket[index]; p != NULL; p = p->next)
	{
		if (strcmp(p->word, word) == 0) { p->count++; return;}
	}
	p = (node*)malloc(sizeof(struct node));
	p->count = 1;
	p->word = (char*)malloc(sizeof(word) + 1);
	strcpy(p->word, word);
	p->next = bucket[index];
	bucket[index] = p;
}

/* here we define the world : strings seperated by blank perhase it may not 
 * be a valid word having meanings 
 *
 */
static void count_word_freq()
{	
	char buf[64];  /* assume the longest world is 63 character */
	memset(bucket, 0, sizeof(bucket)); /* clear the bucket before handle */
	while (scanf("%s", buf) != EOF) /* use io redirection to test functionality */
		insert_word(buf);
	for (int i = 0; i < NHASH; i++)
	{
		for (node *p = bucket[i]; p != NULL; p = p->next)
		{ printf("%s\t%d\n", p->word, p->count);}
	}
	printf("+++++++++++++++++ Count Word Frequency Complete +++++++++++++++++++++++\n");
}


int main(int argc, char** argv)
{
 
  LOG_I("+[ %s ]\n", __FUNCTION__);
  count_word_freq();
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

