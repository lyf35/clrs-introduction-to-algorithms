#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<limits>
#include<random>
#include<ctime>
#include"chapter7.h"

double randomized_select(std::vector<double> vec, int p, int r, int i)
{
	if (p == r)
	{
		return vec[p];
	}
	auto q = randomized_partition(vec, p, r);
	auto k = q - p + 1;
	if (i == k)
	{
		return vec[k];
	}
	else if (i < k)
	{
		return randomized_select(vec, p, q - 1, i);
	}
	else
	{
		return randomized_select(vec, q + 1, r, i - k);
	}
}

double randomized_select_iter(std::vector<double> vec, int p, int r, int i)
{
	int cnt = 0;
	while (1)
	{
		if (p == r)
		{
			return vec[p];
		}
		else if (p > r)
		{
			throw("p should be not larger than r.");
		}
		else
		{
			int q = randomized_partition(vec, p, r);
			int k = q - p + 1;
			if (i == k)
			{
				return vec[q];
			}
			else if (i < k)
			{
				r = q - 1;
			}
			else
			{
				p = q + 1;
				i = i - k;
				cnt += k;
			}
		}
	}
}