/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */


#include <iostream>
#include "Logger.h"
#include "OfflineNaiveBayesClassifier.h"
#include "FileTokenizer.h"

int main(int argc, char** argv) 
{
	std::string trainingDir = "data//training";
	std::string doc = "data//test//posting.txt";
/*
	FileTokenizer tokenizer(doc);
	while(tokenizer.hasNext()) {
		std::string token = tokenizer.nextToken();
		std::cout<<token<<std::endl;
	}
*/
    Logger log("log.txt");
	InputDataSet inputDataSet(trainingDir, log);
	OfflineNaiveBayesClassifier offlineNBC(inputDataSet, log);
	offlineNBC.classify(doc);

	return 0;
}
