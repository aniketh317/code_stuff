// String_similarity.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;


struct suffix_tree {

	struct suff_node {
		suff_node *sl = 0;//suffix link in case of internal node
		suff_node *par = 0;
		int start = -1;//start of the edge
		int end = -2;//end of the edge(not of concern in case the node is a leaf)
		int index;//leaf index(which index starting suffix does this leaf correspond to)
		bool leaf = false;//true->leaf,false->internal node
		vector<suff_node*>chi;

		int num_suf_chi = 0;//Number of leaves that can be reached from this node 
							//(note that each leaf corresponds to a suffix)

		suff_node() { chi.resize(27, 0); }//26 characters + '$' char
	};


	suff_node *root = 0;
	string T;
	
	vector<int>sa;//suffix array
	vector<int>i_sa;//invert suffix array
	vector<int>lcp;//longest common prefix wrt. index
	
	suffix_tree(string &s) :T(s + "$")
	{
		int e = -1;
		int ji = 0;//index from which 1st rule 3 starts in the previous phase(set to 0 for phase corresponding to index 0) 

		int sz = T.size();
		vector<short>ns(sz);
		for (int i = 0; i<sz; i++)
		{
			if (T[i] != '$')
				ns[i] = T[i] - 'a' + 1;
			else
				ns[i] = 0;
		}

		int gl = 0;//number of characters by which downward descent to be made is pending

		root = new suff_node();
		root->sl = root;
		root->par = root;
		suff_node *cur = root;
		for (int i = 0; i<sz; i++)
		{
			e++;

			short c = ns[i];
			suff_node *prev = 0;
			for (int j = ji; j <= i; j++)
			{
				while (gl != 0 && gl >((cur->leaf) ? e : cur->end) - cur->start + 1)//s[j:i-1]is already present in the implicit suffix tree
				{
					gl -= (((cur->leaf) ? e : cur->end) - cur->start + 1);
					cur = cur->chi[ns[i - gl]];
				}

				if (gl == ((cur->leaf) ? e : cur->end) - cur->start + 1)//in this case even the suffix link for this type-2 addition exists(already existing internal node)
				{
					if (prev != 0 && prev->sl == 0 && !(prev->leaf))
						prev->sl = cur;

					if (!cur->leaf && cur->chi[c] == 0)//rule-2
					{
						ji++;
						suff_node *n = new suff_node();
						n->par = cur;
						cur->chi[c] = n;
						n->start = i;
						n->leaf = true;
						n->index = j;
						n->par = cur;

						prev = cur;
					}

					else if (!cur->leaf)//rule-3
					{
						cur = cur->chi[c];

						gl = 1;
						break;
					}
					//if cur->leaf = true and gl==0 , rule-1 extension happens
				}

				else
				{
					if (ns[i] != ns[cur->start + gl])//rule-2
					{
						ji++;

						suff_node *n = new suff_node();
						suff_node *l = new suff_node();

						n->par = cur->par;
						cur->par = n;
						n->start = cur->start;
						n->end = n->start + gl - 1;
						cur->start = cur->start + gl;
						l->start = i;
						l->leaf = true;
						l->par = n;

						l->index = j;


						n->par->chi[ns[n->start]] = n;
						n->chi[ns[i]] = l;
						n->chi[ns[cur->start]] = cur;


						cur = n;

						if (prev != 0 && prev->sl == 0 && !(prev->leaf))
							prev->sl = cur;

						prev = cur;
					}
					else//rule-3
					{
						gl++;
						break;
					}
				}

				cur = cur->par;

				if (cur->sl == cur && j<i)
					gl--;

				cur = cur->sl;

				gl += ((cur->end - cur->start) + 1);
			}

			if (prev != 0 && prev->sl == 0 && !prev->leaf)
				prev->sl = root;

		}
	}
	
	void suffix_array(suff_node *r)
	{
		if(r->leaf)
			sa.push_back(r->index);
		
		else
		{
			for(int i=0;i<=26;i++)
			{
				if(r->chi[i]!=0)
					suffix_array(r->chi[i]);
			}
		}
		
		if(r==root)
		{
			i_sa.resize(sa.size());
			for(int i=0;i<sa.size();i++)
				i_sa[sa[i]] = i;
		}
	}
	
	void LCP()
	{
		lcp.resize(T.size()-1);
		int lc = 0;
		for(int i=0;i<T.size();i++)
		{
			int i_inv = i_sa[i];//posn. of suffix in suffix array
			int i_1 = -2;
			if(i_inv != sa.size()-1 )
				i_1 = sa[i_inv+1];
			
			if(i_1>=0)
			{
				while((i+lc < T.size()) && T[i+lc] == T[i_1+lc])
					lc++;
				
				lcp[i_inv] = lc;
			}
			
			else
				lc = 0;
			
			if(lc!=0)
				lc--;	
		}
	}
	
};
