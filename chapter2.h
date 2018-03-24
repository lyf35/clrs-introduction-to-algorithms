#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>

template<typename T> void insert_increment_sorting(T& a)
{
	std::cout << "The container before sorting is:" << std::endl;
	for_each(a.begin(), a.end(), [](double num) {std::cout << num << '\t';});
	std::cout << std::endl;
	for (int i = 1;i < a.size();i++)
	{
		/*
		for (int j = 0;j < i;j++)
		{
		if (a[i] < a[j])
		{
		auto temp = a[i];
		a.erase(a.begin() + i);
		a.insert(a.begin() + j, temp);
		break;
		}
		}
		*/
		auto key = a[i];
		auto j = i - 1;
		while (j >= 0 && a[j]>key)
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
		std::cout << "The container after the "<<i<<" th sorting is:" << std::endl;
		for_each(a.begin(), a.end(), [](double num) {std::cout << num << '\t';});
		std::cout << std::endl;
	}
}

template<typename T> void insert_decrement_sorting(T& a)
{
	std::cout << "The container before sorting is:" << std::endl;
	for_each(a.begin(), a.end(), [](double num) {std::cout << num << '\t';});
	std::cout << std::endl;
	for (int i = 1;i < a.size();i++)
	{
		/*
		for (int j = 0;j < i;j++)
		{
		if (a[i] > a[j])//modified
		{
		auto temp = a[i];
		a.erase(a.begin() + i);
		a.insert(a.begin() + j, temp);
		break;
		}
		}
		*/
		auto key = a[i];
		auto j = i - 1;
		while (j >= 0 && a[j]<key) //modified
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
		std::cout << "The container after the " << i << " th sorting is:" << std::endl;
		for_each(a.begin(), a.end(), [](double num) {std::cout << num << '\t';});
		std::cout << std::endl;
	}
}

std::vector<double> merge(std::vector<double> a, std::vector<double> b)
{
	std::vector<double> temp;
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

std::vector<double> merge_sort(std::vector<double> vec)
{
	std::vector<double> temp1(vec.begin(), vec.begin() + floor(vec.size() / 2));
	std::vector<double> temp2(vec.begin() + floor(vec.size() / 2), vec.end());
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

std::vector<double>::iterator my_binary_search(std::vector<double>::iterator beg, std::vector<double>::iterator end, double elem)//返回正好相等的那个元素的迭代器，或者可以在某元素位置之前插入不影响整体顺序的那个迭代器
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
void modified_insertion_sort(std::vector<double>& a)
{
	std::cout << "The container before sorting is:" << std::endl;
	for_each(a.begin(), a.end(), [](double num) {std::cout << num << '\t';});
	std::cout << std::endl;
	for (int i = 1;i < a.size();i++)
	{
		auto iter = my_binary_search(a.begin(), a.begin() + i, a[i]);
		a.insert(iter, a[i]);
		a.erase(a.begin() + i + 1);
		std::cout << "The container after the " << i << " th sorting is:" << std::endl;
		for_each(a.begin(), a.end(), [](double num) {std::cout << num << '\t';});
		std::cout << std::endl;
	}
}
void find_elements(std::vector<double> vec,double num)//运行之前先做归并排序，让集合中的元素按照升序排列
{
	int i = 0;
	int cnt = 0;
	while (i < vec.size() - 1&&vec[i] + vec[i + 1] <= num)//注意&&判断条件的计算顺序，右边部分放在左边的时候会优先计算从而可能越界
	{
		double temp = num - vec[i];
		auto x = my_binary_search(vec.begin() + i, vec.end(), temp);
		if (x != vec.end())
		{
			if (*x == temp)
			{
				std::cout << "The elements are " << vec[i] << " and " << *x << std::endl;
				cnt++;
			}
		}
		i++;
	}
	if (cnt == 0)
	{
		std::cout << "No element found." << std::endl;
	}
}