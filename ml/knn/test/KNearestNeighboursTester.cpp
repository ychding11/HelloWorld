/*
 * This a simple tester source file.
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

#include <iostream>
#include "KNearestNeighbours.h"
#include "Timing.h"
#include "Logger.h"

int main(int argc, char** argv) 
{
	std::string trainingDir = "trainingDigits";
	std::string testDir     = "testDigits";
	Logger log("log.txt");
	int k = 3;
	int64 start = getTimeMs64();
	InputDataSet trainingSet(trainingDir, log), testSet(testDir, log);
	log << "Time of constructing training and test sets: "<< getTimeMs64() - start << "ms" <<std::endl;

	start = getTimeMs64();
	Logger knnlog("knn.txt");

	KNearestNeighbours kNearestNeighbours(trainingSet, testSet, k, knnlog);
	double errorRate = kNearestNeighbours.getErrorRate();
	log << "ErrorRate = " << errorRate << std::endl;
	log << "Time of K-Nearest Neighbours algorithm Test: " << getTimeMs64() - start << "ms" << std::endl;

	return 0;
}
