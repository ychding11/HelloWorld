/*
 * OfflineKMeans.cpp
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

#include <limits>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include "OfflineKMeans.h"
#include "Samples.cpp"

OfflineKMeans::OfflineKMeans(const Samples<float>& data, const int k, Logger &log) 
              : __mSamples(data), __mFeatureNum(data.getFeatureNum()), __mSampleNum(data.getSampleNum())
              , __mClusterNum(k), __mlog(log)
{
    __mlog << "Constructing OfflineKMeans..." << std::endl;
	initClusters();
#ifdef OKM_DBG
	print();
	int count = 0;
	while(reCluster() == true) 
	{
		print();    ++count;
	}
#else
	while(reCluster() == true);
#endif
}

OfflineKMeans::~OfflineKMeans() 
{
	for(int i = 0; i < __mClusterNum; ++i)
		delete[] __mpClusters[i].first;
	delete[] __mpClusters;
	delete[] __mpClusterIndexes;
}

void OfflineKMeans::initClusters() 
{
	// calculate the minimum and maximum values of each feature
	std::vector< std::pair<float, float> > featureRanges(__mFeatureNum);
	for(int j = 0; j < __mFeatureNum; ++j) 
	{
		float minFeatureJ = std::numeric_limits<float>::max();
		float maxFeatureJ = std::numeric_limits<float>::min();
		for(int i = 0; i < __mSampleNum; ++i) 
		{
			float feature = __mSamples.getSample(i)[j];
			if(minFeatureJ > feature)	minFeatureJ = feature;
			if(maxFeatureJ < feature)   maxFeatureJ = feature;
		}
		featureRanges[j].first  = minFeatureJ;
		featureRanges[j].second = maxFeatureJ;
		__mlog << "Feature " << j << ": [ " << minFeatureJ << ", " << maxFeatureJ << " ]" << std::endl;
	}

	// initialize centroids of __mpClusters.
	__mpClusters = new std::pair<float*, std::set<float*> >[__mClusterNum];
	srand(time(NULL));
	for(int i = 0; i < __mClusterNum; ++i) 
	{
		float* centroid = new float[__mFeatureNum];	
	    __mpClusters[i].first = centroid;	
		float scale = static_cast<float>(rand()) / RAND_MAX;
		__mlog << "Cluster " << i << " centroid : ";
		for(int j = 0; j < __mFeatureNum; ++j) 
		{
			float minValue = featureRanges[j].first;
			float maxValue = featureRanges[j].second;
			centroid[j] = minValue + (maxValue - minValue) * scale;
			__mlog << centroid[j] << " ";
		}
		__mlog << std::endl;
	}
	
#ifdef OKM_DBG
	std::cout<<"Centroids of __mpClusters initialized randomly:"<<std::endl;
	for(int i = 0; i < __mClusterNum; ++i) {
		std::cout<<"<";
		float* centroid = __mpClusters[i].first;
		int j;
		for(j = 0; j < __mFeatureNum-1; ++j)
			std::cout<<centroid[j]<<" ";
		std::cout<<centroid[j]<<">"<<std::endl;
	}
#endif

	/* cluster all __mSamples for the first time.
	 * setup mapping from __mSamples to __mpClusters and centroids of __mpClusters.
	 */
	__mpClusterIndexes = new int[__mSampleNum];
	for(int i = 0; i < __mSampleNum; ++i) 
	{
		const float* sample = __mSamples.getSample(i);
		int targetCluster = cluster(sample, __mFeatureNum);
		__mpClusterIndexes[i] = targetCluster;
		__mpClusters[targetCluster].second.insert(const_cast<float*>(sample));
	}
	updateCentroids();
}

/*
 * Cluster all the __mSamples again.
 */
