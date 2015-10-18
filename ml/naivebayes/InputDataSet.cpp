/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include "InputDataSet.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdexcept>
#include <iostream>

InputDataSet::InputDataSet(const std::string& dir, Logger &log)
             : __mdirName(dir), __mmCategoryNum(0), mlog(log)
{
#ifdef DEBUG
    mlog << "Constructing InputDataSet..." << std::endl;
#endif
    /* build the map of category and its associative files 
     * store the map info in member varible 
     *       std::map<int, std::vector<std::string> > __mdocs;
     */
	readDir();
#ifdef DEBUG
    mlog << "Constructing InputDataSet Finished." << std::endl;
#endif
}

InputDataSet::~InputDataSet() 
{
}

std::string& InputDataSet::getDir() 
{
	return __mdirName;
}

std::vector<std::string>& InputDataSet::getDocuments(int category) throw(IndexOutOfBound) 
{
	if(category >= __mmCategoryNum)	throw IndexOutOfBound(category);
	return __mdocs[category];
}

int InputDataSet::getCategoryNum() const 
{
	return __mmCategoryNum;
}

/* build the map of category and its associative files */
void InputDataSet::readDir() 
{
	DIR* dir = opendir(__mdirName.c_str());
	struct dirent* dirEntry = readdir(dir);
	while(dirEntry != 0) 
	{
		std::string fileName(dirEntry->d_name);

		if((fileName != ".") && (fileName != "..") && (fileName != ".directory")) 
		{
		#ifdef DEBUG
		    mlog << "Parsing Document Name = " << fileName << std::endl;
        #endif
			int category = parse(fileName);
			__mdocs[category].push_back(fileName);
            ++__mmCategoryNum;
		}
		dirEntry = readdir(dir);
	}
	closedir(dir);
#ifdef DEBUG
	mlog << "Parsing All Document In Dir " << __mdirName << std::endl;
#endif
}

/* From filename parse category info and return int value of category 
 * For example C0 C1 return 0 1 
 */
int InputDataSet::parse(const std::string& fileName) const 
{
	size_t pos = fileName.find_first_of('_', 0);
	std::string categoryName = fileName.substr(0, pos);

	/* check the category name */
	if(categoryName[0] != 'C')	throw std::runtime_error("Invalid document name " + fileName + "!");
	
	int category = 0;
	for(size_t i = 1; i < pos; ++i) 
	{
		char c = categoryName[i];
		
		/* ensure it is a digit */
		if(!((c >= '0') && (c <= '9')))	throw std::runtime_error("Invalid document name " + fileName + "!");
		
		category += category * 10 + c - '0';
	}
	
#ifdef NBC_DEBUG
	std::cout << "CategoryName = " << categoryName 
	          << ", Category Index = "<<category<<std::endl;
#endif
	return category;
}

