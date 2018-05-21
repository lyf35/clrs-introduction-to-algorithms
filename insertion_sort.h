#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include"merge_sort.h"

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
		std::cout << "The container after the " << i << " th sorting is:" << std::endl;
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

template<typename T> void modified_insertion_sort(std::vector<T>& a) //加入二分查找提高插入排序的搜索效率
{
	std::cout << "The container before sorting is:" << std::endl;
	for_each(a.begin(), a.end(), [](T num) {std::cout << num << '\t';});
	std::cout << std::endl;
	for (int i = 1;i < a.size();i++)
	{
		auto iter = my_binary_search(a.begin(), a.begin() + i, a[i]);
		a.insert(iter, a[i]);
		a.erase(a.begin() + i + 1);
		std::cout << "The container after the " << i << " th sorting is:" << std::endl;
		for_each(a.begin(), a.end(), [](T num) {std::cout << num << '\t';});
		std::cout << std::endl;
	}
}