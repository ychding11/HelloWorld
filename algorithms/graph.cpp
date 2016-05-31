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
#include<cassert>
#include<vector>
#include<iostream>
#include<stack>
 
using namespace std;
 
class Graph
{
public:
    Graph(int vetexs = 0);  // Constructor

    void addEdge(int u, int v) // Add an edge to graph
    {
        mAdjList[u].push_back(v); 
    }

    void DFS(int s);  // Traversal graph by DFS from a given source s
    bool isConnected(int s, int d);

private:
    int mVertex;    
    vector<vector<int>> mAdjList;    // adjacency vectors
};
 
Graph::Graph(int vertex)
: mVertex(vertex)
, mAdjList(vertex, vector<int>())
{ }
 
void Graph::DFS(int s)
{
    assert(s >=0 && s < mVertex);
	vector<int> visited(mVertex,false);
	stack<int> stack;
    stack.push(s);
	visited[s] = true; // mark 
	while (!stack.empty())
	{
		int cur = stack.top(); stack.pop();
		cout << cur << " "; //visit vertice from stack top
 
	    //vector<int>::iterator i;
        for (auto i = mAdjList[cur].begin(); i != mAdjList[cur].end(); ++i)
        {
            if (!visited[*i])
            {
                stack.push(*i);
                visited[*i] = true;
            }
        }
    }
}

bool Graph::isConnected(int s, int d)
{
    assert( s>=0 && s < mVertex && d >= 0 && d < mVertex);
    if (s == d) return true;

	vector<int> visited(mVertex,false);
	stack<int> stack;
    stack.push(s);
	visited[s] = true; // mark 
	while (!stack.empty())
    {
		int cur = stack.top(); stack.pop();
        if (cur == d) return true; 
	    //vector<int>::iterator i;
        for (auto i = mAdjList[cur].begin(); i != mAdjList[cur].end(); ++i)
        {
            if (!visited[*i])
            {
                stack.push(*i);
                visited[*i] = true;
            }
        }
    }
    return false;
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
    Graph g(6);  //  6 vertices in graph
    /*
     *        0             5
     *     1     2
     *     3     4
     */
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 0);
    g.addEdge(1, 3);
    g.addEdge(3, 1);
    g.addEdge(2, 0);
    g.addEdge(2, 4);
    g.addEdge(4, 2);
 
    cout << "Following is Depth First Traversal "
            "(starting from vertex 0) \n";
    g.DFS(0);
	cout << std::endl;

    for (int i = 0; i < 6; ++i)
    {
        for (int j = 0; j < 6; ++j)
        {
            printf("Node %d ---> Node %d conneted? %d\n", i, j, g.isConnected(i, j));
        }
    }

	return 0;
}
