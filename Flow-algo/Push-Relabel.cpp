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
	res_adj(){}
	res_adj(int i,int c):ind(i),Cf(c){}
};

struct res_node {
	int exc = 0;//excess in this vertex
	vector<vector<res_adj>>H;//nodes adjacent in the residual graph(on the basis of their heights)
	vector<int>pos;/*posn of the node in the respective Height list(H), -1 if the node
					 is not adjacent in the Residual graph	*/
};

struct residual_graph {
	vector<res_node>V;
	int source;
	int sink;

	residual_graph(vector<vector<int>>&C,int so,int si):source(so),sink(si)
	{
		int n = C.size();
		V.resize(n);
		for (int i = 0; i < n; i++)
		{
			V[i].H.resize(2 * n - 1);
			V[i].pos.resize(n,-1);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if(C[i][j] > 0)
				{
					if(j == source)
					{
						V[i].H[n].emplace_back(j,C[i][j]);//h[source] == n
						V[i].pos[j] = V[i].H[n].size()-1;
					}
					
					else
					{
						V[i].H[0].emplace_back(j,C[i][j]);//h[j] == 0 for j!=source
						V[i].pos[j] = V[i].H[0].size() -1;
					}
				}
			}
		}
	}
	
	
};

int max_flow(vector<vector<int>>C,int source, int sink,vector<vector<int>>F)
//Capacity graph in the form of adjacency matrix
//F will have preflows at every stage and finally max-flow at termination of the while loop
{
	int n = C.size();//number of vertices

	vector<vector<int>>h(2 * n - 1);/*Stores vertices with positive excess(based on their heights)
									maximum possible height is 2*n-1*/
	
	residual_graph RG(C,source,sink);//Residual graph of size n
	for(int i=0;i<)
	
	bool there_is_excess = false;
	for(int i = RG.V[source].H[0].size()-1;i>=0;i--)
	{
		res_adj &r = RG.V[source].H[0][i];
		int j = r.ind;
		int cf = r.Cf;
		RG.V[source].H[0].pop_back();
		F[source][j] += cf;
		RG.V[j].exc += cf;
		RG.V[j].H[n].emplace_back(source,cf);
		there_is_excess = true;
	}
	
	int max_exc_h = 0;//maximum height among heights of vertices with +ve excess
	
	while(there_is_excess)
	{
		
	}
}


int main()
{
    return 0;
}
