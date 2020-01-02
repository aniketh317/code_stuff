// Push-Relabel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>

//Some implementation of push-Relabel for O(V^3) run-time
using namespace std;

//res_ent->residual graph related entries

struct res_adj {
	int ind;//index of the node adjacent in the residual graph
	int Cf;//residual capacity of the edge
	res_adj() {}
	res_adj(int i, int c) :ind(i), Cf(c) {}
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

	residual_graph(vector<vector<int>>&C, int so, int si) :source(so), sink(si)
	{
		int n = C.size();
		V.resize(n);
		for (int i = 0; i < n; i++)
		{
			V[i].H.resize(2 * n - 1);
			V[i].pos.resize(n, -1);
		}
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				if (C[i][j] > 0)
				{
					if (j == source)
					{
						V[i].H[n].emplace_back(j, C[i][j]);//h[source] == n
						V[i].pos[j] = V[i].H[n].size() - 1;
					}

					else
					{
						V[i].H[0].emplace_back(j, C[i][j]);//h[j] == 0 for j!=source
						V[i].pos[j] = V[i].H[0].size() - 1;
					}
				}
			}
		}
	}


};

int max_flow(vector<vector<int>>C, int source, int sink, vector<vector<int>>F)
//Capacity graph in the form of adjacency matrix
//F will have preflows at every stage and finally max-flow at termination of the while loop
{
	int n = C.size();//number of vertices

	vector<vector<int>>h(2 * n - 1);/*Stores vertices with positive excess(based on their heights)
									maximum possible height is 2*n-1*/

	residual_graph RG(C, source, sink);//Residual graph of size n
	for (int i = 0; i < n; i++)
	{
		if (i != source && i != sink)
			h[0].push_back(i);
	}

	int num_exc = 0;//number of nodes with +ve excess
	for (int i = RG.V[source].H[0].size() - 1; i >= 0; i--)
	{
		res_adj &r = RG.V[source].H[0][i];
		int j = r.ind;
		int cf = r.Cf;
		RG.V[source].H[0].pop_back();
		F[source][j] += cf;
		RG.V[j].exc += cf;
		RG.V[j].H[n].emplace_back(source, cf);
		num_exc++;
	}

	int max_exc_h = 0;//maximum height among heights of vertices with +ve excess

	while (num_exc>0)
	{
		int siz = h[max_exc_h].size();
		int u = h[max_exc_h][siz - 1];
		int dh = max_exc_h - 1;//downhill height
		if (RG.V[u].H[dh].size() != 0)
		{
			/*PUSH along an edge which has terminating node at height dh*/
			
			int h = max_exc_h;//height of the current vertex
			res_adj &r = RG.V[u].H[dh].back();
			int d = min(r.Cf, RG.V[u].exc);
			
			r.Cf -= d;
			
			if (C[u][r.ind] > 0)
				F[u][r.ind] += d;
			else
				F[r.ind][u] -= d;

			RG.V[u].exc -= d;
			RG.V[r.ind].exc += d;

			if (RG.V[r.ind].pos[u] == -1)
				RG.V[r.ind].H[h].emplace_back(u, d);
			else
				RG.V[r.ind].H[h][RG.V[r.ind].pos[u]].Cf += d;
			
			if (r.Cf == 0)
				RG.V[u].H[dh].pop_back();
			if(RG.V[)
		}
	}
}


int main()
{
	return 0;
}
