//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <sstream>
#include <map>
#include <cctype>
#include <algorithm>

struct not_digit {
	bool operator()(const char c) {
		return c != ' ' && !std::isdigit(c);
	}
};

using namespace std;
struct node {
	node() {}//only constructor which is default

	vector<int> adj_l = vector<int>();
	int in_d = 0;//in-degree(this will be updated during euler path/cycle construction

	int out_d = 0;//out-degree(this will be updated during euler path/cycle construction)
};

struct e_p_node {
	e_p_node() {}
	e_p_node(int v, e_p_node *p = NULL, e_p_node *n = NULL) :V(v), prev(p), nxt(n) {}
	int V = 0;//Vertex number 
	e_p_node *prev = NULL;//previous node in the euler path
	e_p_node *nxt = NULL;//next node in the euler path
};

int num_V = 0;//number of vertices in the graph
int num_e = 0;//number of edges in the Graph

int edg_le;//edg_le -> number of edges left

vector<node>G;//Graph will be in the form of Map(since graph nodes are not in proper format)

e_p_node *eul;//pointer to the starting node in the eulerian cycle, this list will contain edges(in the form of sequence of vertices)

int num_V_out = 0;//number of Vertices with non zero out degree in the Graph


void euler_cycle();

int main()
{
	cin >> num_V >> num_e;
	G.resize(num_V + 1);//with one dummy node
	for (int i = 0; i < num_e; i++)
	{
		int u, v;
		cin >> u >> v;
		G[u].adj_l.push_back(v);
		G[u].out_d++;
		G[v].in_d++;
	}
	edg_le = num_e;
	bool eul_cycle_exists = true;
	for (node &z : G)
		eul_cycle_exists = eul_cycle_exists && (z.out_d == z.in_d);
	if (!eul_cycle_exists)
		cout << 0<<endl;
	else
	{
        cout<<1<<endl;
		euler_cycle();
		e_p_node *cur = eul;
		while (cur->nxt != NULL)
		{
			cout << cur->V << " ";
			cur = cur->nxt;
		}
		//cout << cur->V << endl;
	}
	return 0;
}

pair<e_p_node*,e_p_node*> start_cycle(int V_st)//starts a cycle with vertex no.V_st as the starting point	
{
	pair<e_p_node*, e_p_node*>ret;
	e_p_node *cur = new e_p_node(G[V_st].adj_l.back());
	G[V_st].adj_l.pop_back();
	G[V_st].out_d--;
	edg_le--;
	ret.first = cur;
	while (G[cur->V].out_d != 0)/*out_d = 0 can only happen when cur->V == V_st
							  since Euler Theorem conditions for Euler Cycle are assumed*/
	{
		int V_cur = cur->V;
		G[V_cur].out_d--;
		int z = G[V_cur].adj_l.back();
		G[z].in_d--;
		G[V_cur].adj_l.pop_back();
		cur->nxt = new e_p_node(z);
		cur->nxt->prev = cur;
		edg_le--;
		cur = cur->nxt;
		V_cur = z;
	}
	ret.second = cur;
	return ret; //A cycle has been constructed
}


void euler_cycle()
{
	pair<e_p_node*, e_p_node*>P;
	int V_cur = 1;
	e_p_node *cur = new e_p_node(V_cur);
	eul = cur;
	P = start_cycle(V_cur);
	cur->nxt = P.first;
	P.first->prev = eul;
	cur = P.second;

	while (edg_le != 0)
	{
		while (G[cur->V].out_d == 0)
			cur = cur->prev;
		V_cur = cur->V;
		P = start_cycle(V_cur);
		e_p_node *temp = cur->nxt;
		cur->nxt = P.first;
		P.first->prev = cur;
		P.second->nxt = temp;
		temp->prev = P.second;
		cur = P.second;
	}

	return;
}
