// largest_palindrome_substr.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

void manacher(string &s, vector<int>&P)
{
	P[0] = 0;//P->denotes radius
	int C = 0;

	for (int i = 1; i < P.size(); i++)
	{
		int R = C + P[C];
		int L = C - P[C];
		int i1 = 2 * C - i;
		if (R > i)
		{
			if (L < i1 - P[i1])
				P[i] = P[i1];//Contradiction to this contradicts the fact that P[i1] is correct

			else if (i1 - P[i1] < L)
				P[i] = R - i;//Contradiction to this contradicts the fact that P[C] is correct

			else
			{
				P[i] = R - i;
				while (i - P[i] - 1 >= 0 && i + P[i] + 1 < s.size() && s[i + P[i] + 1] == s[i - P[i] - 1])
					P[i]++;
				C = i;
			}
		}

		else
		{
			P[i] = 0;
			while (i - P[i] - 1 >= 0 && i + P[i] + 1 < s.size() && s[i + P[i] + 1] == s[i - P[i] - 1])
				P[i]++;
			C = i;
		}
	}
	return;
}
int main()
{
	string s;
	cin >> s;
	string s2;
	s2 = "#";
	vector<int>P;
	for (char z : s)
	{
		s2 += z;
		s2 += '#';
	}

	P.resize(s2.size());
	manacher(s2, P);
	
	int max_len = -1;
	int max_i = -1;
	for (int i = 0; i < P.size(); i++)
	{
		if (P[i] > max_len)
		{
			max_len = P[i];
			max_i = i;
		}
	}

	string ans;
	for (int i = max_i - P[max_i]; i <= max_i + P[max_i]; i++)
	{
		if (s2[i] != '#')
			ans += s2[i];
	}

	cout << ans << endl;
    return 0;
}
