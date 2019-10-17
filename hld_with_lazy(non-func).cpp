// heavy_light_lazy.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <functional>
#include <fstream>
#include <stack>

#define fi first
#define se second
#define M 1000000007


using namespace std;
typedef long long ll;

greater<int>g = greater<int>();
less<int>l = less<int>();

typedef greater<int>gre;
typedef less<int>les;

struct node {
	bool v;
	vector<int>adj;
	int par = 0;
	int w = 0;//weight of the node
	int dep = 0;

	int d = 0;
	int f = 0;
	int s_ind = -1;//spl. child index in the adj.list
	int c_no = -1;//chain no.
	int c_pos = -1;//posn in the chain
	int c_h = 0;//head node in the chain
};


int j;

struct seg_node {
	int L;
	int R;
	int left;
	int right;
	int par = -1;
	ll rval;//range-value
	int ind;//index at which rval occurs in the range

	seg_node(int l, int r) :L(l), R(r) {}
};

template<typename Ty>
struct seg_tree {
	int N = 0;//number of elements on the input array
	vector<seg_node>T;
	Ty F;

	seg_tree(vector<int>&A, Ty &f) :N(A.size()), F(f)

	{
		int siz = 0;
		queue<int>Q;
		T.emplace_back(0, N - 1);
		Q.push(0);
		while (!Q.empty())
		{
			int c = Q.front();
			Q.pop();
			int l = T[c].L;
			int r = T[c].R;

			if (l == r)
			{
				T[c].ind = l;
				T[c].rval = A[l];
			}
			else
			{
				T.emplace_back(l, (l + r) / 2);
				siz++;
				T[c].left = siz;
				T[siz].par = c;
				Q.push(siz);

				T.emplace_back(((l + r) / 2) + 1, r);
				siz++;
				T[c].right = siz;
				T[siz].par = c;
				Q.push(siz);
			}
		}

		for (int i = siz; i >= 0; i--)
		{

			if (T[i].L != T[i].R)

			{

				T[i].rval = F(T[T[i].left].rval, T[T[i].right].rval) ? (T[T[i].left].rval) : (T[T[i].right].rval);

				T[i].ind = F(T[T[i].left].rval, T[T[i].right].rval) ? (T[T[i].left].ind) : (T[T[i].right].ind);
			}
		}
	}

	void update(int ind, int val)

	{
		int c = 0;
		while (T[c].L != T[c].R)
		{
			if (ind <= (T[c].L + T[c].R) / 2)
				c = T[c].left;
			else
				c = T[c].right;
		}

		T[c].rval = val;
		T[c].ind = ind;
		c = T[c].par;
		while (c != -1)
		{
			T[c].rval = F(T[T[c].left].rval, T[T[c].right].rval) ? (T[T[c].left].rval) : (T[T[c].right].rval);

			T[c].ind = F(T[T[c].left].rval, T[T[c].right].rval) ? (T[T[c].left].ind) : (T[T[c].right].ind);

			c = T[c].par;
		}
	}

	pair<int, int>ran_val(int c, int l, int r)
	{
		pair<int, int>ret;
		if (l <= T[c].L && T[c].R <= r)
			ret = make_pair(T[c].ind, T[c].rval);

		else if (l <= T[c].R && T[c].L <= r)
		{
			pair<int, int>p1 = ran_val(T[c].left, l, r);
			pair<int, int>p2 = ran_val(T[c].right, l, r);
			if (p2.fi == -1)
				ret = p1;

			else if (p1.fi == -1)
				ret = p2;

			else if (F(p1.se, p2.se))
				ret = p1;

			else
				ret = p2;
		}

		else
			ret = make_pair(-1, -1);

		return ret;
	}
};

struct seg_add_x {

	int N;
	vector<seg_node>T;

	vector<ll>a; //Will be used for.....
	vector<ll>d; //.....lazy updations(left->right)

	seg_add_x(vector<ll>&A) :N(A.size())
	{
		{
			int siz = 0;
			queue<int>Q;
			T.emplace_back(0, N - 1);
			Q.push(0);
			while (!Q.empty())
			{
				int c = Q.front();
				Q.pop();
				int l = T[c].L;
				int r = T[c].R;

				if (l == r)
				{
					T[c].ind = l;
					T[c].rval = A[l];
				}
				else
				{
					T.emplace_back(l, (l + r) / 2);
					siz++;
					T[c].left = siz;
					T[siz].par = c;
					Q.push(siz);

					T.emplace_back(((l + r) / 2) + 1, r);
					siz++;
					T[c].right = siz;
					T[siz].par = c;
					Q.push(siz);
				}

			}
			a.resize(siz + 1, 0);
			d.resize(siz + 1, 0);
			for (int i = siz; i >= 0; i--)

			{
				if (T[i].L != T[i].R)
					T[i].rval = T[T[i].left].rval + T[T[i].right].rval;
			}
		}
	}


