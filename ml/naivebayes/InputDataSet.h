/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#ifndef _INPUT_DATA_SET_H_
#define _INPUT_DATA_SET_H_

#include <string>
#include <vector>
#include <map>
#include "IndexOutOfBound.h"
#include "Logger.h"

class InputDataSet 
{
private:
	std::string __mdirName;
	std::map<int, std::vector<std::string> > __mdocs;
	Logger &mlog;
	int __mmCategoryNum;

public:
	InputDataSet(const std::string& dir, Logger &log);
	~InputDataSet();
	// A member function returns reference to a non-const member variable can't be constant
	std::string& getDir();
	std::vector<std::string>& getDocuments(int category) throw(IndexOutOfBound);
	int getCategoryNum()const;

private:
	void readDir();
	/* From filename parse category info and return int value of category */
	int parse(const std::string& fileName)const;
};

#endif
