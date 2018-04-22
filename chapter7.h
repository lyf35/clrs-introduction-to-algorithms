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


//升序快排
size_t partition(std::vector<double>& vec, int p, int r)
{
	double x = vec[r];
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
	if (cnt == r - p)
	{
		return (p + r) / 2;
	}
	else
	{
		return i + 1;
	}
}

std::vector<double> quicksort(std::vector<double>& vec, int p, int r)
{
	if (p < r)
	{
		double q = partition(vec, p, r);
		quicksort(vec, p, q - 1);
		quicksort(vec, q + 1, r);
	}
	return vec;
}


//降序快排
size_t partition_decrease(std::vector<double>& vec, int p, int r)
{
	double x = vec[r];
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

std::vector<double> quicksort_decrease(std::vector<double>& vec, int p, int r)
{
	if (p < r)
	{
		double q = partition_decrease(vec, p, r);
		quicksort_decrease(vec, p, q - 1);
		quicksort_decrease(vec, q + 1, r);
	}
	return vec;
}

//随机快排
int randomized_partition(std::vector<double>& vec, int p, int r)
{
	std::uniform_int_distribution<int> u(p, r);
	static std::default_random_engine e(time(0));
	int i = u(e);
	std::swap(vec[i], vec[r]);
	return partition(vec, p, r);
}

std::vector<double> randomized_quicksort(std::vector<double>& vec, int p, int r)
{
	if (p < r)
	{
		int q = randomized_partition(vec, p, r);
		randomized_quicksort(vec, p, q - 1);
		randomized_quicksort(vec, q + 1, r);
	}
	return vec;
}

//hoare划分算法及其快排
int hoare_partition(std::vector<double>& vec, int p, int r)
{
	double x = vec[p];
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

std::vector<double> hoare_quicksort(std::vector<double>& vec, int p, int r)
{
	if (p < r)
	{
		int q = hoare_partition(vec, p, r);
		hoare_quicksort(vec, p, q - 1);
		hoare_quicksort(vec, q + 1, r);
	}
	return vec;
}


//改进后的算法，可以对包含相同数字的数组进行快排
std::tuple<int, int> partition_mod(std::vector<double>& vec, int p, int r)
{
	double x = vec[r];
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

std::tuple<int, int> randomized_partition_mod(std::vector<double>& vec, int p, int r)
{
	std::uniform_int_distribution<int> u(p, r);
	static std::default_random_engine e(time(0));
	int i = u(e);
	std::swap(vec[i], vec[r]);
	return partition_mod(vec, p, r);
}

std::vector<double> randomized_quicksort_mod(std::vector<double>& vec, int p, int r)
{
	if (p < r)
	{
		auto q = randomized_partition_mod(vec, p, r);
		randomized_quicksort_mod(vec, p, std::get<0>(q) - 1);
		randomized_quicksort_mod(vec, std::get<1>(q) + 1, r);
	}
	return vec;
}

std::vector<double> tail_recursive_quicksort(std::vector<double>& vec, int p, int r)
{
	while (p < r)
	{
		auto q = partition(vec, p, r);
		tail_recursive_quicksort(vec, p, q - 1);
		p = q + 1;
	}
	return vec;
}

std::vector<double> modified_tail_recursive_quicksort(std::vector<double>& vec, int p, int r)
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


//区间的模糊排序|此处注意，只需要满足如下条件即可：在排序好的区间内，按从左到右的顺序，只要每个区间内存在一个数，使得这些数可以按照从小到大排列。因此在写程序的时候可以充分利用区间的重叠性质来写，只要两段区间有重叠区间，我们便可以认为两个区间相等，并根据此来进一步缩小重叠区间的范围。
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

