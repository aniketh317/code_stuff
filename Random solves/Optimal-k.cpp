// Selecting_Optimal-k.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

vector<string>reads;
int V_num;
int r_len;//length of reads

struct cust {
	bool in = false;
	vector<int>ind;//vector of indices
};

struct node {
	vector<pair<int,int>>adj;/*list of nodes to which there is an outgoing edge
							   along with multiplicity of the edge */
	bool v = false;
};


void radix_sort(vector<string>&r, vector<int>&look, int k)
{
	for (int rad = k - 1; rad >= 0; rad--)
	{
		vector<int>Cf(4);//A->0,C->1,G->2,T->3
		vector<int>temp(look.size(), 0);
		for (int i = 0; i < r.size(); i++)
		{
			int s = -1;
			switch (r[i][rad])
			{
			case 'A':
				s = 0;
				break;

			case 'C':
				s = 1;
				break;

			case 'G':
				s = 2;
				break;

			case 'T':
				s = 3;
				break;
			}
			Cf[s]++;
		}

		for (int i = 1; i < 4; i++)
			Cf[i] += Cf[i - 1];

		for (int i = look.size() - 1; i >= 0; i--)
		{
			int s = -1;
			switch (r[look[i]][rad])
			{
			case 'A':
				s = 0;
				break;

			case 'C':
				s = 1;
				break;

			case 'G':
				s = 2;
				break;

			case 'T':
				s = 3;
				break;
			}
			temp[Cf[s] - 1] = look[i];
			Cf[s]--;
		}

		look = temp;
	}
	return;
}

void DFS_visit(vector<node>&G,int i)
{
	G[i].v = true;
	for (pair<int,int> z : G[i].adj)
	{
		if (!G[z.first].v)
			DFS_visit(G, z.first);
	}
}

bool Euler_cycle_check(vector<node>&G)
{
	int V_num = G.size();
	DFS_visit(G, 0);
	bool flag = true;
	for (int i = 0; i < V_num; i++)
		flag = flag && G[i].v;

	/*-----No need to check for Strong connectedness-----*/

	vector<int>in_d(V_num, 0);
	vector<int>out_d(V_num, 0);

	for (int i = 0; i < V_num; i++)
	{
		for (pair<int,int> z : G[i].adj)
		{
			out_d[i] += z.second;
			in_d[z.first] += z.second;
		}
	}

	for (int i = 0; i < V_num; i++)
		flag = flag && (out_d[i] == in_d[i]);


	return flag;
}


bool is_cycle_present(int k)//k chosen for k-mers
{
	if (k <= 2)
		return true;

	vector<string>k_s;
	for (int i = 0; i < reads.size(); i++)
	{
		for (int j = 0; j <= reads[i].size() - k; j++)
			k_s.push_back(reads[i].substr(j, k));
	}
	vector<int>ref_k(k_s.size());
	for (int i = 0; i < k_s.size(); i++)
		ref_k[i] = i;
	radix_sort(k_s, ref_k, k);

	vector<pair<string, int>>V;
	V.push_back(make_pair(k_s[ref_k[0]], 1));
	for (int i = 1; i<ref_k.size(); i++)
	{
		if (k_s[ref_k[i]] == k_s[ref_k[i - 1]])
			V.back().second++;
		else
			V.push_back(make_pair(k_s[ref_k[i]], 1));
	}

	k_s.clear();
	

	for (int i = 0; i<V.size(); i++)
	{
		k_s.push_back(V[i].first.substr(0, k - 1));
		k_s.push_back(V[i].first.substr(1, k - 1));
	}

	vector<int>ref_k_1(k_s.size());
	for (int i = 0; i < ref_k_1.size(); i++)
		ref_k_1[i] = i;

	radix_sort(k_s, ref_k_1, k - 1);
	vector<int>cla_ss(ref_k_1.size());//lookup for k-1 mers(to be used for DeBruijn graph construction)
	
	int s = 0;
	cla_ss[ref_k_1[0]] = s;

	int i_s = 0;
	for (int i = 1; i < ref_k_1.size();)
	{
		while ((i < k_s.size()) && (k_s[ref_k_1[i]] == k_s[ref_k_1[i_s]]))
		{
			cla_ss[ref_k_1[i]] = s;
			i++;
		}

		if (i < k_s.size())
		{
			s++;
			cla_ss[ref_k_1[i]] = s;
		}
			
		
		i_s = i;
		i++;
	}

	/*----Preprocessing for Debruijn Graph construction complete----*/

	
	/*-----------Debruijn Graph construction started-------------*/
	vector<node>G(s + 1);
	for (int i = 0; i < V.size(); i++)
		G[cla_ss[i << 1]].adj.push_back(make_pair(cla_ss[(i << 1) + 1], 1));

	/*-----------Debruijn Graph has been constructed-------------*/

	bool flag = Euler_cycle_check(G);
	return flag;
}

int main()
{
	string dum;
	while (getline(cin, dum))
	{
		if (dum.size() != 0)
			reads.push_back(dum);
		else
			break;
	}

	V_num = reads.size();
	int l = 0;
	int h = reads[0].size();

	int mid;
	while (l < h)
	{
		mid = (l + h + 1) / 2;
		bool ret = is_cycle_present(mid);
		if (ret)
			l = mid;
		else
			h = mid - 1;
	}
	cout << l << endl;
	return 0;
}
