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

LogLevel gCurLoglevel = LOG_LEVEL_INFO; //set the current log level

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

/* use a function template will be better! 
 * CAUTION: a and b should not refer to the same location
 *
 */
void swap(int &a, int &b)
{
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}

/* fgets takes newline ('\n') as a valid character 
 * */
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

/* string hash node used to count world frequency 
 * */
typedef struct node{
char *word;
int count;
struct node *next;
} node;

#define NHASH 29989  // a prime number
node *bucket[NHASH];
#define MULT 31 /* ?? */

static unsigned int hash(char *str)
{
	unsigned int n = 0;
	for (; *str; str++)
	{ n = MULT * n + *str; }
	return n % NHASH;
}

/* insert a word into hash bucket, the hash function is above hash() 
 * the interface exposed to outside
 * */
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
	memset(bucket, 0, sizeof(bucket)); /* clear the bucket */
	while (scanf("%s", buf) != EOF) /* use io redirection to test functionality */
		insert_word(buf);
	for (int i = 0; i < NHASH; i++)
	{
		for (node *p = bucket[i]; p != NULL; p = p->next)
		{ printf("%s\t%d\n", p->word, p->count);}
	}
	printf("+++++++++++++++++ Count Word Frequency Complete +++++++++++++++++++++++\n");
}

/* This is an internal function, no parameter validy check!
 * return common part length from string beginning.
 */
static int common_len(const char *p, const char *q)
{
	assert(p != NULL && q != NULL);
	int i = 0;
	
	/* compare characters in strings p and q */
	while (*p && *p++ == *q++) i++;
	return i;
}

#define MAX_CHAR (100 * 10000)

/* define string suffix array 
 * char *suffix[]
 * */
char c[MAX_CHAR], *suffix[MAX_CHAR];

static int cmpstr(const void *a, const void *b)
{
	#if 1
	 char *c = *(char**)a;
	 char *d = *(char**)b;
	 int ret = 0;
	 for (; !(ret = *c - *d) && *c; c++, d++ );
	 return  ret;
	#else
	 return strcmp(c, d);  /* call string lib function */
	#endif
}

static void demo_suffix_array()
{
	int i = 0;
	char ch;
	while ((ch = getchar()) != EOF)
	{
		suffix[i] = &c[i];
		c[i++] = ch;
		//printf("%c \n",ch);
	}
	c[i] = '\0'; /* ending of the string */
	#ifdef DEBUG
	printf("%s\n", c);
	for (int j = 0; j < i; j++)
	{
		printf("%p\t%s", suffix[j], suffix[j]);
	}
	#endif
	
	/* sorting the suffix string, 
	 * void qsort (void* base, size_t num, size_t size, int (*compar)(const void*,const void*));
	 * elements of the array is refered by pointer const void *, so programmer should provides the
	 * compare function. only programmer knows the real data types.
	 * */
	qsort(suffix, i, sizeof(char*), cmpstr);

	#ifdef DEBUG
	printf("++++++++++++++ After sorting +++++++++++++++++""\n");
	for (int j = 0; j < i; j++)
	{
		printf("%p\t%s", suffix[j], suffix[j]);
	}
	#endif

	int maxlen = -1;
	int maxlenIdx = -1;
	for (int j = 0; j < i - 1; j++)
	{
		int len = common_len(suffix[j], suffix[j + 1]);
		if (len > maxlen)
		{  maxlen = len; maxlenIdx = j; }
		
	}
	printf("maxLen = %d\t :", maxlen);
	for (int j = 0; j < maxlen; printf("%c", suffix[maxlenIdx][j++]));
	printf("\n");
}

int main(int argc, char** argv)
{
 
  LOG_I("+[ %s ]\n", __FUNCTION__);
  //count_word_freq();
  demo_suffix_array();
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

