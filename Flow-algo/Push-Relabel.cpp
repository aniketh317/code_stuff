// Push-Relabel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

//Custom implementation of push-Relabel
using namespace std;

//res_ent->residual graph related entries

struct res_adj {
	int ind;//index of the node adjacent in the residual graph
	int Cf;//residual capacity of the edge
};

struct res_node {
	int exc = 0;//excess in this vertex
	vector<vector<res_adj>>H;//nodes adjacent in the residual graph(on the basis of their heights)
	vector<int>pos;/*posn of the node in the respective Height list(H), -1 if the node
					 is not adjacent in the Residual graph	*/
};

struct residual_graph {
	vector<res_node>V;

	residual_graph(vector<vector<int>>&C)
	{
		int n = C.size();
		V.resize(n);
		for (int i = 0; i < n; i++)
		{
			V[i].H.resize(2 * n - 1);
			V[i].pos.resize(n);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{

			}
		}
	}
};

int max_flow(vector<vector<int>>C,int source, int sink,vector<vector<int>>F)
//Capacity graph in the form of adjacency matrix
{
	int n = C.size();//number of vertices

	vector<vector<int>>h(2 * n - 1);/*Stores vertices with positive excess(based on their heights)
									maximum possible height is 2*n-1*/
	
	vector<res_node>RG(n);//Residual graph of size n
}


int main()
{
    return 0;
}

