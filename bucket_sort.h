#pragma once
#include<iostream>
#include<vector>

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