	void update(int c, int l, int r, int a_in,int x)//for range_update in the manner as described in problem
	{
		if (x < 0)
			x = x + M;

		int len = (T[c].R - T[c].L) + 1;

		if (l <= T[c].L && T[c].R <= r)
		{
			a[c] = (a[c] + a_in + ((T[c].L - l + 1)*x) % M) % M;
			d[c] = (d[c] + x) % M;
		}

		ll t = len*(2 * a[c] + (len - 1)*d[c]);
		t = t / 2;
		T[c].rval = (T[c].rval + t) % M;

		int le = T[c].left;
		int ri = T[c].right;

		if (T[c].L != T[c].R)
		{
			a[le] = (a[le] + a[c]) % M;
			d[le] = (d[le] + d[c]) % M;
			a[ri] = (a[ri] + a[c] + d[c] * ((-1 + len) / 2)) % M;
			d[ri] = (d[ri] + d[c]) % M;
		}
		a[c] = 0;
		d[c] = 0;

		if ((r>=l) && !(l <= T[c].L && T[c].R <= r))
		{
			if (l <= T[c].R && T[c].L <= r)
			{
				update(le, l, r, a_in, x);
				update(ri, l, r, a_in, x);

				T[c].rval = (T[le].rval + T[ri].rval) % M;
			}
		}
		return;
	}

	ll ran_val(int c, int l, int r)
	{
		ll ret = 0;
		int len = (T[c].R - T[c].L) + 1;

		ll t = len*(2 * a[c] + (len - 1)*d[c]);
		t = t / 2;
		T[c].rval = (T[c].rval + t) % M;
		
		int le = T[c].left;
		int ri = T[c].right;
		
		if (T[c].L != T[c].R)
		{
			a[le] = (a[le] + a[c]) % M;
			d[le] = (d[le] + d[c]) % M;
			a[ri] = (a[ri] + a[c] + d[c] * ((-1 + len) / 2)) % M;
			d[ri] = (d[ri] + d[c]) % M;
		}

		a[c] = 0;
		d[c] = 0;

		if (r < l)
			ret = 0;
		else if (l <= T[c].L && T[c].R <= r)
			ret = T[c].rval;

		else if (T[c].L <= r && l <= T[c].R)
			ret = (ran_val(le, l, r) + ran_val(ri, l, r)) % M;

		return ret;
	}

};

vector<node>G;
vector<int>ind;
vector<int>Z;
int T;

int dfs_visit(int i)

{
	j = i;
	G[i].v = true;
	T++;
	G[i].d = T;
	int s = 0;
	s++;
	int sm = -1;
	int sc = -1;
	int ind = -1;
	Z.push_back(i);
	for (int z : G[i].adj)
	{
		ind++;
		if (!G[z].v)
		{
			G[z].par = i;
			G[z].dep = G[i].dep + 1;
			int t = dfs_visit(z);
			T++;
			Z.push_back(i);
			s += t;
			if (t>sm)
			{
				sm = t;
				sc = ind;
			}
		}
	}
	G[i].f = T;
	G[i].s_ind = sc;
	return s;
}

void dfs()
{
	T = -1;
	int N = G.size();
	for (int i = 0; i<N; i++)
	{
		if (!G[i].v)
			dfs_visit(i);
	}
}



int cc = 0;
vector<vector<ll>>A;

void hld_visit(int i)
{
	G[i].v = true;
	G[i].c_no = cc;
	if (A.size() == cc)
	{
		A.emplace_back();
		G[i].c_h = i;
	}


	G[i].c_pos = A[cc].size();
	A[cc].push_back(G[i].w);

	if (G[i].s_ind != -1)
	{
		G[G[i].adj[G[i].s_ind]].c_h = G[i].c_h;
		hld_visit(G[i].adj[G[i].s_ind]);
	}

	for (int z : G[i].adj)
	{
		if (!G[z].v)
		{
			cc++;
			hld_visit(z);
		}
	}
}

void hld()//Heavy light decomposition
{
	for (auto &z : G)
		z.v = false;

	for (int i = 0; i<G.size(); i++)
	{
		if (!G[i].v)
			hld_visit(i);
	}
}



int LCA(int u, int v, seg_tree<les> &lc)//Lowest common ancestor O(log(n)) method

