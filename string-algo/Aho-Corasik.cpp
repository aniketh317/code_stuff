// 22.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>

typedef long long ll;
typedef unsigned int uint;
using namespace std;

vector<vector<char>>V;
/*-----------------------Aho-Corasik-----------------------*/

struct aho_node {
	vector<int>p_ind;//will contain indices of the patterns which are represented by this node
	vector<aho_node*>N;
	aho_node *SL = 0;//Suffix Link(largest proper suffix of this string which is prefix of atleast one pattern sn so can be obtained by some  trie traversal)
	aho_node *DL = 0;//pointer to a dictionary leaf which is the longest pattern that is a proper suffix of the string represented by this node                   
	aho_node() { N.resize(10, 0); }//To have 0-9 
};


void insert(aho_node *root, vector<char>&s, int pin)//pin is pattern index(inserting a pattern
//into pattern trie created so far
{
	uint s_iz = s.size();
	uint i = 0;
	aho_node *r = root;
	while (i < s_iz)
	{
		int k = s[i]-'0';
		if (r->N[k] == 0)
			r->N[k] = new aho_node();
		r = r->N[k];
		i++;
	}
	(r->p_ind).push_back(pin);
	return;
}



void calc_SL(aho_node *root)//calculate suffix links for all the node in the trie rooted at node root
							//Also calculates dict links
{
	root->SL = root;
	queue<aho_node*>Q;
	Q.push(root);
	while (!Q.empty())
	{
		aho_node *c = Q.front();
		Q.pop();
		for (int i = 0; i < 10; i++)
		{
			if (c->N[i] != 0) //no need for any visit checks(trie is a tree)
			{
				aho_node *S = c->SL;
				while (S != root && S->N[i] == 0)
					S = S->SL;

				if (S->N[i] != 0 && S->N[i] != c->N[i])
					c->N[i]->SL = S->N[i];
				else
					c->N[i]->SL = root;

				if (c->N[i]->SL->p_ind.size() != 0)
					c->N[i]->DL = c->N[i]->SL;
				else
					c->N[i]->DL = c->N[i]->SL->DL;
				Q.push(c->N[i]);
			}
		}
	}
}

ll trie_trav(aho_node *root, string &T)
{
	aho_node *r = root;
	uint s_iz = T.size();
	uint i = 0;
	ll sum = 0;
	while (i<s_iz)
	{
		int k = T[i] - '0';
		while (r != root && r->N[k] == 0)
			r = r->SL;
		if (r->N[k] != 0)
		{
			r = r->N[k];
			aho_node *c = r;
			while (c != 0)
			{
				sum += (c->p_ind).size();
				c = c->DL;
			}
		}

		i++;
	}
	return sum;
}
/*---------------------------------------------------------------------*/

void pre(aho_node* &root)
{
	V.resize(801);
	V[0].push_back('1');
	for (int i = 1; i <= 800; i++)
	{
		vector<char>&p = V[i - 1];
		int c = 0;
		for (int k = 0; k<p.size(); k++)
		{
			int z = p[k] - '0';
			z = (z << 1) + c;
			if (z - 10 >= 0)
			{
				V[i].push_back((z - 10) + '0');
				c = 1;
			}
			else
			{
				V[i].push_back(z + '0');
				c = 0;
			}
		}
		if (c != 0)
			V[i].push_back(c + '0');
	}

	for (int i = 0; i <= 800; i++)
		reverse(V[i].begin(), V[i].end());

	
	for (int i = 0; i <= 800; i++)
		insert(root, V[i], i);

	calc_SL(root);

	return;
}


int main()
{
	int T;
	cin >> T;
	vector<ll>ans(T);

	aho_node *root = new aho_node();
	
	pre(root);

	for (int t = 0; t < T; t++)
	{
		string s;
		cin >> s;
		ans[t] = trie_trav(root, s);
	}

	for (ll z : ans)
		cout << z << endl;

    return 0;
}
