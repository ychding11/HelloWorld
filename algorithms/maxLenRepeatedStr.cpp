/*
 * maxLenRepeatedStr.cpp
 * demostrate an algorithm to solve max length of repeated string problem.
 * the idea is to apply the suffix array.
 *
 * Source File
 *
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
#include <cstring>
#include <ctime>
#include <cassert>

using namespace std;

#define BUFSIZE 1024

typedef enum tagLogLevel
{
LOG_LEVEL_ERR,
LOG_LEVEL_INFO,
LOG_LEVEL_DBG,
LOG_LEVLE_ALL,
} LogLevel;

LogLevel gCurLoglevel = LOG_LEVEL_ERR; //set the current log level

#define LOG_D(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_DBG) fprintf(stdout,"[ DEBUG ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_E(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_ERR) fprintf(stdout,"[ ERROR ] "fmt,##__VA_ARGS__ ); } while(0)
#define LOG_I(fmt, ...)  do { if (gCurLoglevel >= LOG_LEVEL_INFO) fprintf(stdout,"[ INFO ] "fmt,##__VA_ARGS__ ); } while(0)

//typedef unsigned char uint8_t;
//typedef unsigned int uint32_t;

char to_lowcase(char ch)
{
    if (ch >= 'a' && ch <= 'z') return ch;
    else if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 'a';
    else return -1;
}

/*************************************************
 *  count the lines of a text file. 
 *  return the lines in files. 
 *   
 *  NOTE: fgets takes newline ('\n') as a valid 
 *        character. fgets is a good IO operation.
*************************************************/
static int count_line(char *filename)
{
    int lines = 0;
    char buf[BUFSIZE] = { 0 };  //1024 BUFSIZE
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
        { /* do nothing , no new line in buffer */    }
        else
        { lines++; }
    }
    return lines;
}

/*************************************************
 *  calculate  common part length of tow char* 
 *  string. 
 *  return the common length. 
 *   
 *  NOTE: the comparing method is tricky.
*************************************************/
static int common_len(const char *p, const char *q)
{
    assert(p != NULL && q != NULL);
    int Len = 0;    
    while (*p && *p++ == *q++) Len++; // very tricky
    return Len;
}

#define MAX_CHAR (100 * 10000)  //max chars in a string

/* define string array and string suffix array as globle */
char c[MAX_CHAR], *suffix[MAX_CHAR];

/*************************************************
 *  compare tow char* string. the function is passed
 *  to qsort() as an input parameter.  
 *   
 *  NOTE: How to inteprete the input parametes.
*************************************************/
static int cmpstr(const void *a, const void *b)
{
    assert(a != NULL && b != NULL);
    #if 1
     char *c = *(char**)a;
     char *d = *(char**)b;
     
     // compare two char* string
     int ret = 0;
     for (; !(ret = *c - *d) && *c; c++, d++ ); // very tricky
     return  ret;
    #else
     return strcmp(c, d);  /* call string lib function */
    #endif
}

/*************************************************
 *  demostrate how to use suffix array to slove 
 *  max len of repeated substring problem. 
 *  the idea is :
 *  1. read string from stdin && build string 
 *     suffix array.
 *  2. sort the suffix array by stdcall qsort();
 *  3. after sorting, elements in string suffix
 *     array will be in an dictionary order. so
 *     apply common len of neighbouring elements
 *     calculation algorithms.
 *   
*************************************************/
static void demo_suffix_array()
{
    int i = 0;
    char ch;
    while ((ch = getchar()) != EOF) /* read a string from stdin char by char */
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
    
    /* 
     * sorting the suffix string array
     * void qsort (void* base, size_t num, size_t size, 
     *             int (*compar)(const void*,const void*));
     * elements of the array is refered by void pointer (const void *), 
     * so programmer need to provide the compare function. 
     * because only programmer knows the real data types and how to compare them.
     *
     * */
    qsort(suffix, i, sizeof(char*), cmpstr);

    #ifdef DEBUG
    printf("++++++++++++++ After sorting +++++++++++++++++""\n");
    for (int j = 0; j < i; j++)
    {
        printf("%p\t%s", suffix[j], suffix[j]);
    }
    #endif

    int maxlen = -1, maxlenIdx = -1;
    for (int j = 0; j < i - 1; j++)
    {
        int len = common_len(suffix[j], suffix[j + 1]); // just repeated once
        if (len > maxlen)
        { 
         maxlen = len; 
         maxlenIdx = j; 
        }
        
    }
    printf("maxLen = %d\t :", maxlen);
    for (int j = 0; j < maxlen; printf("%c", suffix[maxlenIdx][j++]));
    printf("\n");
}

/*************************************************
 * just call the demo string.
*************************************************/
int main(int argc, char** argv)
{
 
  LOG_I("+[ %s ]\n", __FUNCTION__);
  demo_suffix_array();
  LOG_I("-[ %s ]\n", __FUNCTION__);
  return 0;
}

