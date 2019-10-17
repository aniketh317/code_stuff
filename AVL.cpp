
//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <exception>

typedef long long ll;
using namespace std;

#define f first
#define s second



struct node
{
	int val;          //value
	node* left = 0;  //left child
	node* right = 0;//right child
	node *par = 0;  //parent of the node
	int ht = 0;     //height of the node
	int s_ize = 1;
};


struct cust{
	node *p = 0;
	node *q = 0;
	cust(){}
	cust(node *a,node *b):p(a),q(b){}
};

pair<bool,cust> check(node *root)//checks if pointers have been updated properly(can catch errors in many cases)
{
	pair<bool,cust>p = make_pair(true,cust());
	if(root == 0)
		return p;
	bool l,r;
	if(root->left == 0)
		l=true;
	else
		l = (root->left->par == root);
	if(!l)
		return make_pair(false,cust(root,root->left));
		
	if(root->right == 0)
		r = true;
	else
		r = (root->right->par == root);
	
	if(!r)
		return make_pair(false,cust(root,root->right));
	
	pair<bool,cust>lc = check(root->left);	
	 if(!lc.f)
	 	return lc;
	 	
	 pair<bool,cust>rc = check(root->right);
	 if(!rc.f)
	 	return rc;
	 return p;
}

int height(node *p)
{
	if (p == 0)
		return -1;
	return p->ht;
}

int s_ize(node *p)
{
	if (p == 0)
		return 0;
	return p->s_ize;
}

void os_node_update(node *N)
{
	if (N == 0)
		return;
	int l_h = height(N->left);
	int r_h = height(N->right);
	N->ht = 1 + ((l_h>r_h) ? l_h : r_h);
	int l_s = s_ize(N->left);
	int r_s = s_ize(N->right);
	N->s_ize = l_s + r_s + 1;
}

void L_rotate(node *N)
{
	node *R = N->right;
	node *P = N->par;

	N->par = R;
	R->par = P;
	N->right = R->left;
	
	if(R->left!=0)
		R->left->par = N;
		
	R->left = N;
	if (P != 0)
	{
		if (P->left == N)
			P->left = R;
		else
			P->right = R;
	}
}

void R_rotate(node *N)
{
	node *L = N->left;
	node *P = N->par;
	N->par = L;
	L->par = P;
	N->left = L->right;
	
	if(L->right!=0)
		L->right->par = N;
		
	L->right = N;
	if (P != 0)
	{
		if (P->left == N)
			P->left = L;
		else
			P->right = L;
	}
}

void L_Rebalance(node *N)
{
	node *R = N->right;
	node *RL = R->left;
	if (height(R->left) > height(R->right))
		R_rotate(R);
	L_rotate(N);
	os_node_update(N);
	os_node_update(R);
	os_node_update(RL);
	return;
}

void R_Rebalance(node *N)
{
	node *L = N->left;
	node *LR = L->right;
	if (height(L->right)>height(L->left))
		L_rotate(L);
	R_rotate(N);
	os_node_update(N);
	os_node_update(L);
	os_node_update(LR);
}

void Rebalance(node *N)
{
	if (N == 0)
		return;
	node *P = N->par;
	int l_h = height(N->left);
	int r_h = height(N->right);
	if (r_h>l_h + 1)
		L_Rebalance(N);
	
	else if (l_h>r_h + 1)
		R_Rebalance(N);
	else
		os_node_update(N);
	Rebalance(P);
}


node* insert(node *root, int n_val)
{
	node *n = new node();
	n->val = n_val;
	if (root == 0)
		return n;
	node *pre = 0;
	node *cur = root;
	while (cur != 0)
	{
		pre = cur;
		if (cur->val>n_val)
			cur = cur->left;
		else
			cur = cur->right;
	}
	n->par = pre;
	if (pre->val>n_val)
		pre->left = n;
	else
		pre->right = n;
	
	Rebalance(n);
	
	cur = n;
	pre = n->par;
	while (pre != 0)
	{
		cur = pre;
		pre = pre->par;
	}
	return cur;
}

