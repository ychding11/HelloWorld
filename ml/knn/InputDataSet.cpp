/*
 * InputDataSet.h
 * This a InputDataSet data type for handwriting recognise.
 *
 * Header Header
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

#include "InputDataSet.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

InputDataSet::InputDataSet(std::string dir, Logger &log) 
             : dataDir(dir), nextIdx(0), mlog(log) 
{
	mlog << "Construct InputDataSet..." << std::endl;
	for(int i = 0; i < DIGIT_NUM; ++i)
	{
		for(int j = 0; ; ++j) 
		{
		    /* construct a string. like sprintf() in C */
			std::ostringstream sampleFile;
			sampleFile << dataDir << "/" << i << "_" << j << ".txt";
			mlog << "Read data from : " << sampleFile.str() << std::endl;
	
		#ifdef CHECK_IO
			std::bitset<1024> sample;
			if(!readSample(sampleFile.str(), sample)) break;
			digits[i].push_back(sample);
			mlog << "Read data OK. Push data into vector : " << i << std::endl;
		#else
			std::ostringstream sample;
			if(!readSample(sampleFile.str(), sample))
				break;
			digits[i].push_back(std::bitset<1024>(sample.str()));
		#endif
		}
	}
    mlog << "Construct : " << dataDir << "Finished." << std::endl;
}

InputDataSet::~InputDataSet() 
{
}

bool InputDataSet::hasNext() 
{
	if(nextIdx == DIGIT_NUM) 
	{   nextIdx = 0;	return false;   }
	return true;
}

std::vector<std::bitset<1024> >& InputDataSet::next() 
{
	return digits[nextIdx++];
}

/* read sample from file line by line, one line contains a sample */
#ifdef CHECK_IO
bool InputDataSet::readSample(std::string sampleFile, std::bitset<1024>& sample) 
{
#else
bool InputDataSet::readSample(std::string sampleFile, std::ostringstream& sample) 
{
#endif
	std::ifstream sampleStream(sampleFile.c_str());
	if(!sampleStream) return false;  //？？？

#ifdef CHECK_IO
	char line[256];
	int lineNo = 0;
	while(true) 
	{
		sampleStream.getline(line, 256);
		if(sampleStream.bad())
		{
			sampleStream.close();
			throw std::runtime_error("Unercoverable error detected when reading from file " + sampleFile + "!");
		}
		if(sampleStream.fail() && !sampleStream.eof())
		{
			sampleStream.close();
			throw std::runtime_error("File " + sampleFile + " corrupted!");
		}

		if(sampleStream.eof())
		{
			sampleStream.close();
			break;
		}

		for(int i = 0; i < SAMPLE_DIM; ++i)
		{
			if( line[i] != '0') sample[lineNo * SAMPLE_DIM + i] = 1;
		}
		++lineNo;
	}
#else
	std::string line;
	while(!(sampleStream>>line).eof())
		sample<<line;
	sampleStream.close();
#endif
	return true;
}


