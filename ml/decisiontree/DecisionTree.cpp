/*
 * DecisionTree.cpp
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
 
#include <cmath>
#include <limits>
#include <map>
#include <queue>
#include <iostream>
#include "DecisionTree.h"
#include "Utils.h"

/*************************************************
 * Constructor.
*************************************************/
DecisionTree::DecisionTree(const Samples<std::string>& samples, Logger &log)
             : mpRoot(NULL), mTrainingSamples(samples)
	         , mFeatureNum(samples.getFeatureNum())
	         , mSampleNum(samples.getSampleNum()) 
	         , mlog(log)
{
    mlog << "Construct DecisionTree." << std::endl;
	std::vector<int> indexes(mSampleNum, 0);
	for(int i = 0; i < mSampleNum; ++i)	indexes[i] = i;

	std::list<int> nonSplitColumns;
	for(int i = 0; i < mFeatureNum; ++i) nonSplitColumns.push_back(i);
	
	build(mpRoot, "", indexes, nonSplitColumns); // learn processing
	mlog << "Construct DecisionTree Finished." << std::endl;
}

Node* DecisionTree::getRoot() const 
{
	return mpRoot;
}

/*************************************************
 * Check whether all samples in the same category.
*************************************************/
bool DecisionTree::inSameCategory(const std::vector<int>& indexes) const 
{
	const std::string* categories = mTrainingSamples.getCategories();
	auto iter = indexes.begin(); 
	std::string category = categories[*iter];
	while(++iter != indexes.end())
	{	
	    if(categories[*iter] != category) return false; 
	}
	return true;
}

/*
 * Splits items indexed by 'indexes', and create an InnerNode for the feature on which the split based.
 * Add the InnerNode as a link to 'parent', and builds links for the InnerNode recursively.
 * When we run out of the features to split, or the items indexed by 'indexes' are in the same class, 
 * a LeafNode is created and function 'build' returns.
 */
void DecisionTree::build(InnerNode*& parent, 
                         const std::string& policyOrValue, 
	                     const std::vector<int>& indexes, 
	                     std::list<int>& nonSplitColumns) 
{
	// if no feature to split any more, create a left node and add the items indexed by 'indexes' to it.
	if((nonSplitColumns.empty() == true) || (inSameCategory(indexes) == true)) 
	{
		LeafNode* leafNode = new LeafNode(policyOrValue);
		for(auto p = indexes.begin(); p != indexes.end(); ++p)
		{	
		    //leaf Node contains all samples with the same category.
		    leafNode->addItem(mTrainingSamples.getSample(*p));		
		}
		parent->addChild(leafNode);
		mlog << "Construct a Leaf Node : " << policyOrValue << std::endl;
		return;
	}

	int splitColumn;
	std::map<std::string, std::vector<int> > splits; // [Feature value, samples]
	splitItems(indexes, nonSplitColumns, splits, splitColumn);

	// create an inner node for the feature in 'splitColumn'
	InnerNode* innerNode = NULL;
	if(parent == NULL)
	{	
	    innerNode = parent = new InnerNode(mTrainingSamples.getFeatureName(splitColumn));
	    mlog << "Construct Root Node : " << mTrainingSamples.getFeatureName(splitColumn) << std::endl;
	}
	else
	{
	    innerNode = new InnerNode(mTrainingSamples.getFeatureName(splitColumn) + "-"+ policyOrValue );
		parent->addChild(innerNode);
		mlog << "Construct Inner Node : " << mTrainingSamples.getFeatureName(splitColumn) + "-"+ policyOrValue << std::endl;
    }
			
	for(auto iter = splits.begin(); iter != splits.end(); ++iter) 
	{
		std::list<int> subNonSplitColumns(nonSplitColumns);
		build(innerNode, (*iter).first, (*iter).second, subNonSplitColumns);
	}
}