pair<bool, node*> del_val(node *root, int v)
{
	pair<bool, node*>ret;
	node *pre, *cur = root;
	while (cur != 0 && cur->val != v)
	{
		if (cur->val < v)
			cur = cur->right;
		else
			cur = cur->left;
	}
	if (cur == 0)
	{
		ret.f = false;
		ret.s = root;
	}
	
	else if(s_ize(root) == 1)
	{
		ret.f = true;
		ret.s = 0;
	}
	
	else
	{
		node *P = 0, *L = 0;
		node *nr = cur->right;
		if (nr != 0)
		{
			node *p = 0;
			node *c = nr;
			while (c != 0)
			{
				p = c;
				c = c->left;
			}
			P = p->par;//in this case P cannot be 0
			if (P->right == p)
				P->right = p->right;
			else
				P->left = p->right;
			if (p->right != 0)
				p->right->par = P;
			cur->val = p->val;
		}
		else
		{
			P = cur->par;
			L = cur->left;
			if (P != 0)
			{
				if (P->left == cur)
				P->left = L;
				else
				P->right = L;
			}
			if (L != 0)
				L->par = P;
		}
		node *N;
		if (P != 0)
			N = P;
		else
			N = L;
		
		Rebalance(N);//P will have sufficed, L is balanced
		
		cur = N;
		pre = N->par;
		while (pre != 0)
		{
			cur = pre;
			pre = pre->par;
		}
		ret.f = true;
		ret.s = cur;
	}
	return ret;
}

node* merge(node *L, node *R)//L->Left tree root
// R->Right tree root
/*All keys in L are less than equal to all keys in R*/
{
	if (L == 0)
		return R;
	if (R == 0)
		return L;
	node *cur = 0;
	node *P = 0, *pre = 0, *pre2 = 0;
	int l = height(L);
	int r = height(R);
	
	if (l <= r)
	{
		cur = R;
		while (height(cur) >= l)
		{
			pre = cur;
			cur = cur->left;
		}
		if (cur == 0)
		{
			pre->left = L;
			L->par = pre;
			P = pre;
		}
		else
		{
			pre2 = 0;
			while (cur != 0)
			{
				pre2 = cur;
				cur = cur->left;
			}
			P = pre2->par;
			P->left = pre2->right;
			if (pre2->right != 0)
				pre2->right->par = P;
			
			pre2->left = L;
			pre2->right = pre;
			pre2->par = pre->par;
			if (pre2->par != 0)
				pre2->par->left = pre2;
			pre->par = pre2;
			L->par = pre2;
			
		}
	}

	else
	{
		cur = L;
		while (height(cur) >= r)
		{
			pre = cur;
			cur = cur->right;
		}
		if (cur == 0)
		{
			pre->right = R;
			R->par = pre;
			P = pre;
		}
		else
		{
			pre2 = 0;
			while (cur != 0)
			{
				pre2 = cur;
				cur = cur->right;
			}
			P = pre2->par;
			P->right = pre2->left;
			if (pre2->left != 0)
				pre2->left->par = P;
			
			pre2->right = R;
			pre2->left = pre;
			pre2->par = pre->par;
			if (pre2->par != 0)
				pre2->par->right = pre2;
			pre->par = pre2;
			R->par = pre2;
		}
	}

	Rebalance(P);
	
	cur = P;
	while (cur != 0)
	{
		pre = cur;
		cur = cur->par;
	}
	node *n_root = pre;
	return n_root;
}


