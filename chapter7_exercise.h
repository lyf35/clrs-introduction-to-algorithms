#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<random>
#include<ctime>
#include"quicksort_increment.h"

//hoare划分算法及其快排。大致思路是以vec[p]为轴点元素，从p和r两边往中间搜索，直至两端的搜索过程相遇
template<typename T> int hoare_partition(std::vector<T>& vec, int p, int r)
{
	T x = vec[p];
	int i = p;
	int j = r;
	while (1)
	{
		while (vec[j] > x)
		{
			j--;
		}
		while (vec[i] < x)
		{
			i++;
		}
		if (i < j)
		{
			std::swap(vec[i], vec[j]);
		}
		else
		{
			return j;
		}
	}
}

template<typename T> std::vector<T> hoare_quicksort(std::vector<T>& vec, int p, int r)
{
	if (p < r)
	{
		int q = hoare_partition(vec, p, r);
		hoare_quicksort(vec, p, q - 1);
		hoare_quicksort(vec, q + 1, r);
	}
	return vec;
}

//尾递归快速排序。节省堆栈空间
template<typename T> std::vector<T> tail_recursive_quicksort(std::vector<T>& vec, int p, int r)
{
	while (p < r)
	{
		auto q = partition(vec, p, r);
		tail_recursive_quicksort(vec, p, q - 1);
		p = q + 1;
	}
	return vec;
}

template<typename T> std::vector<T> modified_tail_recursive_quicksort(std::vector<T>& vec, int p, int r)
{
	while (p < r)
	{
		auto q = partition(vec, p, r);
		if (q <= (r - p + 1) / 2)
		{
			modified_tail_recursive_quicksort(vec, q + 1, r);
			r = q - 1;
		}
		else
		{
			modified_tail_recursive_quicksort(vec, p, q - 1);
			p = q + 1;
		}
	}
	return vec;
}


//区间的模糊排序|此处注意，只需要满足如下条件即可：在排序好的区间内，按从左到右的顺序，只要每个区间内存在一个数，使得这些数可以按照从小到大排列。因此在写程序的时候可以充分利用区间的重叠性质来写，只要两段区间有重叠区间，我们便可以认为两个区间相等，并根据此来进一步缩小重叠区间的范围。可以根据具有相同值的快排算法做简单修改
std::tuple<int,int> section_partition(std::vector<std::tuple<double, double>>& vec, int p, int r)
{
	double x = std::get<0>(vec[r]);
	double y = std::get<1>(vec[r]);
	int i = p - 1;
	int cnt = 0;
	for (auto j = p;j <= r - 1 - cnt;j++)
	{
		if (std::get<0>(vec[j]) < x)//完全在区间左侧
		{
			i++;
			std::swap(vec[i], vec[j]);
		}
		else if (std::get<1>(vec[j]) > y)//完全在区间右侧
		{
			;
		}
		else//与区间有重叠，并缩小重叠区间的范围 
		{
			cnt++;
			x = std::max(std::get<0>(vec[j]), x);
			y = std::min(std::get<1>(vec[j]), y);
			std::swap(vec[j], vec[r - cnt]);
			j--;
		}
	}
	for (auto j = 0;j <= cnt;j++)
	{
		std::swap(vec[i + 1 + j], vec[r - cnt + j]);
	}
	return std::make_tuple(i + 1, i + 1 + cnt);
}

std::tuple<int, int> randomized_section_partition(std::vector<std::tuple<double,double>>& vec, int p, int r)
{
	std::uniform_int_distribution<int> u(p, r);
	static std::default_random_engine e(time(0));
	int i = u(e);
	std::swap(vec[i], vec[r]);
	return section_partition(vec, p, r);
}

std::vector<std::tuple<double, double>> section_sort(std::vector<std::tuple<double, double>>& vec, int p, int r)
{
	if (p < r)
	{
		auto q = section_partition(vec, p, r);
		section_sort(vec, p, std::get<0>(q) - 1);
		section_sort(vec, std::get<1>(q) + 1, r);
	}
	return vec;
}

