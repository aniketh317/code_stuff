// Dinics_implement.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <queue>
using namespace std;
#define fi first
#define se second

int T, N, K;

struct node {
	bool v = false;
	int d = -1;
};

struct edge;


struct flowedge {
	int v = -1;
	int w = -1;//weight/capacity of the path in the original flow network
	int f = 0;//present flow along the edge
	flowedge(int v1, int w1, int f1 = 0) :v(v1), w(w1), f(f1) {}
	flowedge(const flowedge &e) :v(e.v), w(e.w), f(e.f) {}
};

struct orig_node { //these will be the nodes of the original flow network
	vector<flowedge>adj;
	orig_node(){}
};


struct edge {
	int v = -1;//adjoining vertex in the residual graph
	bool fw = false;//does it correspond to a forward edge in orig capacity network
	int pos = -1;// posn in the adj list of the capacity network
	int wr = -1; // wr is the capacity in residual graph
	bool l = false;//is this edge there in the layered subgraph constructed out of residual graph
	edge(int v1, bool d, int p, int w) :v(v1), fw(d), pos(p), wr(w) {}
};





struct res_node //nodes in the residual graph(nodes of the original capacity network will
//come into the residual graph)
{
	int ptr = 0;//the current pointer during DFS done at the time of finding out blocking flow
	vector<edge>adj;
	res_node() {}
};



bool DFS_visit(vector<res_node>&R, int i, vector<pair<int,int>>&P)
//modified visit for finding out blocking flow
//R->Residual graph which also has layered subgraph superimposed through the bool l attribute
{
	int siz = 26;
	if (i == siz - 1)
		return true;

	bool f = false;
	int c;

	for (c = R[i].ptr; c<R[i].adj.size(); c++)
	{
		if (!R[i].adj[c].l)//no need of visit pointers since layered edges ensure that we are going 
			continue;		// in forward direction to next layer

		f = DFS_visit(R, R[i].adj[c].v, P);
		if (f)
		{
			P.push_back(make_pair(i, c));
			break;
		}
	}

	R[i].ptr = c;
	return f;//this will be true if we could find a (unsaturated)path from this node i to the sink
}


bool DFS(vector<res_node> &R, vector<pair<int,int>>&P)
{
	int siz = 26;
	bool ret = DFS_visit(R, 0, P);
	return ret;
}


int Dinics(vector<orig_node>&G)
//Assuming source is 0 and sink is (siz-1)
{
	bool p_e = true;
	while (p_e) //G at every iteration will hold the flows along the edges
	{
		int siz = G.size();
		vector<res_node>R_G;//the residual graph(this itself will be used for layered subgraph too)
		R_G.resize(siz);
		for (int i = 0; i < siz; i++)
		{
			for (int j = 0; j<G[i].adj.size(); j++)
			{
				flowedge z(G[i].adj[j]);
				int f = z.w - z.f;//this will be Cf(capacity in the residual graph)

				if (f>0)
					R_G[i].adj.emplace_back(z.v, true, j, f);

				if (z.f>0)
					R_G[z.v].adj.emplace_back(i, false, j, z.f);
			}
		}

		queue<int>Q;
		Q.push(0);
		vector<node>n;

		n.resize(siz);
		n[0].d = 0;
		n[0].v = true;

		while (!Q.empty())
		{
			int c = Q.front();
			Q.pop();
			if (c == 25)
				break;

			for (edge &z : R_G[c].adj)
			{
				if (!n[z.v].v)
				{
					n[z.v].v = true;
					z.l = true;//this edge will be a layered edge(ie. in layerted subgraph)
					n[z.v].d = n[c].d + 1;
					Q.push(z.v);
				}
				else if (n[z.v].d == n[c].d + 1)
					z.l = true;//this edge will be a layered edge(ie. in layerted subgraph)
			}
		}

		if (!n[25].v)
		{
			p_e = false;
			break;
		}
		bool pel = true;//flow can be pushed in the layered subgraph(to find blocking flow)

		while (pel)
		{
			vector<pair<int,int>>P;
			/*P will contain sequence of edges in the augmenting path of the layered subgraph
			  pair (a,b): a->node no. , b->index in the adjacency list	*/
			
			pel = DFS(R_G, P);

			if (pel)
			{
				int min_w;//minimum weight in the path found 
				int u, in;
				u = P[0].fi;
				in = P[0].se;
				min_w = R_G[u].adj[in].wr;
				for (int k = 1; k < P.size(); k++)
				{
					int u = P[k].fi;//the vertex from which the edge is originating
					int in = P[k].se;//the index of the edge in the adjacency list of u
					if (R_G[u].adj[in].wr < min_w)
						min_w = R_G[u].adj[in].wr;
				}
				for (int k = 0; k < P.size(); k++)
				{
					int u = P[k].fi;//the vertex from which the edge is originating
					int in = P[k].se;//the index of the edge in the adjacency list of u
					R_G[u].adj[in].wr -= min_w;

					if (R_G[u].adj[in].wr == 0)
						R_G[u].ptr++;

					int j = R_G[u].adj[in].pos;

					if (R_G[u].adj[in].fw)
						G[u].adj[j].f += min_w;
					else
						G[u].adj[j].f -= min_w;
				}
			}
		}
	}
	int f = 0;
	for (flowedge &z : G[0].adj)
		f += z.f;
	return f;
}



int main()
{
	cin >> N;
	int ans = 0;
	
	vector<vector<int>>Vn;
	Vn.resize(52);
	for (int i = 0; i < 52; i++)
		Vn[i].resize(52, 0);

	vector<bool>y(52, false);
	for (int i = 0; i < N; i++)
	{
		char ai, bi;
		int fli;
		cin >> ai >> bi >> fli;
		int u, v;
		if (ai - 'A' >= 0 && ai - 'A' <= 25)
			u = ai - 'A';
		else
			u = ai - 'a' + 26;
		y[u] = true;
		
		if (bi - 'A' >= 0 && bi - 'A' <= 25)
			v = bi - 'A';
		else
			v = bi - 'a' + 26;
		y[v] = true;

		Vn[u][v] += fli;
	}

	if (y[0] && y[25])
	{
		vector<orig_node>G(52);
		for (int i = 0; i < 52; i++)
		{
			for (int j = i + 1; j < 52; j++)
			{
				if (Vn[i][j] != 0)
					G[i].adj.emplace_back(j, Vn[i][j]);
			}
		}

		for (int i = 0; i < 52; i++)
		{
			for (int j = 0; j < i; j++)
			{
				if (Vn[i][j] != 0)
				{
					G.emplace_back();
					G[G.size() - 1].adj.emplace_back(j, Vn[i][j]);
					G[i].adj.emplace_back(G.size() - 1, Vn[i][j]);
				}
			}
		}
		ans = Dinics(G);
	}

	cout << ans << endl;
	return 0;
}