{
	pair<int, int>p;
	int Lca = -1;
	if ((G[u].d <= G[v].d) && (G[v].f <= G[u].f))
		Lca = u;

	else if ((G[v].d < G[u].d) && (G[u].f < G[v].f))
		Lca = v;

	else
	{
		if (G[u].f<G[v].d)
			p = lc.ran_val(0, G[u].f, G[v].d);

		else
			p = lc.ran_val(0, G[v].f, G[u].d);

		Lca = ind[p.fi];//p.f never <0 for valid range
	}
	return Lca;
}


int main()

{
	int t = 1;
	vector<int>ans;
	/*ifstream inp;
	ifstream inp2;
	ofstream out;
	inp.open("input.txt");
	inp2.open("output.txt");

	out.open("out.txt");
	out << "Start" << endl;*/
	for (int k = 0; k<t; k++)
	{
		int N, Q;

		G.clear();
		A.clear();
		ind.clear();
		Z.clear();

		cin >> N >> Q;
		//inp >> N >> Q;
		int a, b;
		G.resize(N);
		vector<seg_add_x>V;//segment tree created for every chain

		for (int i = 0; i<N - 1; i++)
		{
			cin >> a >> b;
			//inp >> a >> b;
			G[a].adj.push_back(b);

			G[b].adj.push_back(a);
		}



		//vector<int>Z;//array of vertex for lca comp.
		dfs();
		vector<int>dep;
		for (int z : Z)
		{
			dep.push_back(G[z].dep);
			ind.push_back(z);
		}

		seg_tree<les>lca(dep, l);

		hld();

		for (auto &z : A)
			V.emplace_back(z);

		for (int q = 0; q < Q; q++)
		{
			int s;
			cin >> s;
			//  inp >> s;
			if (s == 1)
			{
				int u, v, x;
				cin >> u >> v >> x;
				//inp >> i >> ti;
				int L = LCA(u, v, lca);

				int pu = u;
				int pv = v;

				int a = 0;
				while (G[pu].c_no != G[L].c_no)
				{
					a = (a + ((G[pu].c_pos + 1)*x) % M) % M;
					V[G[pu].c_no].update(0, 0, G[pu].c_pos, (a+x)%M, -x);
					pu = G[G[pu].c_h].par;
				}

				stack<pair<int, int>>S;
				while (G[pv].c_no != G[L].c_no)
				{
					S.push(make_pair(G[pv].c_no, G[pv].c_pos));
					pv = G[G[pv].c_h].par;
				}

				if (G[pu].c_pos <= G[pv].c_pos)
				{
					V[G[L].c_no].update(0, G[pu].c_pos, G[pv].c_pos, a, x);
					a = (a + (G[pv].c_pos - G[pu].c_pos + 1)*x) % M;
				}

				else
				{
					a = (a + (G[pu].c_pos - G[pv].c_pos + 1)*x) % M;
					V[G[L].c_no].update(0, G[pv].c_pos, G[pu].c_pos, (a+x)%M, -x);
				}

				while (!S.empty())
				{
					pair<int, int>temp = S.top();
					S.pop();
					int cno = temp.fi;
					int cpos = temp.se;
					V[cno].update(0, 0, cpos, a, x);
					a = (a + (cpos + 1)*x) % M;
				}
			}
			else if (s == 2)
			{
				int a, b;
				cin >> a >> b;
				//  inp >> a >> b;
				int L = LCA(a, b, lca);

				int pa = a;
				int pb = b;
				ll sum = 0;
				while (G[pa].c_no != G[L].c_no)
				{
					ll temp = V[G[pa].c_no].ran_val(0, 0, G[pa].c_pos);
					sum = (sum + temp) % M;
					pa = G[G[pa].c_h].par;
				}

				while (G[pb].c_no != G[L].c_no)
				{
					ll temp = V[G[pb].c_no].ran_val(0, 0, G[pb].c_pos);
					sum = (sum + temp) % M;
					pb = G[G[pb].c_h].par;
				}

				int lpos = G[pa].c_pos<G[pb].c_pos ? G[pa].c_pos : G[pb].c_pos;

				int hpos = G[pa].c_pos>G[pb].c_pos ? G[pa].c_pos : G[pb].c_pos;

				ll temp = V[G[pa].c_no].ran_val(0, lpos, hpos);

				sum = (sum + temp) % M;
				/*int ap;
				inp2 >> ap;
				if (ap != m)
				out << "query: " << q << " compute ans: " << m << " actual ans: " << ap << endl;*/
				
				ans.push_back(sum);
			}
			else
				break;
			}
		}

		/*inp.close();

		inp2.close();

		out.close();*/
		for (int z : ans)
			cout << z << endl;

		return 0;
	}
}
