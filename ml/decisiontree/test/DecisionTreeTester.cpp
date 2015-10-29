/**
 * Copyright(c) 2012 - 2013 minglin. All rights reserved.
 */

#include <iostream>
#include "Samples.h"
#include "StringDataReader.h"
#include "DecisionTree.h"
#include "Timing.h"
#include "Logger.h"

int main(int argc, char** argv) 
{
	std::string dataFile = "data/species.txt";
	Logger log("log.txt");
	Samples<std::string> trainingSamples(dataFile, StringDataReader(), log);

	int64 start = getTimeMs64();
	DecisionTree decisionTree(trainingSamples, log);
	//for(int i = 0; i < 1000; ++i) {
		decisionTree.breadthFirstTraverse();
	//}
    std::cout << "execution time: " << getTimeMs64() - start << " ms" <<std::endl;
	return 0;
}
