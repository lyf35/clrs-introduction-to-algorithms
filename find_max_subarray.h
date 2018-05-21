#pragma once
#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>

template<typename T> std::tuple < std::vector<T>::iterator, std::vector<T>::iterator, T > find_max_crossing_subarray(std::vector<T>::iterator beg, std::vector<T>::iterator end)
{
	T sum = 0;
	T cnt = 0;
	std::vector<T>::iterator left, right;
	int pos = (end - beg) / 2;
	while (pos >= 0)
	{
		cnt += *(beg + pos);
		if (cnt > sum)
		{
			sum = cnt;
			left = beg + pos;
		}
		pos--;
	}
	pos = (end - beg) / 2 + 1;
	cnt = sum;
	while (beg + pos < end)
	{
		cnt += *(beg + pos);
		if (cnt > sum)
		{
			sum = cnt;
			right = beg + pos;
		}
		pos++;
	}
	return std::make_tuple(left, right, sum);
}
template<typename T> std::tuple < std::vector<T>::iterator, std::vector<T>::iterator, T > find_max_subarray(std::vector<T>::iterator beg, std::vector<T>::iterator end)
{
	if (beg == end)
	{
		return std::make_tuple(beg, beg, 0);
	}
	if (beg + 1 == end)
	{
		return std::make_tuple(beg, beg, *beg);
	}
	else
	{
		auto mid = beg + (end - beg) / 2;
		auto leftinfo = find_max_subarray(beg, mid);
		auto rightinfo = find_max_subarray(mid + 1, end);
		auto crossinfo = find_max_crossing_subarray(beg, end);
		if (std::get<2>(leftinfo) >= std::get<2>(rightinfo) && std::get<2>(leftinfo) >= std::get<2>(crossinfo))
		{
			return leftinfo;
		}
		else if (std::get<2>(crossinfo) >= std::get<2>(rightinfo) && std::get<2>(crossinfo) >= std::get<2>(leftinfo))
		{
			return crossinfo;
		}
		else
		{
			return rightinfo;
		}
	}
}
template<typename T> std::tuple < std::vector<T>::iterator, std::vector<T>::iterator, T > find_max_subarray_exhaustion(std::vector<T>::iterator beg, std::vector<T>::iterator end) //暴力求解的算法
{
	std::vector<T>::iterator left, right;
	T sum = 0;
	if (beg == end)
	{
		return std::make_tuple(beg, beg, 0);
	}
	else
	{
		for (int i = 0;i < end - beg;i++)
		{
			T temp = 0;
			std::vector<T>::iterator temp_left = beg + i;
			for (int j = i;j < end - beg;j++)
			{
				temp += *(beg + j);
				if (temp > sum)
				{
					left = temp_left;
					right = beg + j;
					sum = temp;
				}
			}
		}
		return std::make_tuple(left, right, sum);
	}
}