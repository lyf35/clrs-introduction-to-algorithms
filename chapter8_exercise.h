#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>

//exercise8.4-4
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