bool OfflineKMeans::reCluster() 
{
    __mlog << "+[ " << __FUNCTION__ << " , " << __LINE__ << " ]" << std::endl;
	bool clusterChanged = false;
    for(int i = 0; i < __mSampleNum; ++i) 
    {
		const float* sample = __mSamples.getSample(i);
		int oldCluster = __mpClusterIndexes[i];
		int newCluster = cluster(sample, __mFeatureNum);
		if(newCluster != oldCluster) 
		{
			clusterChanged = true;
			__mlog <<"Sample : " << i << ", oldCluster = " << oldCluster << ", newCluster = " 
			       << newCluster << ", Move sample from old cluster to new cluster" << std::endl;
			
			// move sample from the old cluster to the new cluster.
			__mpClusters[oldCluster].second.erase(const_cast<float*>(sample));
			__mpClusters[newCluster].second.insert(const_cast<float*>(sample));
			__mpClusterIndexes[i] = newCluster;
		}
	}
	if(clusterChanged == true) updateCentroids();
	 __mlog << "-[ " << __FUNCTION__ << " , " << __LINE__ << " ]" << std::endl;
	return clusterChanged; // return cluster changed or not
}

/*
 * Update the centroids of all __mpClusters, it is always called after 
 * all __mSamples are clustered in one round.
 * To improve: only update the centroids of the affected __mpClusters.
 */
void OfflineKMeans::updateCentroids() 
{
    __mlog << "+[ " << __FUNCTION__ << " , " << __LINE__ << " ]" << std::endl;
    
	for(int i = 0; i < __mClusterNum; ++i) 
	{
		std::set<float*>& cluster = __mpClusters[i].second; // get set belonging to cluster i 
		int clusterSize = cluster.size();
		if(clusterSize > 0) 
		{
		    __mlog << "Cluster " << i << " centroid : ";
			for(int j = 0; j < __mFeatureNum; ++j) 
			{
				float featureSum = 0.0; //std::set<float*>::iterator
				for(auto p = cluster.begin(); p != cluster.end(); ++p)	featureSum += (*p)[j];
				(__mpClusters[i].first)[j] = featureSum / clusterSize;
				__mlog << (__mpClusters[i].first)[j] << " ";
			}
			__mlog << std::endl;
		}
	}	
	__mlog << "-[ " << __FUNCTION__ << " , " << __LINE__ << " ]" << std::endl;
}

// find the cluster whose centroid is closest to sample
int OfflineKMeans::cluster(const float* sample, const int len) const 
{
	int targetCluster = -1;
	float minDist = std::numeric_limits<float>::max();
	for(int j = 0; j < __mClusterNum; ++j) 
	{
		float dist = distance(sample, __mpClusters[j].first, len);
		if(minDist > dist) 
		{
			targetCluster = j;  minDist = dist;
		}
	}

	return targetCluster;
}

// sample1 and sample2 are vetor	
float OfflineKMeans::distance(const float* sample1, const float* sample2,const int len) const 
{
	float dist = 0.0;
	for(int i = 0; i < len; ++i) dist += (sample1[i] - sample2[i]) * (sample1[i] - sample2[i]);
	return dist;
}

void OfflineKMeans::print() const 
{
	__mlog << "Total " << __mClusterNum << " Clusters:" << std::endl;
	__mlog << "------------------------------------" << std::endl;
	for(int i = 0; i < __mClusterNum; ++i) 
	{
	    __mlog << i << ": <";
		float* centroid = __mpClusters[i].first;
		int j;
		for(j = 0; j < __mFeatureNum - 1; ++j) __mlog << centroid[j] << " ";
		__mlog << centroid[j] << ">, ";

		std::set<float*>& clusterSamples = __mpClusters[i].second;
		int count = 0;
		__mlog <<" has " << clusterSamples.size() << " Samples. " << std::endl;
		for(auto iter = clusterSamples.begin(); iter != clusterSamples.end(); ++iter) 
		{
		    if ((++count) % 4 == 0) __mlog << "\n";
			__mlog << "[";
			for(j = 0; j < __mFeatureNum - 1; ++j){   __mlog << (*iter)[j] << " ";    }
			__mlog  << (*iter)[j] << "] ";		
		}
		__mlog << std::endl;
		__mlog << "------------------------------------" << std::endl;
	}
}

