// Selecting_Optimal-k.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>

vector<string>reads;
int V_num;
int r_len;//length of reads
using namespace std;

void radix_sort(vector<string>&r, vector<int>&look, int k)
{
	for (int rad = k - 1; rad >= 0; rad--)
	{
		vector<int>Cf(4);//A->0,C->1,G->2,T->3
		vector<int>temp(look.size(), 0);
		for (int i = 0; i < r.size(); i++)
		{
			int s = -1;
			switch (r[i][rad])
			{
				case 'A':
					s = 0;
					break;

				case 'C':
					s = 1;
					break;

				case 'G':
					s = 2;
					break;

				case 'T':
					s = 3;
					break;
			}
			Cf[s]++;
		}

		for (int i = 1; i < 4; i++)
			Cf[i] += Cf[i - 1];

		for (int i = look.size() - 1; i >= 0; i--)
		{
			int s = -1;
			switch (r[i][rad])
			{
				case 'A':
					s = 0;
					break;

				case 'C':
					s = 1;
					break;

				case 'G':
					s = 2;
					break;

				case 'T':
					s = 3;
					break;
			}
			temp[Cf[s] - 1] = look[i];
			Cf[s]--;
		}

		look = temp;
	}
	return;
}

bool is_cycle_present(int k)//k chosen for k-mers
{
	vector<string>k_s;
	for (int i = 0; i < reads.size(); i++)
	{
		for (int j = 0; j < reads[i].size() - k + 1; j++)
			k_s.push_back(reads[i].substr(j, k - 1));
	}
	vector<int>ref_k_s(k_s.size());
	for (int i = 0; i < k_s.size(); i++)
		ref_k_s[i] = i;
	radix_sort(k_s, ref_k_s, k - 1);
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
	int l = 0;
	int h = reads[0].size();

	int mid; 
	while (l < h)
	{
		mid = (l + h) / 2;
		bool ret = is_cycle_present(mid);
		if (ret)
			l = mid;
		else
			h = mid - 1;
	}
	cout << mid << endl;
    return 0;
}

