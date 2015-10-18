/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "FileTokenizer.h"
#include <cstring>
#include <stdexcept>

FileTokenizer::FileTokenizer(const std::string& fileName)
              : __mdataStream(fileName.c_str()), __moffset(0), __mlength(0)
{
	if(!__mdataStream)
	{	throw std::runtime_error("Unable to open file " + fileName + "!"); }

	__mdataStream.getline(__mline, 256);
	__mlength = strlen(__mline);
}

FileTokenizer::~FileTokenizer() 
{
	__mdataStream.close();
}	

bool FileTokenizer::hasNext() 
{
    // the current line is empty. need to read the next line in file.
	if(__moffset >= __mlength) 
	{
		__mdataStream.getline(__mline, 256);
		if(__mdataStream.eof()){ return false; } 
		__moffset = 0; __mlength = strlen(__mline);
	}

	int i = 0;
	while(__moffset < __mlength) 
	{
		if(__mline[__moffset] == ' ')	break;
		__mtoken[i++] = __mline[__moffset++];
	}
	__mtoken[i] = '\0';
	// skip following spaces
	while((__moffset < __mlength) && (__mline[++__moffset] == ' '));
	return true;
}

std::string FileTokenizer::nextToken()const 
{
	return std::string(__mtoken);
}

