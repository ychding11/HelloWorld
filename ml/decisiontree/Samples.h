/*
 * Samples.h
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

#ifndef _SAMPLES_H_
#define _SAMPLES_H_

#include <string>
#include <vector>
#include "IndexOutOfBound.h"
#include "DataReader.h"
#include "Logger.h"
/*
 * Class Samples reads trainging or test mpSamples from file and store them in a vector.
 * To read mpSamples stored in file, user-defined converter should be provided to translate the string into data as expected.
 * Users can define their converter by inherting from interface Converter.
 */

/*************************************************
 *  class template
 * 
 *  used as input
*************************************************/
template<typename DataType>
class Samples 
{
private:
	std::string dataFile;
	const DataReader<DataType>& dataReader; //template used in template
	std::vector<std::string> mFeatureNames;
	int mFeatureNum;	// number of features, excluding class label
	std::vector<DataType*> mpSamples;
	int mSampleNum;
	DataType* mpCategories;
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
	    if(index >= mSampleNum)  
	    {
            throw IndexOutOfBound(index);
	    }
	    return mpSamples[index];
	}

protected:
	bool readSample(char line[256], DataType* mpSample);
};


template<typename DataType>
Samples<DataType>::Samples(const std::string& file, const DataReader<DataType>& reader, Logger &log)
                  : dataFile(file)
                  , dataReader(reader)
                  , mFeatureNum(0)
                  , mSampleNum(0)
                  , __mlog(log)
{
    __mlog << "Construct Sample..." << std::endl;
	std::ifstream dataStream(dataFile.c_str());
	if(!dataStream)
	{ 
	    throw std::runtime_error("Unable to open file " + dataFile + "!");
	}

    //construct mFeatureNames from first line of input data file.
	char line[256];
	dataStream.getline(line, 256);  
	std::string str(line);
    int start = 0, end = 0;
	for(end = str.find_first_of('\t', start); 
	    end != std::string::npos; 
		start = end + 1, end = str.find_first_of('\t', start)) 
	{
	    log << "Search pos = " << end;
		std::string feature = str.substr(start, end-start);
		mFeatureNames.push_back(feature);    
		++mFeatureNum;
		log << " Feature Name:" << feature << " " << mFeatureNum << std::endl;
	}	
	mFeatureNames.push_back(str.substr(start, str.length()-start));
	log << "Feature Name:" << mFeatureNames.back() << " " << mFeatureNum << std::endl;
	
#ifdef SAMPLES_DEBUG
	std::cout<<"mFeatureNum = "<<mFeatureNum<<std::endl;
	for(int i = 0; i < mFeatureNum; ++i)
		std::cout<<mFeatureNames[i]<<"\t";
	std::cout<<std::endl;
	std::cout<<"category = "<<mFeatureNames[mFeatureNum]<<std::endl;
#endif

    //construct Sample from remaining lines of input data file.
	while(true) 
	{
		dataStream.getline(line, 256);
		if(dataStream.bad()) //read error occurs
		{
			dataStream.close();
			throw std::runtime_error("Unrecoverable errors detected when reading from file " + dataFile + "!");
		}
		if(dataStream.fail() && !dataStream.eof()) //file corrupted
		{
			dataStream.close();
			throw std::runtime_error("File " + dataFile + " is corrupted!");
		}
		if(dataStream.eof()) //read all lines
		{
			dataStream.close();
			break;
		}
		
        //allocate space for a single smaple vector. the extra one is for category
		DataType* mpSample = new DataType[mFeatureNum + 1]; 
		if(readSample(line, mpSample) == false) 
		{
		    throw std::runtime_error("File " + dataFile + " is corrupted!");
		}
		mpSamples.push_back(mpSample);
		++mSampleNum;
	}
	__mlog << "Sample Number = " << mSampleNum << std::endl;
	mpCategories = new DataType[mSampleNum];
	for(int i = 0; i < mSampleNum; ++i)	mpCategories[i] = mpSamples[i][mFeatureNum];
	__mlog << "Construct Sample Finished" << std::endl;
}

template<typename DataType>
Samples<DataType>::~Samples() 
{
	if(mSampleNum > 0) 
	{
		for(typename std::vector<DataType*>::iterator iter = mpSamples.begin(); iter != mpSamples.end(); ++iter)
		{    delete[] *iter; }
		delete[] mpCategories;
	}
}

/*************************************************
 *  parse data fields from buffer line and construct
 *  the mpSample.
 *  the data fields was seperate by '\t'
*************************************************/
template<typename DataType>
bool Samples<DataType>::readSample(char line[256], DataType* mpSample) 
{
	std::string str(line);
	
#ifdef SAMPLES_DEBUG
	std::cout<<"str = "<< str <<std::endl;
#endif

	DataType digit;
	int i = 0;
	size_t start = 0, end = 0;
	__mlog << "Read data line : ";
	for(end = str.find_first_of('\t', start); 
	    (end != std::string::npos) && (i <= mFeatureNum); 
	    start = end + 1, end = str.find_first_of('\t', start)) 
	{
		std::string digitStr = str.substr(start, end - start);
		if(dataReader(digitStr, digit) == false) return false;
		mpSample[i++] = digit;
		__mlog << digit << " ";
	}
	
    /**
     * handle the last columu, it is the category of the current mpSample
     **/
	if(dataReader(str.substr(start, str.length() - start), digit) == false)
	{
	 return false;
	}
	mpSample[i] = digit;
    __mlog << digit << std::endl;
    
#ifdef SAMPLES_DEBUG
	std::cout<<"digitNum = "<< digitNum << std::endl;
#endif

	return true;
}

template<typename DataType>
int Samples<DataType>::getFeatureNum() const 
{
	return mFeatureNum;
}

template<typename DataType>
std::string Samples<DataType>::getFeatureName(const int index) const throw(IndexOutOfBound) 
{
	if(index >= mFeatureNum) throw IndexOutOfBound(index);
	return mFeatureNames[index];
}

template<typename DataType>
const DataType* Samples<DataType>::getCategories() const 
{
	return mpCategories;
}

template<typename DataType>
int Samples<DataType>::getSampleNum() const 
{
	return mSampleNum;
}

#endif
