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
 
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cassert>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>

#include <glog/logging.h>

#include "list.h"
#include "profiler.h"

using std::cin;
using std::cout;

//#define PERFORMANCE_METER

std::unordered_map<std::string, core::ProfilerEntry> core::CPUProfiler::ProfilerData;

#define ENTER_FUNCTION 
#define EXIT_FUNCTION 

//< 64K  512K  2M  8M
struct SimpleMemAllocator
{
	struct MemSlice
	{
		void* addr;
		uint64_t size;
		uint64_t slot;

	};
	struct AllocateInfo
	{
		void* addr;
		uint64_t size;
		uint64_t slot;
	};
	std::vector<std::list<MemSlice>> mSlicePool;

	AllocateInfo allocate(size_t size)
	{
		uint64_t slotIndex;
		//< slot
		//< allocate(one insert, another put into pool) or  return from pool
		//< insert or make a new entry
		


	}

	void retire(const AllocateInfo &info)
	{
		//< put it into the certain list.
	}

	static SimpleMemAllocator* getMemAllocator();



};

//#if !defined(BUILD_LIBRARY)
/*************************************************
 * list algorithm tester.
*************************************************/
int main(int argc, char** argv)
{
  core::CPUProfiler::begin();
  ENTER_FUNCTION;

  EXIT_FUNCTION;
  LOG(INFO) << core::CPUProfiler::end();
  return 0;
}
//#endif