pair<node *, node *>split(node *root, int v)
//split the tree about the
{
	node *L = 0;
	node * R = 0;
	
	if (root == 0)
		return make_pair(L, R);
	
	R = root->right;
	L = root->left;
	if (L != 0)
		L->par = 0;
	if (R != 0)
		R->par = 0;
	
	root->left = 0;
	root->right = 0;
	os_node_update(root);
	
	node *L_root = 0;
	node *R_root = 0;
	
	if (root->val>v)
	{
		pair<node*, node*>L_spl = split(L, v);
		R_root = merge(L_spl.s, root);
		R_root = merge(R_root, R);
		L_root = L_spl.f;
	}

	else
	{
		pair<node*, node*>R_spl = split(R, v);
		L_root = merge(root, R_spl.f);
		L_root = merge(L, L_root);
		R_root = R_spl.s;
	}

	return make_pair(L_root, R_root);
}


pair<node *, node *>split_rank(node *root, int rank)//rank and above are there in right subtree
//split the tree about the node of rank - rank
{
	node *L = 0;
	node * R = 0;
	
	if (root == 0)
		return make_pair(L, R);
	
	if (root->s_ize < rank)
		return make_pair(root, R);
		
	if (rank <= 1)
		return make_pair(L, root);
	
	R = root->right;
	L = root->left;
	
	if (L != 0)
		L->par = 0;
	if (R != 0)
		R->par = 0;
	
	root->left = 0;
	root->right = 0;
	os_node_update(root);
	
	node *L_root = 0;
	node *R_root = 0;
	
	if (1 + s_ize(L) >= rank)
	{
		pair<node*, node*>L_spl = split_rank(L, rank);
		R_root = merge(L_spl.s, root);
		R_root = merge(R_root, R);
		L_root = L_spl.f;
	}
	
	else
	{
		pair<node*, node*>R_spl = split_rank(R, rank-(1+s_ize(L)));
		L_root = merge(root, R_spl.f);
		L_root = merge(L, L_root);
		R_root = R_spl.s;
	}
	
	return make_pair(L_root, R_root);
}


int find_num(node *root,int rank)
{	
	if(rank >s_ize(root))
		
		return -10;//default value
	else
	{
		if(s_ize(root->left)>=rank)
			return find_num(root->left,rank);
		
		else if(s_ize(root->left)+1 == rank)
			return root->val;
		
		else
			return find_num(root->right,rank-(1+s_ize(root->left)));
	}
}


void print_inorder_traversal(node *root,vector<int>&arr)
{
	if (root == 0)
		return;
	print_inorder_traversal(root->left,arr);
	arr.push_back(root->val);
	print_inorder_traversal(root->right, arr);
}

int main()
{
	/*node *root = 0;
	vector<int>In{ 1,10,20,5,2,15,25,8 };
	for (int z : In)
	root = insert(root, z);
	pair<node*, node*>spl_roo = split(root, 10);*/

	node *root = 0;
	int N, M;
	cin >> N >> M;
	
	vector<ll>arr(N+1);
	vector<int>ord;
	
	node *L, *Mid, *H;
	for (int i = 1; i <= N; i++)
	{
		vector<int>ord2;
		cin >> arr[i];
		root = insert(root, i);
	}
	
	
	for (int a = 0; a < M; a++)
	{
		int q, i, j;
		cin >> q >> i >> j;
		pair<node*, node*>L_sp = split_rank(root, i);
		L = L_sp.f;
		Mid = L_sp.s;
		pair<node*, node*>H_sp = split_rank(Mid, j+2-i);
		Mid = H_sp.f;
		H = H_sp.s;
		
		root = merge(L, H);
		
		if (q == 1)
			root = merge(Mid, root);
		
		else
			root = merge(root, Mid);
	}
	print_inorder_traversal(root, ord);
	int N1 = ord[0];
	int N2 = ord[ord.size() - 1];
	ll abs = (arr[N1] > arr[N2]) ? arr[N1] - arr[N2] : arr[N2] - arr[N1];
	cout << abs << endl;
	for (int z : ord)
	cout << arr[z] << " ";
	
	return 0;
}
