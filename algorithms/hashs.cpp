
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

#include<iostream>
#include<cstdio>
#include<vector>
#include<unordered_map> // only C++11 support

using namespace std;

/*************************************************
 * Function:  given an array of integers determine
 * whether it can be divide into groups and each 
 * group can be divide by k.
 *   
 * Param[in]:  nums array of number 
 * Param[in]:  k dividor factor 
 *   
 * Retrun: the result of judgement 
 *   
 * Notice: how about an empty number array ?   
 * Ideas: store reminder infomation and search in
 * second pass.
*************************************************/
bool canDivideGroupsByK(vector<int> & nums, int k)
{
	int n = nums.size();
	if (n ^ 0x1) return false; //odd number cannot divide into groups
	unordered_map<int, int> reminderFreqs;
	for (int i = 0; i < n; i++)
	{
		reminderFreqs[nums[i] % k]++;
	}
	for (int i = 0; i < n; i++)
	{
		int rem = nums[i] % k;
		if (rem + rem == k)
		{
			if (reminderFreqs[rem] & 0x1) 
				return false;
		}
		else
		{
			if (reminderFreqs[rem] != reminderFreqs[k - rem])
				return false;
		}
	}
	return true;
}

/*************************************************
 * Function: given an array of integers, try to 
 * find four elements a, b, c and d such that
 * a+b=c+d. the function determines whether such
 * four elemnts exist or not.
 *   
 * Param[in]:  nums the array of integers 
 *   
 * Retrun: bool indicating whether exist or not 
 *   
 * Notice:   
 * Ideas:  list all possible pairs and store the pair
 * sum and pair itself as an map entry.for each new
 * pair search its sum in map first, if got one such
 * four elements exist.
*************************************************/
bool arePairsExist(vector<int> &nums)
{
	int n = nums.size();
	if (n < 4) return false;
	unordered_map<int, pair<int, int>> map;
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			int sum = nums[i] + nums[j];
			if (map.find(sum) == map.end())
			{
				map[sum] =  make_pair(i, j);
			}
			else
			{
				printf("Found it (%d, %d) (%d, %d)\n", nums[i], nums[j], nums[map[sum].first], nums[map[sum].second]);
				return true;
			}
		}
	}
	return false;
}

int main()
{
	return 0;
}
