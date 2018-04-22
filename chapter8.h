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
#include"chapter2.h"

std::vector<int> counting_sort(const std::vector<int> vec, int k)
{
	std::vector<int> sort(vec.size(), 0), cnt(k + 1, 0);
	for (int i = 0;i < vec.size();i++)
	{
		cnt[vec[i]]++;
	}
	for (int i = 1;i < cnt.size();i++)
	{
		cnt[i] = cnt[i] + cnt[i - 1];
	}
	for (int i = vec.size() - 1;i >= 0;i--)
	{
		sort[cnt[vec[i]] - 1] = vec[i];
		cnt[vec[i]]--;
	}
	return sort;
}

int print_cnt(const std::vector<int> vec, int k, int a, int b)
{
	std::vector<int> cnt(k + 1, 0);
	for (int i = 0;i < vec.size();i++)
	{
		cnt[vec[i]]++;
	}
	for (int i = 1;i < cnt.size();i++)
	{
		cnt[i] = cnt[i] + cnt[i - 1];
	}
	return cnt[b] - cnt[a - 1];
}

std::vector<double> bucket_sort(const std::vector<double> vec)
{
	int n = vec.size();
	std::vector<std::vector<double>> arr(n, std::vector<double>(0, 0));
	for (int i = 0;i < vec.size();i++)
	{
		arr[floor(n*vec[i])].push_back(vec[i]);
	}
	for (auto i = arr.begin();i != arr.end();i++)
	{
		if (i->begin() != i->end())
		{
			for (int m = 1;m < i->size();m++)
			{
				auto key = (*i)[m];
				auto j = m - 1;
				while (j >= 0 && (*i)[j]>key)
				{
					(*i)[j + 1] = (*i)[j];
					j--;
				}
				(*i)[j + 1] = key;
			}
		}
	}
	std::vector<double> out;
	for (auto i = arr.begin();i != arr.end();i++)
	{
		if (i->begin() != i->end())
		{
			for (auto j : *i)
			{
				out.push_back(j);
			}
		}
	}
	return out;
}

std::vector<std::tuple<double, double>> point_sort(const std::vector<std::tuple<double, double>> vec)
{
	int n = vec.size();
	std::vector<std::vector<std::tuple<double, double>>> arr(n, std::vector<std::tuple<double, double>>(0, std::make_tuple(0, 0)));
	for (int i = 0;i < vec.size();i++)
	{
		double x = std::get<0>(vec[i]);
		double y = std::get<1>(vec[i]);
		arr[floor(n*sqrt(pow(x, 2) + pow(y, 2)))].push_back(vec[i]);
	}
	for (auto i = arr.begin();i != arr.end();i++)
	{
		if (i->begin() != i->end())
		{
			for (int m = 1;m < i->size();m++)
			{
				auto keypoint = (*i)[m];
				auto key = sqrt(pow(std::get<0>((*i)[m]), 2) + pow(std::get<1>((*i)[m]), 2));
				auto j = m - 1;
				while (j >= 0 && sqrt(pow(std::get<0>((*i)[m]), 2) + pow(std::get<1>((*i)[m]), 2))>key)
				{
					(*i)[j + 1] = (*i)[j];
					j--;
				}
				(*i)[j + 1] = keypoint;
			}
		}
	}
	std::vector<std::tuple<double, double>> out;
	for (auto i = arr.begin();i != arr.end();i++)
	{
		if (i->begin() != i->end())
		{
			for (auto j : *i)
			{
				out.push_back(j);
			}
		}
	}
	return out;
}
