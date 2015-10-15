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
 * Class Samples reads trainging or test __mSamples from file and store them in a vector.
 * To read __mSamples stored in file, user-defined converter should be provided to translate the string into data as expected.
 * Users can define their converter by inherting from interface Converter.
 */
template<typename DataType>
class Samples 
{
private:
	std::string dataFile;
	const DataReader<DataType>& dataReader;
	std::vector<std::string> featureNames;
	int __mFeatureNum;	// number of features, excluding class label
	std::vector<DataType*> __mSamples;
	int __mSampleNum;
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
	    if(index >= __mSampleNum)  throw IndexOutOfBound(index);
	    return __mSamples[index];
	}

protected:
	bool readSample(char line[256], DataType* sample);
};

#endif
