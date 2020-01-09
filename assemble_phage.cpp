// Assemble_reads.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <functional>
using namespace std;

int V_num;
int s_len = 0;
vector<string>reads;

vector<unsigned int>next_ver;//next vertex in the path for a veterx

struct inci {
	bool in_d = false;//set true when inward incidence is there
	bool out_d = false;////set true when outward incidence is there
};

struct Union{
	struct node{
		int par;
		int rank = 0;
	};
	
	vector<node>DS;//Disjoint sets
	int num;//Number of disjoint sets in the Union data structure
	Union(int N):num(N)
	{
		DS.resize(N);
		for(int i=0;i<N;i++)
			DS[i].par = i;
	}
	
	int Find(int i)//parent of the tree in the forest found
	{
		if(DS[i].par == i)
			return i;
		
		DS[i].par = Find(DS[i].par);
		return DS[i].par;
	}
	
	void Merge(int i,int j)//Merge components containing node[i] and node[j]
	{
		int fi = Find(i);
		int fj = Find(j);
		if(fi != fj)
		{
			if(DS[fi].rank < DS[fj].rank)
				swap(fi,fj);
			
			DS[fj].par = fi;
			if(DS[fi].rank == DS[fj].rank)
				DS[fi].rank++;
			num--;
		}
	}
};
	

struct edge {
	edge() {}
	edge(int u1, int v1, int w) :u(u1), v(v1), weight(w) {}
	edge(const edge &r) :u(r.u), v(r.v), weight(r.weight) {}
	int u = 0;//starting vertex of the edge
	int v = 0;//terminal vertex of the edge
	int weight = 0;

	bool operator>=(const edge &rhs)const
	{
		return weight >= rhs.weight;
	}

	bool operator<=(const edge &rhs)const
	{
		return weight <= rhs.weight;
	}

	bool operator>(const edge &rhs)const
	{
		return weight > rhs.weight;
	}

	bool operator<(const edge &rhs)const
	{
		return weight < rhs.weight;
	}
};

vector<edge>edg_sor;
vector<int>ps;//path-sequence


unsigned int Compute_Overlap(const string &s1, const string &s2)
/*Length of max possible prefix of s1 which is also suffix of s2*/
{
	string s = s1 + "$" + s2;
	unsigned int len = s.size();
	vector<unsigned int>Pre_func(len, 0);
	Pre_func[0] = 0;
	for (unsigned int i = 1; i < len; i++)
	{
		unsigned int border = Pre_func[i - 1];
		while (border > 0 && s[border] != s[i])
			border = Pre_func[border - 1];
		if (s[border] == s[i])
			border++;
		Pre_func[i] = border;
	}
	return Pre_func[len - 1];
}

void Create_adj_mat(vector<vector<int>>&OG, vector<pair<int, vector<int>>>&V12_out, vector<pair<int, vector<int>>>&V12_in)
{
	for (int i = 0; i < V12_out.size(); i++)
	{
		for (int j = 0; j < V12_in.size(); j++)
		{
			int temp;
			temp = Compute_Overlap(reads[V12_in[j].first], reads[V12_out[i].first]);
			OG[V12_out[i].first][V12_in[j].first] = temp;
			if (temp < 12)
			{
				for (int z1 : V12_out[i].second)
				{
					for (int z2 : V12_in[j].second)
						OG[z1][z2] = temp;
				}
			}
		}
	}
	
	for (int i = 0; i < reads.size(); i++)
	{
		for (int j = 0; j < reads.size(); j++)
		{
			if (i != j && OG[i][j] == -1)
				OG[i][j] = Compute_Overlap(reads[j], reads[i]);
		}
	}
}

void Sort_edges_weight(vector<vector<int>>&OG)
{
	for (int i = 0; i < V_num; i++)
	{
		for (int j = 0; j < V_num; j++)
		{
			if(i!=j)
			edg_sor.emplace_back(i, j, OG[i][j]);
		}
	}
	sort(edg_sor.begin(), edg_sor.end());
}

void Compute_path()
{
	next_ver.resize(V_num,-1);
	int edges = 0;
	vector<inci>flag(V_num);
	
	Union D(V_num);
	
	for (int i = edg_sor.size() - 1; i >= 0 && edges < V_num-1; i--)
	{
		int u = edg_sor[i].u;
		int v = edg_sor[i].v;
		if (!(flag[u].out_d) && !(flag[v].in_d) && (next_ver[v]!=u) && (D.Find(u)!=D.Find(v)))
		{
			flag[u].out_d = true;
			flag[v].in_d = true;
			next_ver[u] = v;
			D.Merge(u,v);
			edges++;
		}
	}
	int start = 0;
	for(int i=0;i<V_num;i++)
	{
		if(!flag[i].in_d)
		{
			start = i;
			break;
		}
	}
	ps.push_back(start);
	int next = next_ver[start];
	
	while(next != -1)
	{
		ps.push_back(next);
		next = next_ver[next];
	}	
	ps.push_back(start);
}

void Print_path(vector<vector<int>>&OG)
{
	for(int i=0;i<ps.size()-1;i++)
	{
		int cur = ps[i];
		int nxt = ps[i+1];
		int ov = OG[cur][nxt];//overlap with the next read
		cout<<reads[cur].substr(0,s_len-ov);
	}
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
	s_len = reads[0].size();

	vector<vector<int>>OG(V_num);//Overlap graph
	for (auto &z : OG)
		z.resize(V_num, -1);//initialized to dummy values (-1)

	vector<pair<int,vector<int>>>V12_out;//equivalence classes based on 12-mer shared at the suffix
	vector<pair<int,vector<int>>>V12_in;//equivalence classes based on 12-mer shared at the prefix

	vector<bool>V_m(V_num, false);
	for (int i = 0; i < V_num; i++)
	{
		if (!V_m[i])
		{
			V12_out.push_back(make_pair(i, vector<int>()));
			V12_out.back().second.push_back(i);
			V_m[i] = true;
			for (int j = 0; j < V_num; j++)
			{
				if (!V_m[j] && s_len>12)
				{
					if (reads[i].substr(s_len - 12) == reads[j].substr(s_len - 12))
					{
						V12_out.back().second.push_back(j);
						V_m[j] = true;
					}
				}
			}
		}
	}

	for (int i = 0; i < V_m.size(); i++)
		V_m[i] = false;

	for (int i = 0; i < V_num; i++)
	{
		if (!V_m[i] && s_len>12)
		{
			V12_in.push_back(make_pair(i, vector<int>()));
			V12_in.back().second.push_back(i);
			V_m[i] = true;
			for (int j = 0; j < V_num; j++)
			{
				if (!V_m[j])
				{
					if (reads[i].substr(0,12) == reads[j].substr(0,12))
					{
						V12_in.back().second.push_back(j);
						V_m[j] = true;
					}
				}
			}
		}
	}

	Create_adj_mat(OG,V12_out,V12_in);
	Sort_edges_weight(OG);
	Compute_path();
	Print_path(OG);

    return 0;
}
