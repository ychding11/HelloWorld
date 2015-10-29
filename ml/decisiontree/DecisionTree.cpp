

#include <cmath>
#include <limits>
#include <map>
#include <queue>
#include <iostream>
#include "DecisionTree.h"
#include "Utils.h"

DecisionTree::DecisionTree(const Samples<std::string>& samples, Logger &log)
             : mpRoot(NULL), mTrainingSamples(samples)
	         , mFeatureNum(samples.getFeatureNum())
	         , mSampleNum(samples.getSampleNum()) 
	         , mlog(log)
{
    mlog << "Construct DecisionTree." << std::endl;
	std::vector<int> indexes(mSampleNum);
	for(int i = 0; i < mSampleNum; ++i)	indexes[i] = i;

	std::list<int> nonSplitColumns;
	for(int i = 0; i < mFeatureNum; ++i) nonSplitColumns.push_back(i);
	
	build(mpRoot, "", indexes, nonSplitColumns);
	mlog << "Construct DecisionTree Finished." << std::endl;
}

Node* DecisionTree::getRoot() const 
{
	return mpRoot;
}

/*************************************************
 * Function(s)
 * Test whether all samples in the same category.
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

/*
 * Calculates entropies based on features in 'nonSplitColumns' and 
 * find the feature on which we will get the maximum entropy bases.
 * Split the items indexed by 'indexes' based on the feature found, 
 * store the feature index in 'splitColumn' and splits' indexes in splits.
 *
 * [in] nonSplitColumns: input parameter, the columns hasn't been split.
 * [in] indexes: indexes of items in 'items' to split in this round.
 * [out] splitColumn: output parameter, the column on which the items will be split based in this round.
 * [out] splits: output parameter, the <feature value, item list> pairs after split in this round.
 */
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

void DecisionTree::breadthFirstTraverse() const 
{
	if(mpRoot == NULL)
	{
	    mlog << "Tree is empty." << std::endl;
		return;
	}
    mlog << "Decision Tree Info :" << std::endl;
    mlog << "------------------------------" << std::endl;
	std::vector<int> nodeNumPerLevel(10, 0);
	++nodeNumPerLevel[0];
	
	std::queue<Node*> que;
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
