/*
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

#include<iostream>
#include<stack>
#include<vector>
#include<climits>
#include<cstdio>

using namespace std;

struct TreeNode
{
	int val;
	TreeNode *left, *right;

	TreeNode(int v) : val(v), left(NULL), right(NULL) {}
};

/*************************************************
 * Function: given a integer sequence, judge whether
 * it is a valid pre-order traversal of bst.
 *   
 * Param[in]:  integer sequence	 
 *   
 * Retrun: a bool indicating valid or not 
 *   
 * Notice:   
*************************************************/
bool isValidBSTPreorderSequence(vector<int> &seq)
{
	if (seq.empty()) return true; // empty tree
	int rootVal = INT_MIN;
	stack<int> st;
	for (int i = 0; i < seq.size(); i++)
	{
		if (seq[i] < rootVal) return false;
		while (!st.empty() && st.top() < seq[i]) // eat right subtree node
		{
			rootVal = seq[i];
			st.pop();
		}
		st.push(seq[i]);
	}
	return true;
}

/*************************************************
 * Function: for a node print all its ancestors in
 * bst.
 *   
 * Param[in]: root of a bst   
 * Param[in]: searched node 
 *   
 * Retrun: 
 *   
 * Notice:  suppose the searched node is in root 
*************************************************/
void printAncestors(TreeNode *root, TreeNode *e)
{
	if (!root || !e) return;
	stack<TreeNode*> st;
	while (true)
	{
		while (root && root->val != e->val)
		{
			st.push(root);
			root = root->left;
		}
		if (root && root->val == e->val) break;
		if (st.top()->right == NULL) //here, stack top elements has no left child
		{
			TreeNode *temp = st.top(); st.pop();
			while (!st.empty() && st.top()->right == temp)
			{
				temp = st.top(); st.pop();
			}

		}	
		root = st.empty()? NULL : st.top()->right;
	}
	//print elements in stack, 
	//all elements in stack now is ancestors of searched node
	while (!st.empty()) 
	{
		printf("%d ", st.top()->val);
		st.pop();
	}
	printf("\n");
}

/*************************************************
 * Function: print elements in a binary tree in post
 * order by interative way.
 *   
 * Param[in]:  a binary tree root 
 *   
 * Retrun: none
 *   
 * Notice:  implements iterative method by two stacks 
*************************************************/
void postOrderIterativeTraversal(TreeNode * root)
{
	if (!root) return;
	stack<TreeNode*> stk1, stk2;
	stk1.push(root);
	while (!stk1.empty())
	{
		TreeNode *temp = stk1.top(); stk1.pop();
		stk2.push(temp);
		if (temp->left) stk1.push(temp->left);
		if (temp->right) stk1.push(temp->right);
	}
	while (!stk2.empty())
	{
		printf("%d ", stk2.top()->val);
		stk2.pop();
	}
	printf("\n");
}

int main()
{
	return 0;
}
