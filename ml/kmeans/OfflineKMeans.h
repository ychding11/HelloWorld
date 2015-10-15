/*
 * OfflineKMeans.h
 *
 * Header File
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

#ifndef _OFFLINE_K_MEANS_H_
#define _OFFLINE_K_MEANS_H_


#include <set>
#include <utility>
#include "Samples.h"
#include "Logger.h"

class OfflineKMeans 
{
private:
	const Samples<float>& __mSamples;
	int __mFeatureNum;
	int __mSampleNum;
	int __mClusterNum;
	std::pair<float*, std::set<float*> >* __mpClusters;
	/*
	 * Mapping from __mSamples to __mpClusters. __mpClusterIndexes[i] means sample indexed by 'i'
	 * is in cluster indexed by '__mpClusterIndexes[i]'.
	 */
	int* __mpClusterIndexes;
	Logger & __mlog;

public:
	OfflineKMeans(const Samples<float>& data, const int k, Logger &log);
	void print()const;
	~OfflineKMeans();

private:
	void initClusters();
	bool reCluster();
	void updateCentroids();
	int cluster(const float* sample, const int len)const;
	float distance(const float* sample1, const float* sample2, const int len)const;
};

#endif
