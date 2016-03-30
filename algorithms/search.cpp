/*
 * search.cpp
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
#include "Logger.h"

//using namespace std;

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

#ifdef PERFORMANCE_METER
    
#endif

#ifdef PERFORMANCE_METER
    
#endif


#define METER_START()     \
clock_t clkStart;         \
{                         \
    clkStart = clock();   \
}

#define METER_END(clkStart)                                 \
{                                                            \
    clock_t clkEnd = clock();                               \
    clock_t clkDelt = clkEnd - clkStart;                    \
    float seconds = ((float)clkDelt) / CLOCKS_PER_SEC;      \
    printf (" %d ticks, %.4fseconds\n", clkDelt, seconds ); \
}

typedef int DataType;

#define SEARCH_BUFFER_SIZE (100 * 10000)
DataType array[SEARCH_BUFFER_SIZE];
int size = 0;

/*************************************************
 * Binary Search Function
 * search 't' in data array 'a'
 * It is a very excellent algorithm.
*************************************************/
template <class DataType>
int binSearch(DataType a[], int n, DataType t)
{
    ENTER_FUNCTION;
    assert(a != NULL && n > 0);
    
    int l, u, m;
    l = -1; u = n;
    logger << "l = " << l << " u = " << u << std::endl;
    while (l + 1 != u)
    {
        m = (l + u) / 2;
        logger << "m = " << m << std::endl;
        if (a[m] < t)
        {    l = m; }
        else
        {    u = m; }
        logger << "l = " << l << " u = " << u << std::endl;
    }
    int idx = u;
    if (idx >= n || a[idx] != t)
    {    
        logger << "Not Find target " << t << std::endl;
        return -1;    
    }
    return idx;
}

/*************************************************
 * Load data from file 
 * Usage `cat dat.txt | tester`.
*************************************************/
template <class DataType>
void loadData(DataType a[])
{
    DataType temp;
    int n = 0;
    while ((n < SEARCH_BUFFER_SIZE) && (scanf("%d", &temp) != EOF))
    {
        a[n++] = temp;
    }
    size = n;
}

/*************************************************
 * Tester Function
 * Read data from stdin. 
 * Usually by " cat dat.txt | Tester "
*************************************************/
int main(int argc, char** argv)
{ 
  ENTER_FUNCTION;
  DataType a;
  if (argc != 1)
  {
      printf("Usage Error! %s target\n", argv[0]);
  }
  a = atoi(argv[1]);
  logger << "Search " << a << std::endl;
  loadData(array);
  int ret = binSearch(array, size, a);
  if (ret < 0) 
  { 
    printf("%d does not exist in file!\n", a);
  }
  else            
  {
    printf("%d exist in file offset: %d\n", a, ret);
  }
  
  EXIT_FUNCTION;
  return 0;
}

