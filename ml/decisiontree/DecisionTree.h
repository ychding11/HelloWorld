/*
 * DecisionTree.h
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

#ifndef _DECISION_TREE_H_
#define _DECISION_TREE_H_

#include <vector>
#include <list>
#include <map>
#include "Samples.h"
#include "Node.h"
#include "Logger.h"

/*
 * PLAN: 
 * Write a general serialize/deserialize framework in C++, 
 * and use it to store the decision tree to disk for later use.
 */
class DecisionTree 
{
private:
	const Samples<std::string>& mTrainingSamples;
	int mFeatureNum;
	int mSampleNum;
	InnerNode* mpRoot;
	std::list<int> mSplitColumns;
    Logger & mlog;
public:
	DecisionTree(const Samples<std::string>& samples, Logger &log);
	~DecisionTree();
	Node* getRoot()const;
	void breadthFirstTraverse()const;
	
private:
	void build(InnerNode*& parent, 
	           const std::string& policyOrValue, 
	           const std::vector<int>& indexes, 
	           std::list<int>& nonSplitColumns);
	bool inSameCategory(const std::vector<int>& indexes) const;
	void splitItems(const std::vector<int>& indexes, 
	                std::list<int>& nonSplitColumns, 
	                std::map<std::string, 
	                std::vector<int> >& splits, 
	                int& splitColumn);
	float calcBaseEntropy() const;
	float calcEntropy(const int col) const;
	void destroy(Node* node);
};

#endif
