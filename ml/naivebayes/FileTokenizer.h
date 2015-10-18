/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _FILE_TOKENIZER_H_
#define _FILE_TOKENIZER_H_

#include <string>
#include <fstream>

/* tokens are seperated by space (multiple spaces) */
class FileTokenizer 
{
private:
	std::ifstream __mdataStream;
	int __moffset;
	int __mlength;
    char __mline[256];
	char __mtoken[256];

public:
	FileTokenizer(const std::string& fileName);
   ~FileTokenizer();
	bool hasNext();
	std::string nextToken()const;
};

#endif
