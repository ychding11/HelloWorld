/*
 * Node.h
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

#ifndef _NODE_H_
#define _NODE_H_

#include <string>
#include <vector>
#include <list>

class Node 
{
public:
	virtual ~Node() { }
};

/*************************************************
 *  class InnerNode
 * 
 *  Type defined for Inner Node of decision tree.
*************************************************/
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

/*************************************************
 *  class LeafNode
 * 
 *  Type defined for Leaf Node of decision tree.
*************************************************/
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
