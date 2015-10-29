

#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <vector>
#include <list>

class Node 
{
public:
	virtual ~Node() 
	{ }
};

class InnerNode: public Node 
{
private:
	std::string mAttribute;
	std::list<Node*> mpChildren;
	int mChildrenNum;

public:
	typedef std::list<Node*>::iterator iterator;

	InnerNode(std::string attrib);
	~InnerNode();
	int getChildrenNum()const;
	std::string getAttribute()const;
	void addChild(Node *const child);
	iterator begin();
	iterator end();		
};

class LeafNode: public Node 
{
private:
	std::string value;
	std::vector<const std::string*> items;
	int itemNum;

public:
	typedef std::vector<const std::string*>::iterator iterator;

	LeafNode(const std::string& str);
	~LeafNode();
	std::string getValue()const;
	int getItemNum()const;
	void addItem(const std::string* item);
	iterator begin();
	iterator end();
	
};

#endif
