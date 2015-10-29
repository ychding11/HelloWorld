

#include "Node.h"

// InnerNode implementation
InnerNode::InnerNode(std::string attrib): mAttribute(attrib), mChildrenNum(0) 
{
    
}

int InnerNode::getChildrenNum() const 
{
	return mChildrenNum;
}

std::string InnerNode::getAttribute() const 
{
	return mAttribute;
}

void InnerNode::addChild(Node *const child) 
{
	mpChildren.push_back(child);
	++mChildrenNum;
}

typename InnerNode::iterator InnerNode::begin() 
{
	return mpChildren.begin();
}

typename InnerNode::iterator InnerNode::end() 
{
	return mpChildren.end();
}

InnerNode::~InnerNode() 
{
}

/*-------------- LeafNode implementation -----------------------------------*/
LeafNode::LeafNode(const std::string& str): value(str), itemNum(0)
{
}

LeafNode::~LeafNode() 
{
}

std::string LeafNode::getValue() const 
{
	return value;
}

int LeafNode::getItemNum()const
{
	return itemNum;
}

void LeafNode::addItem(const std::string* item) 
{
	items.push_back(item);
	++itemNum;
}

typename LeafNode::iterator LeafNode::begin() 
{
	return items.begin();
}

typename LeafNode::iterator LeafNode::end() 
{
	return items.end();
}



