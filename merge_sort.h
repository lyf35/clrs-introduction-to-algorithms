#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>

template<typename T> std::vector<T> merge(std::vector<T> a, std::vector<T> b)
{
	std::vector<T> temp;
	int i = 0, j = 0;
	while (i <= a.size() && j <= a.size())
	{
		if (a[i] < b[j])
		{
			temp.push_back(a[i]);
			i++;
		}
		else
		{
			temp.push_back(b[j]);
			j++;
		}
		if (i == a.size())
		{
			temp.insert(temp.end(), b.begin() + j, b.end());
			break;
		}
		if (j == b.size())
		{
			temp.insert(temp.end(), a.begin() + i, a.end());
			break;
		}
	}
	return temp;
}

template<typename T> std::vector<T> merge_sort(std::vector<T> vec)
{
	std::vector<T> temp1(vec.begin(), vec.begin() + floor(vec.size() / 2));
	std::vector<T> temp2(vec.begin() + floor(vec.size() / 2), vec.end());
	if (temp1.size() > 1)
	{
		temp1 = merge_sort(temp1);
	}
	if (temp2.size() > 1)
	{
		temp2 = merge_sort(temp2);
	}
	return merge(temp1, temp2);
}

template<typename T> std::vector<T>::iterator my_binary_search(std::vector<T>::iterator beg, std::vector<T>::iterator end, T elem)//返回正好相等的那个元素的迭代器，或者可以在某元素位置之前插入不影响整体顺序的那个迭代器
{
	if (end==beg)
	{
		return end;
	}
	else
	{
		if (*(beg+(end-beg)/2)==elem)
		{
			return beg + (end - beg) / 2;
		}
		else if (*(beg + (end - beg) / 2) > elem)
		{
			return my_binary_search(beg, beg + (end - beg) / 2, elem);
		}
		else
		{
			return my_binary_search(beg + (end - beg) / 2 + 1, end, elem);
		}
	}
}


