/**
 * Copyright(c) . All rights reserved.
 */
#include <cmath>
#include <iostream>
#include "KNearestNeighbours.h"
#include "NMinHeap.h" //template definition

KNearestNeighbours::KNearestNeighbours(InputDataSet& training, InputDataSet& test, int k, Logger& log)
                   : mTrainingSets(training), mTestSets(test)
                   , mkNN(k), mTotalTestSamples(0)
                   , mErrorTestSamples(0), mlog(log)
{
    mlog << ">>>>Construct KNearestNeighbours..." << std::endl;
    mlog << "Test test sample in TestSet one by one" << std::endl;
	for (int testDigit = 0; mTestSets.hasNext(); ++testDigit) 
	{
		auto& testSamples = mTestSets.next(); //std::vector<std::bitset<1024> >
		int testSampleNum = testSamples.size();
		for(int i = 0; i < testSampleNum; ++i) 
		{
			//std::cout<<"test: "<<testDigit<<"_"<<i<<std::endl;
			if(!KNN(testDigit, testSamples[i]))
			{	
			    ++mErrorTestSamples;
			}
		}
		mTotalTestSamples += testSampleNum;
	}
	mlog << "<<<<Construct KNearestNeighbours Finished" << std::endl;
}

KNearestNeighbours::~KNearestNeighbours() 
{
}

/* knn to */
bool KNearestNeighbours::KNN(int testDigit, const std::bitset<1024>& testSample) 
{
	NMinHeap<double, int> mkNNSet(mkNN);
	mlog << "+" << __FUNCTION__ << std::endl;
    mlog << "Build NMinheap size = " << mkNN << std::endl;
    mlog << "Judge digit : "  << testDigit << std::endl;
	int trainingDigit = 0;
	while(mTrainingSets.hasNext()) 
	{
		mlog << "Compute distance with training data: " << trainingDigit <<"_: ";
		auto& trainingSamples = mTrainingSets.next();
		int trainingSampleNum = trainingSamples.size();
		for(int i = 0; i < trainingSampleNum; ++i) 
		{
			mlog << i << ", ";
			mkNNSet.insert(distance(testSample, trainingSamples[i]), trainingDigit);
		}
		++trainingDigit;
		mlog << std::endl;
	}

    /* find most frequently occured 'digit' in traing set */
	int count[InputDataSet::DIGIT_NUM] = { 0 };
	int size = mkNNSet.size();
	for(int i = 0; i < size; ++i)
	{	++count[mkNNSet[i].second]; }

	int digit = 0, times = count[0];
	for(int i = 1; i < InputDataSet::DIGIT_NUM; ++i)
    {
		if(count[i] > times) 
		{
			digit = i;
			times = count[i];
		}
    }
    
	if(digit == testDigit) 
	{   
		mlog << "Matched: testDigit = " << testDigit << std::endl;
		mlog << "-" << __FUNCTION__ << std::endl;
		return true;
	}
	mlog << "Mismatched: testDigit = " << testDigit << ", digit = " << digit << std::endl;
	mlog << "-" << __FUNCTION__ << std::endl;
	return false;
}

float KNearestNeighbours::getErrorRate()const 
{
	mlog  << "ErrorTestSample = " << mErrorTestSamples << ", TotalTestSample = "
		    << mTotalTestSamples << std::endl;
	return static_cast<float>(mErrorTestSamples) / static_cast<float>(mTotalTestSamples);
}

/*
 * Performance boosts greatly when I optimized the code snippet below, 
 * the execution time reduces from 3897ms to 616ms, amazing!
 * int squareSum = 0;
 * for(int i = 0; i < 1024; ++i) {
 *     int diff = sample1[i] ^ sample2[i];
 *	   squareSum += diff;	// as to a value can be either 0 or 1, value == value*value 
 * }
 * int squareSum = (sample1 ^ sample2).count();
 */
float KNearestNeighbours::distance(const std::bitset<1024>& sample1, 
                                   const std::bitset<1024>& sample2 ) const 
{
	int squareSum = (sample1 ^ sample2).count();
	return sqrt(static_cast<float>(squareSum));
}


