#include <iostream>
#include <vector>
using namespace std;

#define Z -1000000000
struct cust{
	int D = -1;//deadline
	int M = -1;//time for exec
	bool operator<(const cust &rhs) const
	{
		return ((D<rhs.D)||((D==rhs.D)&&(M<rhs.M)));
	}
	
	bool operator>(const cust &rhs) const
	{
		return ((D>rhs.D)||((D==rhs.D)&&(M>rhs.M)));
	}
	
	cust(int d,int m):D(d),M(m){}
	cust(){}
};

struct node
{
	cust val;          //value
	node* left = 0;  //left child
	node* right = 0;//right child
	node *par = 0;  //parent of the node
	int ht = 0;     //height of the node
	
	//augmentation part
	int tsum = 0;//sum of all times for exec in the sub tree rooted at this node
	
	int T = 0;//timestamp of completion of current node(but without del applied)
	int ov_max = 0;//max overhead in the subtree rooted at this node(can be -ve too)
	int del = 0;//delta in the completion timestamps for every node in the subtree rooted at this node
};

int height(node *p)
{
	if (p == 0)
		return -1;
	return p->ht;
}

int get_tsum(node *p)
{
	if (p == 0)
		return 0;
	return p->tsum;
}

int get_ov_max(node *p)
{
	if (p == 0)
		return Z;
	return p->ov_max;
}

int get_T(node *p)
{
	if (p == 0)
		return 0;
	return p->T;
}
int get_del(node *p)
{
	if (p == 0)
		return 0;
	return p->del;
}


void os_node_update(node *N)
{
	if (N == 0)
		return;
	int l_h = height(N->left);
	int r_h = height(N->right);
	N->ht = 1 + ((l_h>r_h) ? l_h : r_h);
	int ov = N->T - (N->val).D;
	int ovl = get_ov_max(N->left) + get_del(N->left);
	int ovr = get_ov_max(N->right) + get_del(N->right);
	
	int max = (ov>ovl)?ov:ovl;
	max = (max>ovr)?max:ovr;
	N->ov_max = max;
	N->tsum =get_tsum(N->left) + get_tsum(N->right)+ (N->val).M;
}

void L_rotate(node *N)
{
	node *R = N->right;
	node *L = N->left;
	node *P = N->par;
	
	R->del += N->del;
	if(L!=0)
		L->del += N->del;
	
	N->T += N->del;
	N->del = 0;
	
	if(R->left != 0)
		R->left->del += R->del;
	if(R->right != 0)
		R->right->del += R->del;
	
	R->T += R->del;
	R->del = 0;
	
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
	node *R = N->right;
	node *P = N->par;
	
	L->del += N->del;
	if(R!=0)
		R->del += N->del;
	
	N->T += N->del;
	N->del = 0;
	
	if(L->left != 0)
		L->left->del += L->del;
	if(L->right != 0)
		L->right->del += L->del;
	
	L->T += L->del;
	L->del = 0;
	
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


node* insert(node *root, int d,int m)
{
	node *n = new node();
	(n->val).D = d;
	(n->val).M = m;
	if (root == 0)
	{
		n->T = m;
		n->tsum = m;
		n->ov_max = m-d;
		return n;
	}
	node *pre = 0;
	node *cur = root;
	int T = 0;
	while (cur != 0)
	{
		pre = cur;
		if(cur->left!=0)
			cur->left->del = cur->left->del + cur->del;
		
		if(cur->right!=0)
			cur->right->del = cur->right->del + cur->del;
		
		cur->T = cur->T + cur->del;
		cur->del = 0;
		
		if (cur->val>n->val)
		{
			cur->T = cur->T + m;
			if(cur->right!=0)
				cur->right->del += m;
			cur = cur->left;
		}
		else
		{
			T = T+(cur->val).M + get_tsum(cur->left);
			cur = cur->right;
		}
		
	}
	n->par = pre;
	n->T = T+m;
	
	if (pre->val>n->val)
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

int main()
{
	int T;
	cin>>T;
	node *root = 0;
	vector<int>ans(T);
	for(int t=0;t<T;t++)
	{
		int d,m;
		cin>>d>>m;
		root = insert(root,d,m);
		int temp = get_ov_max(root);
		ans[t] = (temp>0)?temp:0;
	}
	
	for(int z:ans)
		cout<<z<<endl;
	return 0;
}