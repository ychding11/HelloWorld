
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

#include<iostream>
#include<vector>


namespace test
{

using std::vector;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// For subset problem test.
//

template<typename T>
void allSubsetSumHelper(const vector<T> &nums, unsigned int index, T tempSum, vector<T> &sums)
{
    if (index == nums.size()) { sums.push_back(tempSum); return; }
    allSubsetSumHelper(nums, index + 1, tempSum + nums[index], sums);
    allSubsetSumHelper(nums, index + 1, tempSum, sums);
}

template<typename T>
vector<T> allSubsetSum(const vector<T> &nums)
{
    vector<T> ret;
    allSubsetSumHelper(nums, 0, 0, ret);
    return ret;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
