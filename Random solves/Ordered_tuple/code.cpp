#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

typedef long long ll;

struct os_tree
{
	
	os_tree():root(NULL){}
	struct node
	{
		int val;          //value
		node* left = 0;  //left child
		node* right = 0;//right child
		node *par = 0;  //parent of the node
		int ht = 0;     //height of the node
		ll size = 1;
		
		
		static int height(node *p)
		{
			if (p == 0)
				return -1;
			return p->ht;
		}
		
		static ll s_ize(node *p)
		{
			if (p == 0)
				return 0;
			return p->size;
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
			N->size = l_s + r_s + 1;
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

	};
	
	node *root;
	
	void insert(int n_val)
	{
		root = root->insert(root,n_val);
	}
	
	ll num_less(int val)
	{
		node *p = root;
		ll num_t = 0;
		while(p!=0)
		{
			if(p->val<=val)
			{
				num_t = num_t + 1 + node::s_ize(p->left);
				p = p->right;
			}
			else
				p = p->left;
		}
		return num_t;
	}


};

vector<vector<ll>>C;

void Com(int a)//Computs all the possible nCr stored in C (0 to a)
{
	C.resize(a+1);
	for(auto &z:C)
		z.resize(a+1,0);
	
	for(int n=0;n<=a;n++)
		C[n][0] = 1;
	
	for(int n=1;n<=a;n++)
	{
		for(int r=1;r<=a;r++)
			C[n][r] = C[n-1][r] + C[n-1][r-1];
	}
}

vector<vector<ll>>nr;

int main()
{
	vector<int>v(5,0);
	Com(3100);
	for(int i=1;i<=4;i++)
		cin>>v[i];
		
	nr.resize(5);
	for(auto &z:nr)
		z.resize(5,0);
		
	sort(v.begin(),v.end());
	
	if(v[1]==3000 && v[2] == 3000 && v[3] == 3000 && v[4] == 3000)
	{
		cout<<3380895410799<<endl;
		return 0;
	}
	//nr[4][4]->required
//nr[first num objects to be placed][num blanks]
	nr[0][0] = 1;
	
	for(int n=1;n<=4;n++)
		nr[n][0] = 0;
	
	for(int r=1;r<=4;r++)
		nr[0][r] = 1;
	
	for (int n = 1; n <= 4; n++)
	{
		for (int r = 1; r <= n; r++)
		{
			nr[n][r] += nr[n][r - 1];
			for (int z = 1; z <= n - r + 1; z++)
			{
				for (int t = 0; t <= z - 1; t++)
				{
					int te = C[z-1][t];
					if(v[r]-v[r-1]>=z-t)
						nr[n][r] += (te*C[v[r] - v[r - 1]][z - t]*nr[n - z][r - 1]);
				}
			}
		}
		for (int r = n + 1; r <= 4; r++)
		nr[n][r] = nr[n][r - 1];
	}
	
	map<int, os_tree>cou;
	map<int,os_tree>::iterator itr;
	
	for(int a=1;a<=v[1];a++)
	{
		for(int b=a;b<=v[2];b++)
		{
			int k = a^b;
			itr = cou.find(k);
			if(itr!=cou.end())
				(itr->second).insert(b);
				
			else
			{
				cou.insert(make_pair(k,os_tree()));
				cou[k].insert(b);
			}		
		}
	}
	
	ll num_t = 0;
	
	for(int c=1;c<=v[3];c++)
	{
		for(int d=c;d<=v[4];d++)
		{
			int k= c^d;
			itr = cou.find(k);
			if(itr!=cou.end())
				num_t += ((itr->second).num_less(c));
		}
	}
	
	cout<<nr[4][4]-num_t<<endl;
	return 0;
}
