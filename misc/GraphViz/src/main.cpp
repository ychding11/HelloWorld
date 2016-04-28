#include <iostream>
#include <cstdio>
#include <fstream>
#include "SimpleGraph.h"

using namespace std;

void Welcome();
string getInput()
{
	string ret;
	getline(cin, ret);
	return ret;
}

string promptFileName()
{
	while (true)
	{
        	cout << "Input graph name:" << endl;
        	string filename = getInput();
        	ifstream filestream(filename.c_str());
        	if (filestream.is_open())
        	{
        		filestream.close();
        		return filename;
        	}
        	else
        	{
        		cout << "graph name error." << endl;
        	}
	}
}

Node constructNode(size_t id, size_t nodeNum)
{
	Node ret;
	ret.x = 0, ret.y = 0;
	return ret;
}
SimpleGraph loadGraph()
{
	string filename = promptFileName();
	ifstream filestream(filename.c_str());
	SimpleGraph graph;
	size_t num;
	filestream >> num ;
	if (filestream.fail())
	{
		cout << "Read node number error." << endl;
		return graph;
	}
	for (size_t i = 0; i < num; ++i)
	{
		Node node = constructNode(i, num);
		graph.nodes.push_back(node);
	}
	size_t start, end;
	while (filestream >> start >> end)
	{
		Edge edge;
		edge.start = start;
		edge.end = end;
		graph.edges.push_back(edge);
	}
	return graph;
}

// Main method
int main()
{
    Welcome();
    do
    {
    	SimpleGraph graph = loadGraph();
    	cout << "Type \"begin\" to load a graph or type ENTER to quit." << endl;
    } while(getInput() == "begin");
    printf("Ready to quit.\n");
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */
void Welcome() {
    cout << "Welcome to CS106L GraphViz!" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}
