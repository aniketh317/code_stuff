// Bubble-detection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

using namespace std;

vector<string>reads;
int V_num;
int r_len;//length of reads
int k, t;//k-mers to be generated from reads, t-> bubble threshold


struct node {
	vector<pair<int, int>>adj;/*list of nodes to which there is an outgoing edge
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

vector<node>G;//DeBruijn graph to be constructed

void DeBruijn_Construct()
/*---Generate DeBruijn Graph out of k-mers generated fromm the given set of reads----*/
{
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
	
	G.resize(s + 1);

	for (int i = 0; i < V.size(); i++)
		G[cla_ss[i << 1]].adj.push_back(make_pair(cla_ss[(i << 1) + 1], 1));

	/*-----------Debruijn Graph has been constructed-------------*/
	
	return;
}

vector<bool>visit;
vector<bool>mark;
vector<int>d;

void path_extend(int i)
{
	for (pair<int, int>z : G[i].adj)
	{
		if (!visit[z.first])
		{
			visit[z.first] = true;
			d[z.first] = d[i] + 1;
			path_extend(z.first);
			visit[z.first] = false;
		}

		else if (visit[z.first])
		{
			if (d[z.first] <= t || d[i] + 1 <= t)
			{
				mark[z.first] = true;
			}
				
		}
	}
}

int count_bubbles()
/*----  t:Bubble threshold  ----*/
{
	visit.resize(G.size(), false);
	mark.resize(G.size(), false);
	d.resize(G.size(), -1);

	int bubble_count = 0;
	for (int u = 0; u < G.size(); u++)
	{
		for (int i = 0; i < G.size(); i++)
		{
			visit[i] = false;
			mark[i] = false;
			d[i] = -1;
		}
		
		for (pair<int, int>z : G[u].adj)
		{
			if (!visit[z.first])
			{
				d[z.first] = 1;
				visit[z.first] = true;
				path_extend(z.first);
			}
		}

		for (int i = 0; i < mark.size(); i++)
			bubble_count += (mark[i] ? 1 : 0);
	}
}

int main()
{
	cin >> k >> t;
	string dum;
	while (getline(cin, dum))
	{
		if (dum.size() != 0)
			reads.push_back(dum);
		else
			break;
	}

	V_num = reads.size();

	DeBruijn_Construct();
	int ans = count_bubbles();

	cout << ans << endl;
    return 0;
}

