/*
 * Copyright (C) 2014-2016  Yaochuang Ding - <ych_ding@163.com>
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
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cassert>

//using namespace std;
using std::vector;  //better 
using std::string;  //better 
using std::max;  //better 
using std::min;  //better 


/************************************************************************************
 * Function:
 *   Given a mxn array, each cell of the array contains an integer number.
 *   Caculcate the number of paths from left up to bottom right with constraint
 *   that path sum equal to K
 *   
 * Param[in]:   
 * Param[in]:   
 * Param[in]:   
 * Param[in]:   
 * Param[in]:   
 *   
 * Retrun: int, path number 
 * Ideas: Recursion on a matrix. Each move has two choices.
 * Notice: Handle boundary. 
 * TODO: any better solutions ?
**************************************************************************************/
int pathNumberWithK(const vector<vector<int>> &a, int m, int n, int c, const int K)
{
	if (m < 0 || n < 0) return 0;
	if (m == 0 && n == 0)
	{
		if (c + a[m][n] == K) return 1; // !caution
	    else return 0;
	}
	return pathNumberWithK(a, m - 1, n,     c + a[m][n], K) +
	       pathNumberWithK(a, m,     n - 1, c + a[m][n], K);
}

int PathSumEquatoK(const int **a, int m, int n, int c, const int K)
{
	if (m < 0 || n < 0) return 0;
	if (m == 0 && n == 0)
	{
		if (c + a[m][n] == K) return 1; // !caution
	    else return 0;
	}
	return PathSumEquatoK(a, m - 1, n,     c + a[m][n], K) +
	       PathSumEquatoK(a, m,     n - 1, c + a[m][n], K);
}


/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[in]:   
 *   
 * Retrun: int, all possible coin change method 
 * Ideas: I can apply tree structure to explain 
 * my solution. 'money' as root, each edge has 
 * a value corresponding to coins[i], a new node
 * is generated by substrabt current weight value
 * from current node, if the substrabtion is 0,
 * then you get a terminal node. the terminal 
 * node number is result.
 *
 * Notice: The solutions has some defact. For 
 * example, {4, [1, 2, 3]} as input. there are
 * <1, 1, 1, 1,> <1, 1, 2>, <2, 2>, <1, 3> totally
 * 4 methods.
 * But use the following solution, it gives 7 methods.
 * The additional methods are <3, 1>, <2, 1, 1> and 
 * <1, 2, 1>
*************************************************/
int coinChange(int money, const vector<int> &coins)
{
	int n = coins.size(); // assume n > 0
	if (money == 0) return 1; // !Caution money >= 0 is always true
	int num = 0;
	for (int i = 0; i < n; ++i)
		if(coins[i] < money) num += coinChange(money - coins[i], coins);
	return num;
}

int coinChange(int n, const int *coins, int m)
{
	if (n < 0 || m <= 0) return 0;
	if (n == 0) return 1;
	return coinChange(n, coins, m - 1) + coinChange(n - coins[m - 1], coins, m);
}

/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[in]:   
 *   
 * Retrun: int, min number of coins 
 * Ideas:
 * f(n), minimum number of coins for money n.   
 * f(0) = 0.
 * f(n) = min {f(n - coins[i])} + 1, n >= coins[i].	 
 * Notice:   
*************************************************/
int coinChangeWithMinCoins(int money, const vector<int> &coins)
{
    return 0;
}

/**************************************************************************************************
 * Function: Given a grid with each cell containing an integer value. Calculate the max path sum 
 * from (0, 0) --> (m-1, n-1). For each cell, only two direction is possible: right, down.
 *   
 * Param[in]:  array  
 *   
 * Retrun:
 *  int, max path sum 
 * Ideas:
 *  Dynamic Programming 
 *  f(i,j) : max path sum of (0,0) --> (i, j)
 *  f(i,j) = max{f(i, j-1), f(i-1, j)} + grid[i][j], i>0, j>0
 *
 * Notice:  Handle boundary with caution 
**************************************************************************************************/
int maxPathSumOfGrid(const vector<vector<int>> &grid)
{
   int m = grid.size();
   if (m <= 0) return 0;
   int n = grid[0].size();
   if (n <= 0) return 0;
   vector<vector<int>> pathSum(m, vector<int>(n, 0));
   pathSum[0][0] = grid[0][0];
   int i, j;
   for (i = 1; i < n; ++i) pathSum[0][i] = pathSum[0][i - 1] + grid[0][i];
   for (i = 1; i < m; ++i) pathSum[i][0] = pathSum[i - 1][0] + grid[i][0];
   for (i = 1; i < m; ++i)
       for (j = 1; j < n; ++j)
	   {
		   pathSum[i][j] = max(pathSum[i][j - 1], pathSum[i - 1][j]) + grid[i][j]; 
	   }
   return pathSum[m - 1][n - 1];
}

