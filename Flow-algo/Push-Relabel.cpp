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

	res_adj & operator=(const res_adj &rhs)
	{
		ind = rhs.ind;
		Cf = rhs.Cf;
		return *this;
	}

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
			V[i].H.resize(2 * n);
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
						V[i].H[n].emplace_back(j, C[i][j]);//height[source] == n
						V[i].pos[j] = V[i].H[n].size() - 1;
					}

					else
					{
						V[i].H[0].emplace_back(j, C[i][j]);//height[j] == 0 for j!=source
						V[i].pos[j] = V[i].H[0].size() - 1;
					}
				}
			}
		}
	}


};

int max_flow(vector<vector<int>>&C, int source, int sink, vector<vector<int>>&F)
//Capacity graph in the form of adjacency matrix
//F will have preflows at every stage and finally max-flow at termination of the while loop
{
	int n = C.size();//number of vertices

	vector<vector<int>>h(2 * n);/*Stores vertices with positive excess(based on their heights)
									maximum possible height is 2*n-1 */

	residual_graph RG(C, source, sink);//Residual graph of size n
	
	int num_exc = 0;//number of nodes with +ve excess
	for (int i = RG.V[source].H[0].size() - 1; i >= 0; i--)
	{
		res_adj &r = RG.V[source].H[0][i];
		int j = r.ind;
		int cf = r.Cf;

		RG.V[source].H[0].pop_back();
		RG.V[source].pos[j] = -1;

		F[source][j] += cf;

		RG.V[j].exc += cf;
		if (j != sink)
		{
			h[0].push_back(j);
			num_exc++;
		}

		RG.V[j].H[n].emplace_back(source, cf);
		RG.V[j].pos[source] = RG.V[j].H[n].size() - 1;	
	}
	vector<int>h_st;//stack of heights for which there are vertices with +ve excess with one
	//of these heights

	h_st.push_back(0);

	while (num_exc>0)
	{
		int max_exc_h = h_st.back();//maximum height among heights of vertices with +ve excess
		int siz = h[max_exc_h].size();
		int u = h[max_exc_h][siz - 1];
		int dh = max_exc_h - 1;//downhill height
		if (dh>=0 && RG.V[u].H[dh].size() != 0)
		{
			/*PUSH along an edge which has terminating node at height dh*/
	
			int cu_h = max_exc_h;//height of the current vertex
			res_adj &r = RG.V[u].H[dh].back();
			int d = min(r.Cf, RG.V[u].exc);

			r.Cf -= d;

			if (C[u][r.ind] > 0)
				F[u][r.ind] += d;
			else
				F[r.ind][u] -= d;

			RG.V[u].exc -= d;
			RG.V[r.ind].exc += d;

			if (RG.V[r.ind].exc == d && r.ind != sink && r.ind != source)
			{
				h[dh].push_back(r.ind);

				if (h[dh].size() == 1)
				{
					h_st.push_back(dh);
					int sz = h_st.size();
					swap(h_st[sz - 1], h_st[sz - 2]);
				}

				num_exc++;
			}

			if (RG.V[u].exc == 0)
			{
				h[cu_h].pop_back();
				num_exc--;
				if (h[cu_h].size() == 0)
					h_st.pop_back();
			}

			if (RG.V[r.ind].pos[u] == -1)
			{
				RG.V[r.ind].H[cu_h].emplace_back(u, d);
				RG.V[r.ind].pos[u] = RG.V[r.ind].H[cu_h].size() - 1;
			}
			else
				RG.V[r.ind].H[cu_h][RG.V[r.ind].pos[u]].Cf += d;

			if (r.Cf == 0)
			{
				RG.V[u].pos[r.ind] = -1;
				RG.V[u].H[dh].pop_back();
			}

		}

		else
		{
			/*RELABEL the node*/
			h_st.push_back(max_exc_h + 1);
			h[max_exc_h].pop_back();
			h[max_exc_h + 1].push_back(u);

			if (h[max_exc_h].empty())
			{
				int sz = h_st.size();
				h_st[sz - 2] = h_st[sz - 1];
				h_st.pop_back();
			}

			for (int i = 0; i<n; i++)
			{
				if (RG.V[i].pos[u] != -1)
				{
					res_adj &c = RG.V[i].H[max_exc_h][RG.V[i].pos[u]];
					RG.V[i].H[max_exc_h + 1].emplace_back(c.ind, c.Cf);

					c = RG.V[i].H[max_exc_h].back();

					RG.V[i].pos[c.ind] = RG.V[i].pos[u];
					RG.V[i].H[max_exc_h].pop_back();

					RG.V[i].pos[u] = RG.V[i].H[max_exc_h + 1].size() - 1;
				}
			}

		}
	}


	int flow_max = 0;
	for (int v = 0; v < n; v++)
		flow_max += (F[source][v] - F[v][source]);

	return flow_max;

}




int main()
{
	/*vector<vector<int>>C(6);
	for (auto &z : C)
		z.resize(6, 0);

	vector<vector<int>>F(6);
	for (auto &f : F)
		f.resize(6, 0);
	C[0][1] = 10;
	C[0][2] = 10;
	C[1][2] = 2;
	C[1][3] = 4;
	C[1][4] = 8;
	C[2][4] = 9;
	C[3][5] = 10;
	C[4][3] = 6;
	C[4][5] = 10;*/
	int N;
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

	vector<vector<int>>F;//Flow graph

	int siz = 52;
	vector<vector<int>>C;
	for (int i = 0; i < 52; i++)
	{
		for (int j = i + 1; j < 52; j++)
		{
			if (Vn[i][j] > 0 && Vn[j][i]>0)
				siz++;
		}
	}

	C.resize(siz);
	for (auto &z : C)
		z.resize(siz, 0);

	int nex_fil = 52;
	for (int i = 0; i < 52; i++)
	{
		for (int j = i + 1; j < 52; j++)
		{
			if (Vn[i][j] > 0 && Vn[j][i]>0)
			{
				C[j][nex_fil] = Vn[j][i];
				C[nex_fil][i] = Vn[j][i];
				Vn[j][i] = 0;
				nex_fil++;
			}
			C[i][j] = Vn[i][j];
			C[j][i] = Vn[j][i];
		}
	}

	F.resize(siz);
	for (auto &f : F)
		f.resize(siz, 0);

	if (y[0] && y[25])
		ans = max_flow(C, 0, 25, F);

	//int ans = max_flow(C, 0, 5, F);
	cout << ans << endl;
	return 0;
}
