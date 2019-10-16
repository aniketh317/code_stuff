
// Heavy-Light-decomposition.cpp : Defines the entry point for the console application.

//

 

#include "stdafx.h"

#include <iostream>

#include <vector>

#include <queue>

#include <map>

#include <string>

#include <functional>

#include <fstream>

 

#define fi first

#define se second

 

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

 

vector<node>G;

int j;

vector<int>Z;

template<typename Ty>

struct seg_tree {

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

 

vector<int>ind;

int T;

int dfs_visit(vector<node>&G, int i, vector<int>&Z)

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

                      int t = dfs_visit(G, z, Z);

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

 

 

void dfs(vector<node>&G, vector<int>&Z)

{

       T = -1;

       int N = G.size();

       for (int i = 0; i<N; i++)

       {

              if (!G[i].v)

                      dfs_visit(G, i, Z);

       }

}

 

int cc = 0;

vector<vector<int>>A;

 

void hld_visit(vector<node>&G, int i)

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

              hld_visit(G, G[i].adj[G[i].s_ind]);

       }

 

       for (int z : G[i].adj)

       {

              if (!G[z].v)

              {

                      cc++;

                      hld_visit(G, z);

              }

       }

}

 

void hld(vector<node>&G)//Heavy light decomposition

{

       for (auto &z : G)

              z.v = false;

       for (int i = 0; i<G.size(); i++)

       {

              if (!G[i].v)

                      hld_visit(G, i);

       }

}

 

int LCA(int u, int v, vector<node>&G, seg_tree<les> &lc)

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

       ifstream inp;

       ifstream inp2;

       ofstream out;

       inp.open("input.txt");

       inp2.open("output.txt");

       out.open("out.txt");

       out << "Start" << endl;

       for (int k = 0; k<t; k++)

       {

              int N, Q;

              A.clear();

              ind.clear();

              //cin >> N >> Q;

              inp >> N >> Q;

              int a, b, c;

              G.resize(N);

              vector<seg_tree<gre>>V;//segment tree created for every chain

 

              for (int i = 0; i<N - 1; i++)

              {

                      //cin >> a >> b;

                      inp >> a >> b;

                      G[a].adj.push_back(b);

                      G[b].adj.push_back(a);

              }

 

              //vector<int>Z;//array of vertex for lca comp.

 

              dfs(G, Z);

              vector<int>dep;

              for (int z : Z)

              {

                      dep.push_back(G[z].dep);

                      ind.push_back(z);

              }

 

              seg_tree<les>lca(dep, l);

 

              hld(G);

 

              for (auto &z : A)

                      V.emplace_back(z, g);

 

              for (int q = 0; q < Q; q++)

              {

                      int s;

                      //cin >> s;

                      inp >> s;

                      if (s == 1)

                      {

                             int i, ti;

                             //cin >> i >> ti;

                             inp >> i >> ti;

                             G[i].w = ti;

                             V[G[i].c_no].update(G[i].c_pos, ti);

                      }

 

                      else if (s == 2)

                      {

                             int a, b;

                             //cin >> a >> b;

                             inp >> a >> b;

                             int L = LCA(a, b, G, lca);

                             int pa = a;

                             int pb = b;

                             int ma = -1;

                             int mb = -1;

                             while (G[pa].c_no != G[L].c_no)

                             {

                                    pair<int, int> temp = V[G[pa].c_no].ran_val(0, 0, G[pa].c_pos);

                                    if (temp.se>ma)

                                           ma = temp.se;

                                    pa = G[G[pa].c_h].par;

                             }

 

                             while (G[pb].c_no != G[L].c_no)

                             {

                                    pair<int, int>temp = V[G[pb].c_no].ran_val(0, 0, G[pb].c_pos);

                                    if (temp.se>mb)

                                           mb = temp.se;

                                    pb = G[G[pb].c_h].par;

                             }

                             int m = (ma>mb) ? ma : mb;

 

                             int lpos = G[pa].c_pos<G[pb].c_pos ? G[pa].c_pos : G[pb].c_pos;

 

                             int hpos = G[pa].c_pos>G[pb].c_pos ? G[pa].c_pos : G[pb].c_pos;

 

                             pair<int, int>temp = V[G[pa].c_no].ran_val(0, lpos, hpos);

                             if (temp.se>m)

                                    m = temp.se;

 

                             int ap;

                             inp2 >> ap;

                             if (ap != m)

                                    out << "query: " << q << " compute ans: " << m << " actual ans: " << ap << endl;

                             ans.push_back(m);

                      }

 

                      else

                             break;

              }

 

       }

       inp.close();

       inp2.close();

       out.close();

       for (int z : ans)

              cout << z << endl;

 

       return 0;

}