/*************************************************
 * split samples that has not been split yet.
 *
 * [in] nonSplitColumns: feartures has not been 
 *      selected yet.
 * [in] indexes: indexes of samples to split under
 *               current round.
 * [out] splitColumn: fearture selected according 
 *                    to entropy.
 * [out] splits: <feature value, item list> map,
 *               the split result according to 
 *               fearture selected.
*************************************************/
void DecisionTree::splitItems(const std::vector<int>& indexes, 
                              std::list<int>& nonSplitColumns, 
	                          std::map<std::string, std::vector<int> >& splits, 
	                          int& splitColumn) 
{
	double maxEntropy = std::numeric_limits<double>::min();
	std::list<int>::iterator selectedColumn = nonSplitColumns.end();

	for(auto iter = nonSplitColumns.begin(); iter != nonSplitColumns.end(); ++iter) 
	{
		double entropy = calcEntropy(*iter);
		if(maxEntropy < entropy) 
		{
			maxEntropy     = entropy;
			selectedColumn = iter;
		}
	}
	if (selectedColumn == nonSplitColumns.end())
	{
	    mlog << "Select None Colum." << std::endl;
	    return;
	}
	nonSplitColumns.erase(selectedColumn);
	splitColumn = *selectedColumn;
	
    mlog << ">>>>Select Column : " << splitColumn << " Begin Split..." << std::endl;
	for(auto p = indexes.begin(); p != indexes.end(); ++p)
	{	
	    mlog << "Feature " << splitColumn << " " 
	         << mTrainingSamples.getSample(*p)[splitColumn] << " Sample Index " 
	         << *p << std::endl;
	    splits[mTrainingSamples.getSample(*p)[splitColumn]].push_back(*p);
	}
	mlog << "<<<<Select Column : " << splitColumn << " Finish Split..." << std::endl;
}

float DecisionTree::calcBaseEntropy()const 
{
	return calcEntropy(mFeatureNum-1);
}

float DecisionTree::calcEntropy(const int col)const
{
    mlog << "Caculate Entropy for Col : "<< col << std::endl; 
	std::map<std::string, int> valueFrequency;

	for(int row = 0; row < mSampleNum; ++row)
	{	
	    ++valueFrequency[mTrainingSamples.getSample(row)[col]];
	}
		
	float entropy = 0.0;
	for(auto iter = valueFrequency.begin(); iter != valueFrequency.end(); ++iter) 
	{
		float probability = (*iter).second / static_cast<float>(mSampleNum);
		entropy += probability * logN(probability, 2);
		mlog << "Value : " << (*iter).first << " Occurs " << (*iter).second 
		     << " times. " << std::endl;
		    
	}
	mlog << "Entropy = " << entropy << std::endl;
	return -entropy;
}

/*************************************************
 * traverse the built decision tree by level.
 * it use queue struct
*************************************************/
void DecisionTree::breadthFirstTraverse() const 
{
	if(mpRoot == NULL)
	{
	    mlog << "Tree is empty." << std::endl;
		return;
	}
    mlog << "Decision Tree Info :" << std::endl;
    mlog << "------------------------------" << std::endl;
	std::vector<int> nodeNumPerLevel(10, 0); //suppose max level is 10
	++nodeNumPerLevel[0];
	
	std::queue<Node*> que; //base class pointer to reference
	que.push(mpRoot);
	
	int level = 0;
	int visitedNodeNum = 0;
	while(!que.empty()) 
	{
		Node* node = que.front(); que.pop();
		++visitedNodeNum;
		
		InnerNode* innerNode = dynamic_cast<InnerNode*>(node);
		if(innerNode) 
		{
			mlog << innerNode->getAttribute() << " ";			
			for(auto iter = innerNode->begin(); iter != innerNode->end(); ++iter)
			{
				que.push(*iter);
				++nodeNumPerLevel[level+1];
			}
		}
		else 
		{
			LeafNode* leafNode = dynamic_cast<LeafNode*>(node);
			if(leafNode) 
			{
				mlog << leafNode->getValue() <<": ";
				mlog << "[";
				for(auto iter = leafNode->begin(); iter != leafNode->end(); ++iter) 
				{
					const std::string* item = *iter;
					mlog << "(";
					int i;
					for(i = 0; i < mFeatureNum; ++i) mlog << item[i] << ", ";
					mlog << item[i] << ") ";
				}
				mlog << "]" << " ";
			} 
			else 
			{
				mlog << "Invalid Node type!" << std::endl;
			}
		}
		if(nodeNumPerLevel[level] == visitedNodeNum) 
		{
			visitedNodeNum -= nodeNumPerLevel[level];
			++level;
			mlog << std::endl;
		}
	}
	mlog << "------------------------------" << std::endl;
}

/* destory the decision tree recursivly */
void DecisionTree::destroy(Node* node) 
{
	if(node == 0)
	{
	    return;
	}
	InnerNode* innerNode = dynamic_cast<InnerNode*>(node);
	if(innerNode != 0) 
	{
		InnerNode::iterator iter = innerNode->begin();
		while(iter != innerNode->end()) 
		{
			destroy(*iter);
			++iter;
		}
	}
	delete node;
}

DecisionTree::~DecisionTree() 
{
	destroy(mpRoot);
}
