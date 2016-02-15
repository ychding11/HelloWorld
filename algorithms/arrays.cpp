
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

#include<cstdio>
#include<vector>
#include<deque>

using namespace std;

/*************************************************
 * Function: 
 *   
 * Param[in]:  nums array of '1' '0' 
 * Param[in]:  max number of flipping 0 to 1 
 *   
 * Retrun: index vector indicating which elements
 * flipped.
 *   
 * Notice:   
 * Ideas:  sliding window.
*************************************************/
vector<int> flips(vector<int> &nums, int m)
{
	vector<int> ret;
	deque<int> que;
	int n = nums.size();
	int p = 0, q = 0;
	int zeros = 0;
	int widest = 0, width = 0;
	while (q < n)
	{
		//enlarge right side
		while (zeros <= m && q < n)
		{
			if (nums[q] == 0) que.push_back(q), ++zeros;
			++q;
		}

		//check and update
		if (q >= n)
		{
			while (!que.empty()) ret.push_back(que.front()), que.pop_front();
			return ret;
		}
		width = q - p;
		if (widest < width)
		{
			widest = width;
		}

		//shrink left side
		for (; p <= q && nums[p] != 0; ++p);
		if (nums[p] == 0) que.pop_front(), --zeros;
	}
	while (!que.empty()) ret.push_back(que.front()), que.pop_front();
	return ret;
}

/*************************************************
 * Function: reorder two array in asending order
 * array nums contains values while array index
 * contains corresponding index;
 * array 2 3 5 1
 * index 1 2 4 3
 *   
 * Param[in]:   
 * Param[in]:   
 *   
 * Retrun: 
 *   
 * Notice:   
*************************************************/
void reorderIndex(vector<int> &nums, vector<int> &index)
{
	int n = nums.size();
	if (n <= 1) return;
	for (int i = 0; i < n; i++)
	{
		while (index[i] != i) //disorder
		{
			int oldNum = nums[i];
			int oldIdx = index[i];
			nums[i] = nums[index[i]];
			index[i] = index[index[i]];
			nums[index[i]] = oldNum;
			index[index[i]] = oldIdx;
		}
	}
}

int main()
{
 return 0;
}