/*
 * Samples.cpp
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
#include <stdexcept>
#include <fstream>
#include <iostream>
#include "Samples.h"

template<typename DataType>
Samples<DataType>::Samples(const std::string& file, const DataReader<DataType>& reader, Logger &log)
    : dataFile(file), dataReader(reader), __mFeatureNum(0), __mSampleNum(0), __mlog(log)
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
		featureNames.push_back(feature);    ++__mFeatureNum;
		log << " Feature Name:" << feature << " " << __mFeatureNum << std::endl;
	}	
	featureNames.push_back(str.substr(start, str.length()-start));
	log << "Feature Name:" << featureNames.back() << " " << __mFeatureNum << std::endl;
	
#ifdef SAMPLES_DEBUG
	std::cout<<"__mFeatureNum = "<<__mFeatureNum<<std::endl;
	for(int i = 0; i < __mFeatureNum; ++i)
		std::cout<<featureNames[i]<<"\t";
	std::cout<<std::endl;
	std::cout<<"category = "<<featureNames[__mFeatureNum]<<std::endl;
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

		DataType* sample = new DataType[__mFeatureNum + 1]; //allocate space for smaple vector
		if(readSample(line, sample) == false) throw std::runtime_error("File " + dataFile + " is corrupted!");
		__mSamples.push_back(sample);
		++__mSampleNum;
	}
	__mlog << "Sample Number = " << __mSampleNum << std::endl;
	categories = new DataType[__mSampleNum];
	for(int i = 0; i < __mSampleNum; ++i)	categories[i] = __mSamples[i][__mFeatureNum];
}

template<typename DataType>
Samples<DataType>::~Samples() 
{
	if(__mSampleNum > 0) 
	{
		for(typename std::vector<DataType*>::iterator iter = __mSamples.begin(); iter != __mSamples.end(); ++iter)
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
	for(end = str.find_first_of('\t', start); (end != std::string::npos) && (i <= __mFeatureNum); 
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
	return __mFeatureNum;
}

template<typename DataType>
std::string Samples<DataType>::getFeatureName(const int index) const throw(IndexOutOfBound) 
{
	if(index >= __mFeatureNum) throw IndexOutOfBound(index);
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
	return __mSampleNum;
}
/*
template<typename DataType>
const DataType* Samples<DataType>::getSample(const int index)const
	throw(IndexOutOfBound) {
	if(index >= __mSampleNum)
		throw IndexOutOfBound(index);

	return __mSamples[index];
}
*/

