/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _SAMPLES_H_
#define _SAMPLES_H_

#include <string>
#include <vector>
#include "IndexOutOfBound.h"
#include "DataReader.h"
#include "Logger.h"
/*
 * Class Samples reads trainging or test samples from file and store them in a vector.
 * To read samples stored in file, user-defined converter should be provided to translate the string into data as expected.
 * Users can define their converter by inherting from interface Converter.
 */
template<typename DataType>
class Samples 
{
private:
	std::string dataFile;
	const DataReader<DataType>& dataReader;
	std::vector<std::string> featureNames;
	int mFeatureNum;	// number of features, excluding class label
	std::vector<DataType*> samples;
	int mSampleNum;
	DataType* categories;
    Logger & __mlog;
public:
	Samples(const std::string& file, const DataReader<DataType>& reader, Logger &log);
    virtual ~Samples();
	int getFeatureNum()const;
	std::string getFeatureName(const int index)const throw(IndexOutOfBound);
	const DataType* getCategories()const;
	int getSampleNum()const;
	inline const DataType* getSample(const int index) const throw(IndexOutOfBound) 
	{
	    if(index >= mSampleNum)  throw IndexOutOfBound(index);
	    return samples[index];
	}

protected:
	bool readSample(char line[256], DataType* sample);
};


template<typename DataType>
Samples<DataType>::Samples(const std::string& file, const DataReader<DataType>& reader, Logger &log)
    : dataFile(file), dataReader(reader), mFeatureNum(0), mSampleNum(0), __mlog(log)
{
    __mlog << "Construct Sample..." << std::endl;
	std::ifstream dataStream(dataFile.c_str());
	if(!dataStream) throw std::runtime_error("Unable to open file " + dataFile + "!");

	char line[256];
	dataStream.getline(line, 256);  std::string str(line);
    int start = 0, end = 0;
	for(end = str.find_first_of('\t', start); end != std::string::npos; 
		start = end + 1, end = str.find_first_of('\t', start)) 
	{
	    log << "Search pos = " << end;
		std::string feature = str.substr(start, end-start);
		featureNames.push_back(feature);    ++mFeatureNum;
		log << " Feature Name:" << feature << " " << mFeatureNum << std::endl;
	}	
	featureNames.push_back(str.substr(start, str.length()-start));
	log << "Feature Name:" << featureNames.back() << " " << mFeatureNum << std::endl;
	
#ifdef SAMPLES_DEBUG
	std::cout<<"mFeatureNum = "<<mFeatureNum<<std::endl;
	for(int i = 0; i < mFeatureNum; ++i)
		std::cout<<featureNames[i]<<"\t";
	std::cout<<std::endl;
	std::cout<<"category = "<<featureNames[mFeatureNum]<<std::endl;
#endif

	while(true) 
	{
		dataStream.getline(line, 256);
		if(dataStream.bad()) 
		{
			dataStream.close();
			throw std::runtime_error("Unrecoverable errors detected when reading from file " + dataFile + "!");
		}
		if(dataStream.fail() && !dataStream.eof()) 
		{
			dataStream.close();
			throw std::runtime_error("File " + dataFile + " is corrupted!");
		}
		if(dataStream.eof())
		{
			dataStream.close();
			break;
		}

		DataType* sample = new DataType[mFeatureNum + 1]; //allocate space for smaple vector
		if(readSample(line, sample) == false) throw std::runtime_error("File " + dataFile + " is corrupted!");
		samples.push_back(sample);
		++mSampleNum;
	}
	__mlog << "Sample Number = " << mSampleNum << std::endl;
	categories = new DataType[mSampleNum];
	for(int i = 0; i < mSampleNum; ++i)	categories[i] = samples[i][mFeatureNum];
	__mlog << "Construct Sample Finished" << std::endl;
}

template<typename DataType>
Samples<DataType>::~Samples() 
{
	if(mSampleNum > 0) 
	{
		for(typename std::vector<DataType*>::iterator iter = samples.begin(); iter != samples.end(); ++iter)
		    delete[] *iter;
		delete[] categories;
	}
}

template<typename DataType>
bool Samples<DataType>::readSample(char line[256], DataType* sample) 
{
	std::string str(line);
	
#ifdef SAMPLES_DEBUG
	std::cout<<"str = "<<str<<std::endl;
#endif

	DataType digit;
	int i = 0;
	size_t start = 0, end = 0;
	__mlog << "Read data line : ";
	for(end = str.find_first_of('\t', start); (end != std::string::npos) && (i <= mFeatureNum); 
	    start = end + 1, end = str.find_first_of('\t', start)) 
	{
		std::string digitStr = str.substr(start, end - start);
		if(dataReader(digitStr, digit) == false) return false;
		sample[i++] = digit;
		__mlog << digit << " ";
	}
	if(dataReader(str.substr(start, str.length() - start), digit) == false) return false;
	sample[i] = digit;
    __mlog << digit << std::endl;
    
#ifdef SAMPLES_DEBUG
	std::cout<<"digitNum = "<< digitNum << std::endl;
#endif

	return true;
}

template<typename DataType>
int Samples<DataType>::getFeatureNum()const 
{
	return mFeatureNum;
}

template<typename DataType>
std::string Samples<DataType>::getFeatureName(const int index) const throw(IndexOutOfBound) 
{
	if(index >= mFeatureNum) throw IndexOutOfBound(index);
	return featureNames[index];
}

template<typename DataType>
const DataType* Samples<DataType>::getCategories() const 
{
	return categories;
}

template<typename DataType>
int Samples<DataType>::getSampleNum() const 
{
	return mSampleNum;
}

#endif
