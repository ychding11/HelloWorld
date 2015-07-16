#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstring>
#include <string>
#include <map>
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
LOG_LEVEL_ALL,
} LogLevel;

LogLevel gCurLoglevel = LOG_LEVEL_ERR; //set the current log level

#define LOG_D(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_DBG) fprintf(stdout,"[ DEBUG ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_E(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_ERR) fprintf(stdout,"[ ERROR ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_I(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_INFO) fprintf(stdout,"[ INFO ] "fmt,##__VA_ARGS__ ); } while(0)

char to_lowcase(char ch)
{
    if (ch >= 'a' && ch <= 'z') return ch;
    else if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 'a';
    else return -1;
}

/* fgets takes newline ('\n') as a valid character 
 * reference:
 * http://www.cplusplus.com/reference/cstdio/fgets/?kw=fgets
 * return number of lines in a txt file. 
 */
static unsigned int count_line(char *filename)
{
    int lines = 0;
    char buf[BUFSIZE] = { 0 };
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    if (NULL == fp)
    {
        printf("Open file error! [ %s ] line:%d\n", filename, __LINE__);
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

/* struct hash node 
 * used to count world frequency 
 */
typedef struct node
{
char *word;
int count;
struct node *next;
} node;

#define NHASH 29989  // hash bucket number, a prime number
#define MULT 31      // used in hash function, prime number

node *bucket[NHASH];

/* hash function: turn a string into an 
 * interger. the interger later will be
 * used as an index of hash bucket. */
static unsigned int hash(char *str)
{
    unsigned int n = 0;
    for (; *str; str++)
    { n = MULT * n + *str; }
    return n % NHASH;
}

/* insert a word into hash bucket. */
void insert_word(char *word)
{
    assert(word != NULL);
    unsigned int index = hash(word);
    node *p;
    for(p = bucket[index]; p != NULL; p = p->next)
    {
        if (strcmp(p->word, word) == 0) // word already in bucket
        { p->count++; return;}
    }
    p = (node*)malloc(sizeof(struct node));
    p->count = 1;
    p->word = (char*)malloc(sizeof(word) + 1);
    strcpy(p->word, word);
    p->next = bucket[index];
    bucket[index] = p;
}

/* read word from stdin and count all world and its frequency.
 * we define world : strings seperated by blank regardless of
 * its actual meaning.  
 */
static void count_word_freq()
{    
    char buf[128];       /* supose longest world 127 character */
    memset(bucket, 0, sizeof(bucket)); /* clear Hash Bucket */
    while (scanf("%s", buf) != EOF)  
    {    insert_word(buf); }
    for (int i = 0; i < NHASH; i++)
    {
        for (node *p = bucket[i]; p != NULL; p = p->next)
        { printf("%64s\t%d\n", p->word, p->count);}
    }
}

#define DELIMETER "():.,'?-! \n;"

/* define the world : strings seperated by blank  
 * printf reference:
 * http://www.cplusplus.com/reference/cstdio/printf/?kw=printf
 */
static void count_word_freq2()
{    
    char wordBuf[128];        /* longest world 127 character */
    char lineBuf[2048];       /* longest line 2047 character */
    int lines = 0;
    int words = 0;
    memset(bucket, 0, sizeof(bucket)); /* clear Hash Bucket */
    
    while (fgets(lineBuf, 2048, stdin) != NULL)
    {
        lines++;
        char *p = strtok(lineBuf, DELIMETER);
        while (NULL != p)
        {    
            words++;
            LOG_D("Get word: %s\n", p);
            insert_word(p);
            p = strtok(NULL, DELIMETER);
        }
    }
    
    for (int i = 0; i < NHASH; i++)
    {
        for (node *p = bucket[i]; p != NULL; p = p->next)
        { printf("%-32s\t%3d\n", p->word, p->count);}
    }
    LOG_I("%s contains %d lines\n", __FILE__, lines);
    LOG_I("%s contains %d words\n", __FILE__, words);
}

/* take advantage of C++ STL map 
 * http://www.cplusplus.com/reference/map/map/operator[]/
 * STL map: element in the container is sorted. it is 
 * the difference from privous function. 
 */
static void count_word_freq3()
{    
    char wordBuf[128];        /* longest world 127 character */
    char lineBuf[2048];       /* longest line 2047 character */
    int lines = 0;
    int words = 0;
    memset(bucket, 0, sizeof(bucket)); /* clear Hash Bucket */
    map<string, int> mapTable;
    map<string, int>::iterator i;
    
    while (fgets(lineBuf, 2048, stdin) != NULL)
    {
        lines++;
        char *p = strtok(lineBuf, DELIMETER);
        while (NULL != p)
        {    
            words++;
            LOG_D("Get word: %s\n", p);
            mapTable[string(p)]++;
            p = strtok(NULL, DELIMETER);
        }
    }
    
    for ( i = mapTable.begin(); i != mapTable.end(); i++)
    {
        { printf("%-32s\t%3d\n", (i->first).c_str(), i->second);}
    }
    LOG_I("%s contains %d lines\n", __FILE__, lines);
    LOG_I("%s contains %d words\n", __FILE__, words);
}

int main(int argc, char** argv)
{
  LOG_I("+[ %s ]\n", __FUNCTION__);
  
  count_word_freq3();
  
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