int allPathsOfGrid(const vector<int> &grid, int n)
{
    return 0;
}

int minInitialValueOfGrid(const vector<int> &grid, int n)
{

    return 0;
}

/**************************************************************************************************
 * Function: Given a grid containing character, 
 * and a start character in the grid,
 * find the length of logest consecutive sequence. 
 * a cell has 8 neighbours. 
 * 
 * Param[in]:  array, char grid 
 * Param[in]:  char,  starting character. 
 *   
 * Retrun: int, max length 
 * Ideas:  
 * Notice:  For example, starting char is 'a' 
 * a b c
 * b a d 
 * c d e
 * the logest sequence is a->b->c->d->e.
 * There are multiple start points. use dp to store
 * calculated results can improve effiency.
**************************************************************************************************/
int  longestConsecutiveSequenceOfGridHelper(const  vector<vector<char>> &grid, vector<vector<int>> &dp, int x, int y, int preChar)
{
	int m = grid.size(), n = grid[0].size(); // no need to check m & n here.
	const int detX[8] = {0, 1, 1, 1, 0, -1, -1, -1};
	const int detY[8] = {1, 1, 0, -1,-1, -1, 0, 1};
	if (x >= m || x < 0 || y >= n || y < 0) return 0; //check boundary
	if (grid[x][y] != preChar - 1) return 0; // check requirements
	if (dp[x][y] != -1) return dp[x][y]; // lookup dp table to see whether it really needs calculation
	int ret = 0;
	for (int i = 0; i < 8; ++i)
	{
        	int temp = longestConsecutiveSequenceOfGridHelper(grid, dp, x+detX[i], y + detY[i], grid[x][y]);  
		if (temp + 1 > ret) ret = temp + 1;
	}
	dp[x][y] = ret; // record result in dp table
	return ret;
}

int longestConsecutiveSequenceOfGrid(const vector<vector<char>> &grid, char ch)
{
	int m = grid.size();
	if (m <= 0 ) return 0;
	int n = grid[0].size();
	int ret = 0;
	vector<vector<int>> dp(m, vector<int>(n, -1));
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (grid[i][j] == ch) //find the starting char
			{
				int temp = longestConsecutiveSequenceOfGridHelper(grid, dp, i, j, ch - 1);
				if (temp > ret) ret = temp;
			}
		}
	}
	return ret;
}

//#define  min(a, b)  (a > b ? b : a)

int editDistance(const string &str1, const string &str2)
{
    if (str1.empty()) return str2.length();
    if (str2.empty()) return str1.length();
    int m = str1.length();
    int n = str2.length();
    if (str1[m - 1] == str2[n - 1])
    {
        return editDistance(str1.substr(0, m - 1), str2.substr(0, n - 1));
    }
    else
    {
        int d1 = editDistance(str1.substr(0, m - 1), str2.substr(0, n - 1)); // modify
        int d2 = editDistance(str1, str2.substr(0, n - 1)); // del
        int d3 = editDistance(str1.substr(0, m - 1), str2); // add
        return min(min(d1, d2), d3) + 1;
    }
}

/*********************************************************************************
 * Function:
 *  return the max sum of subarray 
 *  all nagetive values max subarray is 0
 *  subarray with 0 elements is ok
 *
 * Ideas:
 *  s[i] : max sum of subarray ending with element i
 *   
*********************************************************************************/
int maxSumSubarray(const std::vector<int> &a)
{
    int n = a.size();
    assert(n > 0);
    std::vector<int> s(n,0);
    s[0] = a[0] > 0 ? a[0] : 0;
    for (int i = 1; i < n; ++i)
    {
        s[i] = s[i - 1] > 0 ? s[i - 1] + a[i] : a[i];
    }
    return *std::max_element(s.begin(), s.end());
}

/*********************************************************************************
 * 
 * Test code begins 
 *
*********************************************************************************/

#include "thirdparty/gtest/gtest.h"

TEST(DynamicPlanning, maxSumSubarray)
{
    {
        std::vector<int> a{ -2,1,-3,4,-1,2,1,-5,4 };
        EXPECT_EQ(6, maxSumSubarray(a));
    }

    {
        std::vector<int> a{ -2,-3,-1,-5 };
        EXPECT_EQ(0, maxSumSubarray(a));
    }
}

//int main()
//{
//	return 0;
//}
