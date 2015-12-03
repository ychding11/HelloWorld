/*
 * FileTokenizer.cpp
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

#include "FileTokenizer.h"
#include <cstring>
#include <stdexcept>

/*************************************************
 *  Constructor.
*************************************************/
FileTokenizer::FileTokenizer(const std::string& fileName)
              : mDataStream(fileName.c_str()) //initialize member varible
              , mOffset(0)
              , mLength(0)
{
	if(!mDataStream) //check file stream is ok
	{	
	    throw std::runtime_error("Unable to open file " + fileName + "!"); 
	}
	mDataStream.getline(mLineBuffer, 256);
	mLength = strlen(mLineBuffer);
}

/*************************************************
 *  Destructor.
*************************************************/
FileTokenizer::~FileTokenizer() 
{
	mDataStream.close();
}	

bool FileTokenizer::hasNext() 
{
    //check current line buffer, if it is empty. reload next line from file.
	if(mOffset >= mLength) 
	{
		mDataStream.getline(mLineBuffer, 256);
		if(mDataStream.eof())
		{  
		    return false;
		} 
		mOffset = 0; 
		mLength = strlen(mLineBuffer);
	}

	int i = 0;
	while(mOffset < mLength) 
	{
		if(mLineBuffer[mOffset] == ' ')	break;
		mTokenBuffer[i++] = mLineBuffer[mOffset++];
	}
	mTokenBuffer[i] = '\0';
	
	//skip following spaces
	while((mOffset < mLength) && (mLineBuffer[++mOffset] == ' '));
	return true;
}

std::string FileTokenizer::nextToken()const 
{
	return std::string(mTokenBuffer);
}

