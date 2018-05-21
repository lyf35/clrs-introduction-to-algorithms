#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<random>
#include<ctime>
#include"quicksort_increment.h"

//随机快排。如果要提高”好划分“的概率，可以改用随机选三个数然后取中位数的随机快排，可以取得更好的效果。
template<typename T> int randomized_partition(std::vector<T>& vec, int p, int r)
{
	std::uniform_int_distribution<int> u(p, r);
	static std::default_random_engine e(time(0));
	int i = u(e);
	std::swap(vec[i], vec[r]);
	return partition(vec, p, r);
}

template<typename T> std::vector<T> randomized_quicksort(std::vector<T>& vec, int p, int r)
{
	if (p < r)
	{
		int q = randomized_partition(vec, p, r);
		randomized_quicksort(vec, p, q - 1);
		randomized_quicksort(vec, q + 1, r);
	}
	return vec;
}

//改进后的算法，可以对包含相同数字的数组进行快排
template<typename T> std::tuple<int, int> partition_mod(std::vector<T>& vec, int p, int r)
{
	T x = vec[r];
	int i = p - 1;
	int cnt = 0;
	for (auto j = p;j <= r - 1 - cnt;j++)
	{
		if (vec[j] < x)
		{
			i++;
			std::swap(vec[i], vec[j]);
		}
		if (vec[j] == x)
		{
			cnt++;
			std::swap(vec[j], vec[r - cnt]);
			j--;
		}
	}
	for (auto j = 0;j <= cnt;j++)
	{
		std::swap(vec[i + 1 + j], vec[r - j]);
	}
	return std::make_tuple(i + 1, i + 1 + cnt);
}

template<typename T> std::tuple<int, int> randomized_partition_mod(std::vector<T>& vec, int p, int r)
{
	std::uniform_int_distribution<int> u(p, r);
	static std::default_random_engine e(time(0));
	int i = u(e);
	std::swap(vec[i], vec[r]);
	return partition_mod(vec, p, r);
}

template<typename T> std::vector<double> randomized_quicksort_mod(std::vector<T>& vec, int p, int r)
{
	if (p < r)
	{
		auto q = randomized_partition_mod(vec, p, r);
		randomized_quicksort_mod(vec, p, std::get<0>(q) - 1);
		randomized_quicksort_mod(vec, std::get<1>(q) + 1, r);
	}
	return vec;
}
