/*
 * combination.cpp
 * demostrate recursion algorithms that how to generate all possible combinations
 * of a set.
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
#include <ctime>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Logger.h"

using namespace std;

#define PERFORMANCE_METER

/*************************************************
 *  The class is to generate all possible 
 *  combinations.
*************************************************/
class Combination 
{
public:
    /* 
     * given a set with n elements, caculate all
     * possible combinations of k elements from 
     * the set.
     */
    vector<vector<int> > combine(int n, int k) 
    {
        vector<vector<int> > result;
        vector<int> tmp;
        combineHelper(1, n, k, tmp, result);
        return result;  //call copy constrctor here.
    }
    
    void combineHelper(int begin, int end, int n, vector<int> &tmp, vector<vector<int> > &result)
    {
        /* search termination condition */
        if (n == 0)
        {  result.push_back(tmp); return; }
        for (int i = begin; i <= end; i++)
        {
            tmp.push_back(i);
            combineHelper(i + 1, end, n - 1, tmp, result);
            tmp.pop_back();
        }
    }
};

void printResult(const vector<vector<int> > &ret)
{
    int m = ret.size();
    for (int i = 0; i < m; i++)
    {
        int n = ret[i].size();
        cout << "[ ";
        for (int j = 0; j < n; j++)
        {
            cout << ret[i][j] << " ";
        }
        cout << "]" << std::endl;
    }
}
/*************************************************
 * just a simple tesing
*************************************************/
int main(int argc, char** argv)
{
  logger.setLevel(DEBUG);
  logger.setLineLevel(DEBUG);
  ENTER_FUNCTION;
  
  int n, k;
  Combination cmb;
  while (true)
  {
    cout << "Enter set size and selected elements:";
    cin >> n >> k;
    if (-1 == n)
    {
        cout << "Game Over." << std::endl;
        return 0;
    }
    
    vector<vector<int> > ret = cmb.combine(n, k);
    cout << "All possible combinations:" << std::endl; 
    printResult(ret);
  }
  
  EXIT_FUNCTION;
  return 0;
}

