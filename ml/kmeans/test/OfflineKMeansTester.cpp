/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */
#include <iostream>
#include "OfflineKMeans.h"
#include "FloatDataReader.h"
#include "Samples.cpp"  // template
#include "Logger.h"

int main(int argc, char** argv) 
{
	std::string dataFile("data/testSet.txt");
	Logger log("log.txt");
	log << __FUNCTION__ << std::endl;
	Samples<float> __mSamples(dataFile, FloatDataReader(), log);
	
	/*
	int __mFeatureNum = __mSamples.getFeatureNum();
	int __mSampleNum = __mSamples.getSampleNum();
	for(int i = 0; i < __mSampleNum; ++i) {
		const float* sample = __mSamples.getSample(i);
		for(int j = 0; j < __mFeatureNum; ++j)
			std::cout<<sample[j]<<" ";
		std::cout<<std::endl;
	}
	*/
	
	int k = 4;
	OfflineKMeans okm(__mSamples, k, log);
	std::cout<<"*****Result*****"<<std::endl;
	okm.print();

	return 0;
}
