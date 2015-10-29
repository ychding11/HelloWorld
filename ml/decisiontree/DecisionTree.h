

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
