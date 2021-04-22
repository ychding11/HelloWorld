/*
 * recursion.cpp
 * demostrate recursion algorithms that how to generate all possible subsets
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
#include<cassert>
#include <glog/logging.h>

#include "thirdparty/gtest/gtest.h"
#include<bitset>

using namespace std;

#define PERFORMANCE_METER

/**************************************************************************
 *  The class is to generate subsets of a certain set
 *  
**************************************************************************/
class SubsetsGen 
{
public:
    //
    // generate all subsets of a set. such as {1, 2, 3}
    // {}, empty set is valid
    // 
    vector<vector<int> > subsets(vector<int>& nums) 
    {
        vector<vector<int> > ret;
        vector<int> temp;
        int n = nums.size();
        sort(nums.begin(), nums.end()); //assure subset is sorted.
        printVector(nums);
        subsetsHelper(ret, temp, nums, 0, n);
        printResult(ret);
        return ret;
    }

   /*
    * Given a set of n elements [1, n],
    * generate all possible combinations of k elements from the set.
    *
    */
    vector<vector<int> > combine(int n, int k)
    {
        vector<vector<int> > result;
        vector<int> tmp;
        combineHelper(1, n, k, tmp, result);
        return result;  //call copy constrctor here.
    }

    void combineHelper(int begin, int end, int k, vector<int> &tmp, vector<vector<int> > &result)
    {
        if (k == 0) // termination
        {
            result.push_back(tmp);
            return;
        }
        for (int i = begin; i <= end; i++)
        {
            tmp.push_back(i);
            combineHelper(i + 1, end, k - 1, tmp, result);
            tmp.pop_back();
        }
    }

 private:   
    void subsetsHelper(vector<vector<int> > &ret, vector<int> temp, const vector<int>& nums, int i, int n)
    {
        if (i >= n) // termination condition.
        {
            ret.push_back(temp);
            return;
        } 
        
        subsetsHelper(ret, temp, nums, i + 1, n); //no elements selected and search.
        temp.push_back(nums[i]);  //select current elements and search.
        subsetsHelper(ret, temp, nums, i + 1, n);
    }

    void printVector(const vector<int> &a)
    {
        int m = a.size();
        cout << "[ ";
        for (int i = 0; i < m; i++)
        {
            cout << a[i] << " ";
        }
        cout << "]" << std::endl;
    }
    void printResult(const vector<vector<int> > &ret)
    {
        int m = ret.size();
        cout << "All possible subsets:" << std::endl;
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
};


/*********************************************************************************
 * 
 * Test code begins 
 *
*********************************************************************************/

TEST(Subset, allSubsets)
{
    //
    // the result is much depends on human observer
    //
    int n;
    SubsetsGen generator;
    while (true)
    {
        cout << "Enter set size:";
        cin >> n;
        if (-1 == n)
        {
            cout << "Game Over." << std::endl;
            exit(1);
        }

        srand(time(NULL));
        vector<int> nums(n, 0);
        for (int i = 0; i < n; i++)
        {
            nums[i] = rand() % 100;
        }
        vector<vector<int> > ret = generator.subsets(nums);
    }
}

#if 0
int main(int argc, char** argv)
{
  int n;
  SubsetsGen generator;
  while (true)
  {
    cout << "Enter set size:";
    cin >> n;
    if (-1 == n)
    {
        cout << "Game Over." << std::endl;
        return 0;
    }

    vector<int> nums(n, 0);
    srand(time(NULL));

    cout << "Original vector: [ " ;
    for (int i = 0; i < n; i++)
    {
        nums[i] = rand() % 100;
        cout << nums[i] << " "; 
    }
    cout << "]" << std::endl;
    
    vector<vector<int> > ret = generator.subsets(nums);
    cout << "All possible subsets:" << std::endl; 
    printResult(ret);
  }
  
  return 0;
}
#endif

