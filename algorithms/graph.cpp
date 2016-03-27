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

#include<cstdio>
#include<vector>
#include<iostream>
#include<stack>
 
using namespace std;
 
class Graph
{
private:
    int mVertexs;    
    vector<int> *mAdjList;    // adjacency vectors

public:
    Graph(int vetexs);  // Constructor
    void addEdge(int v, int w); // add an edge to graph
    void DFS(int s);  // traversal graph by DFS from a given source s
};
 
Graph::Graph(int vertex) : mVertexs(vertex)
{
    mAdjList = new vector<int>[mVertexs]; //check status needed
}
 
void Graph::addEdge(int v, int w)
{
    mAdjList[v].push_back(w); 
}

void Graph::DFS(int s)
{
	vector<int> visited(mVertexs,false);
	stack<int> stack;
	visited[s] = true; stack.push(s);
	while (!stack.empty())
	{
		int cur = stack.top();
		stack.pop();
		cout << cur << " ";
 
	    vector<int>::iterator i;
        for (i = mAdjList[cur].begin(); i != mAdjList[cur].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true; stack.push(*i);
            }
        }
    }
}
 
/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[out]:   
 *   
 * Retrun: 
 *   
 * Notice:   
*************************************************/
int main()
{
    Graph g(5);  //  5 vertices in graph
    g.addEdge(1, 0);
    g.addEdge(0, 2);
    g.addEdge(2, 1);
    g.addEdge(0, 3);
    g.addEdge(3, 4);
    g.addEdge(4, 0);
 
    cout << "Following is Depth First Traversal "
            "(starting from vertex 0) \n";
    g.DFS(0);
	cout << std::endl;
	return 0;
}
