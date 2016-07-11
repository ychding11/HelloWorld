/*
 * Copyright (C) 2014-2016  Yaochuang Ding - <ych_ding@163.com>
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
 
#include <vector>

using std::vector;

#define PERFORMANCE_METER

template <typename T>
struct BstNode 
{
    T val;
    BstNode *left, *right;
    BstNode(T x) : val(x), left(NULL), right(NULL) { }
};

template <typename T>
class BstTree
{
private:
BstNode<T>* _root;

public:
BstTree(BstNode<T> *root = NULL) :_root(root) {}
BstTree(const BstTree &rhs); // copy constructor
~BstTree();

BstTree* copy(const BstTree &rhs); // copy constructor
BstTree& operator=(const BstTree &rhs);
bool insert(T elem);
bool remove(T elem);
bool remove(BstNode<T> *node);
BstNode<T>* find(T elem);

private:
BstNode<T>* insert(T elem, BstNode<T> *root);
};

BstNode<T>* BstTree::insert(T elem, BstNode<T> *root)
{
    if (!root) return new BstNode<T>(elem);
    if (elem < _root->val)
    {
        BstNode *temp = insert(elem, _root->left);
        !temp ? return NULL; : _root->left = temp;
    }
    else if (elem > _root->val)
    {
        BstNode *temp = insert(elem, _root->right);
        !temp ? return NULL; : _root->right = temp;
    }
    else
    {
        return NULL; // already exist.
    }
    return root;
}

