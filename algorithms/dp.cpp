/*
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
#include <iostream>
#include <vector>

using namespace std;


/*************************************************
 * Function: Given a mxn array, each cell of the
 * array contains an integer number. Caculcate number
 * of paths from left up to bottom right with constraint
 * that path sum equal to K
 *   
 * Param[in]:   
 * Param[in]:   
 * Param[in]:   
 * Param[in]:   
 * Param[in]:   
 *   
 * Retrun: int, path number 
 * Ideas: Recursion on a matrix. Each move has two
 * choices.
 * Notice:  Handle boundary. 
 * TODO: any better solutions?
*************************************************/
int pathNumberWithK(const vector<vector<int>> &a, int m, int n, int c, const int K)
{
	if (m < 0 || n < 0) return 0;
	if (m == 0 && n == 0)
	{
		if (c + a[m][n] == K) return 1; // !caution
	    else return 0;
	}
	return pathNumberWithK(a, m -1, n, c + a[m][n], K) +
	    	pathNumberWithK(a, m, n - 1, c + a[m][n], K);
}


/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[in]:   
 *   
 * Retrun: int, all possible coin change method 
 * Ideas:  
 * Notice:   
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
}
int main()
{
	return 0;
}
