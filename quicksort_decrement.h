#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<random>
#include<ctime>

//½µÐò¿ìÅÅ
template<typename T> size_t partition_decrease(std::vector<T>& vec, int p, int r)
{
	T x = vec[r];
	int i = p - 1;
	int cnt = 0;
	for (auto j = p;j <= r - 1;j++)
	{
		if (vec[j] >= x)
		{
			i++;
			if (vec[j] == x)
			{
				cnt++;
			}
			std::swap(vec[i], vec[j]);
		}
	}
	std::swap(vec[i + 1], vec[r]);
	if (cnt == r - p)
	{
		return (p + r) / 2;
	}
	else
	{
		return i + 1;
	}
}

template<typename T> std::vector<T> quicksort_decrease(std::vector<T>& vec, int p, int r)
{
	if (p < r)
	{
		double q = partition_decrease(vec, p, r);
		quicksort_decrease(vec, p, q - 1);
		quicksort_decrease(vec, q + 1, r);
	}
	return vec;
}