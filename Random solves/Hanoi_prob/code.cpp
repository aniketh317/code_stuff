#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct node{
	bool v = false;
	int d = -1;
	vector<int>adj;
};

vector<node>G;

void BFS() //Breadth first search
{
	queue<int>q;
	
	G[0].d = 0;
	G[0].v = true;
	q.push(0);
	while(!q.empty())
	{
		int f = q.front();
		q.pop();
		for(int z:G[f].adj)
		{
			if(!G[z].v)
			{
				G[z].v = true;
				G[z].d = G[f].d +1;
				q.push(z);
			}
		}
	}
	return;
}
int main()
{
	int N;
	cin>>N;
	
	int value = 0;
	for(int i=0;i<N;i++)
	{
		int k;
		cin>>k;
		value+=((1<<(i<<1))*(k-1));
	}
	
	G.resize(1<<(N<<1));
	
	for(int i=0;i<(1<<(N<<1));i++)
	{
		vector<vector<int>>z(4);
		int j=0;
		int k = i;
		while(j<N)
		{
			
			int d0= k-((k>>1)<<1);
			k=k>>1;
			int d1 = k-((k>>1)<<1);
			k = k>>1;
			int r = (d1<<1)+d0;
			z[r].push_back(j);
			j++;
		}
		
		//15 is some dummy value (>10)
		vector<int>v(4);
		v[0] = z[0].size()>0?z[0][0]:15;
		v[1] = z[1].size()>0?z[1][0]:15;
		v[2] = z[2].size()>0?z[2][0]:15;
		v[3] = z[3].size()>0?z[3][0]:15;
		
		int val = 0;
		for(int k1=0;k1<4;k1++)
		{
			for(int k2=0;k2<4;k2++)
			{
				if(k1==k2)
					continue;
				
				if(v[k1]<v[k2])
				{
					val = i+(k2-k1)*(1<<((v[k1])<<1));
					G[i].adj.push_back(val);
				}
			}
		}
	}
	
	BFS();
	
	cout<<G[value].d<<endl;
	
	return 0;
}
