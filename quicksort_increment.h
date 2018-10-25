#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<random>
#include<ctime>

//升序快排
template<typename T> size_t partition(std::vector<T>& vec, int p, int r) //对数组p~r部分做划分
{
	T x = vec[r];
	int i = p - 1;
	int cnt = 0;
	for (auto j = p;j <= r - 1;j++)
	{
		if (vec[j] <= x)
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
	if (cnt == r - p) //如果整个划分区域全部都是一样的数，则返回它们的中点处
	{
		return (p + r) / 2;
	}
	else
	{
		return i + 1;
	}
}

template<typename T> std::vector<T> quicksort(std::vector<T>& vec, int p, int r)
{
	if (p < r)
	{
		T q = partition(vec, p, r);
		quicksort(vec, p, q - 1);
		quicksort(vec, q + 1, r);
	}
	return vec;
}
