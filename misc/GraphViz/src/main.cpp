#include <iostream>
#include <cstdio>
#include <fstream>
#include <cmath>
#include <ctime>
#include <sstream>
#include "SimpleGraph.h"

using namespace std;

#define DEBUG
const double kPi = 3.14159265358979323;
const double kRepel = 0.01;
const double kAttrack = 100;

void Welcome();

/*************************************************
 * Function: 
 *   
 * Param[in]:   
 *   
 * Retrun: 
 * Ideas:  
 * Notice:   
*************************************************/
string getInput()
{
	string ret;
	getline(cin, ret);
	return ret;
}

int PromptAlgorithmTime()
{
	while (true)
	{
		cout << "Please Enter expected run time in seconds" << endl;
		stringstream ss; 
		ss << getInput();
		int dTime;
		if (ss >> dTime)
		{
			char c;
			if (ss >> c)
			{
				cout << "Invalid input." << endl;
			}
			else
			{
				return dTime;
			}
		}
		else
		{
			cout << "Input is not a nuber, try again." << endl;
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
 * Ideas:  
 * Notice:   
*************************************************/
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

/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[out]:   
 *   
 * Retrun: 
 * Ideas:  
 * Notice:   
*************************************************/
Node constructNode(size_t id, size_t nodeNum)
{
	Node ret;
	ret.x = cos(2 * kPi * ((double)id / (double)nodeNum));
	ret.y = sin(2 * kPi * ((double)id / (double)nodeNum));
	#if defined DEBUG
	printf("node %d, initial position [%lf, %lf]\n", id, ret.x, ret.y);
	#endif
	return ret;
}

/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[out]:   
 *   
 * Retrun: 
 * Ideas:  
 * Notice:   
*************************************************/
SimpleGraph loadGraph()
{
	string filename = promptFileName();
	ifstream filestream(filename.c_str());
	SimpleGraph graph;
	size_t num;
	size_t start, end;
	filestream >> num ;
	if (filestream.fail())
	{
		cout << "Read node number error." << endl;
		return graph;
	}
	while (filestream >> start >> end)
	{
		Edge edge;
		edge.start = start;
		edge.end = end;
		graph.edges.push_back(edge);
	}
	for (size_t i = 0; i < num; ++i)
	{
		Node node = constructNode(i, num);
		graph.nodes.push_back(node);
	}
	return graph;
}

/*************************************************
 * Function: 
 *   
 * Param[in]:   
 * Param[out]:   
 *   
 * Retrun: 
 * Ideas:  
 * Notice:   
*************************************************/
void repelDetPos(const SimpleGraph &graph, vector<Node> &detpos)
{
	int n = graph.nodes.size();
	for (int i = 0; i < n -1; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			double x0 = graph.nodes[i].x;
			double y0 = graph.nodes[i].y;
			double x1 = graph.nodes[j].x;
			double y1 = graph.nodes[j].y;
			double len = sqrt((x0 -x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
			Node p1toP0((x0 - x1) / len, (y0 - y1) / len); // it is a unit direction
			double repelForce = kRepel / len;
			detpos[i].x += repelForce * p1toP0.x;
			detpos[i].y += repelForce * p1toP0.y;
			detpos[j].x -= repelForce * p1toP0.x;
			detpos[j].y -= repelForce * p1toP0.y;
			#if defined DEBUG
			printf("node<%d, %d>, [%lf,  %lf] [%lf, %lf], distance = %lf, repelForce = %lf\n"
			       "\tdetForce=[%lf, %lf], \tdetpos=[%lf, %lf] [%lf, %lf]\n",
			       i, j, x0, y0, x1, y1, len, repelForce,
				repelForce * p1toP0.x, repelForce * p1toP0.y,
			       detpos[i].x, detpos[i].y, detpos[j].x, detpos[j].y);
			#endif
		}
	}
}

void attrackDetPos(const SimpleGraph &graph, vector<Node> &detpos)
{
	int n = graph.edges.size();
	for (int i = 0; i < n -1; ++i)
	{
		int start = graph.edges[i].start;
		int end = graph.edges[i].end;
		double x0 = graph.nodes[start].x;
		double y0 = graph.nodes[start].y;
		double x1 = graph.nodes[end].x;
		double y1 = graph.nodes[end].y;
		double len = sqrt((x0 -x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
		Node p1toP0((x0 - x1) / len, (y0 - y1) / len); // it is a unit direction
		double attrackForce = kAttrack * ((x0 -x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
		detpos[start].x -= attrackForce * p1toP0.x;
		detpos[start].y -= attrackForce * p1toP0.y;
		detpos[end].x += attrackForce * p1toP0.x;
		detpos[end].y -= attrackForce * p1toP0.y;

		#if defined DEBUG
		printf("node<%d, %d>, [%lf,  %lf] [%lf, %lf], distance = %lf, attrackForce = %lf\n"
		       "\tdetForce=[%lf, %lf], \tdetpos=[%lf, %lf] [%lf, %lf]\n", 
			start, end, x0, y0, x1, y1, len, attrackForce,
			attrackForce * p1toP0.x, attrackForce * p1toP0.y,
			detpos[start].x, detpos[start].y, detpos[end].x, detpos[end].y);
		#endif
	}
}

void applyDetPos(SimpleGraph &graph, vector<Node> &detpos)
{
	int n = graph.nodes.size();
	for (int i = 0; i < n; ++i)	
	{
		graph.nodes[i].x += detpos[i].x;
		graph.nodes[i].y += detpos[i].y;
		#if defined DEBUG
		printf("node %d position [%lf, %lf]\n", i, graph.nodes[i].x, graph.nodes[i].y);
		#endif
	}
}

void forceDirectedAlgorithm(SimpleGraph &graph)
{
	int n = graph.nodes.size();
	vector<Node> detPos(n, Node(0,0)); // check Node default constructor 
	repelDetPos(graph, detPos);
	attrackDetPos(graph, detPos);
	applyDetPos(graph, detPos);
}

// Main method
int main()
{
    Welcome();
    do
    {
    	SimpleGraph graph = loadGraph();
	int expectedTime = PromptAlgorithmTime();
	time_t endTime, startTime = time(NULL);
	cout << "begin running algorithm..." << endl;
	while (true)	
	{
		forceDirectedAlgorithm(graph);
		endTime = time(NULL);
		if (endTime - startTime >= expectedTime)
		{			
			//cout << "end running algorithm..." << endl; 
			//break;
		}
	}
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
