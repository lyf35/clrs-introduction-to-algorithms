#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

//clrs exercise15.4-6
int find_pos(double elem, std::vector<int> pos, std::vector<double> a, int m, int n) //用于下面函数longest_increment_sequence的二分法搜索函数
{
	while (n != m)
	{
		if (elem < a[pos[(m + n) / 2]])
		{
			n = (m + n) / 2;
		}
		else if (elem == a[pos[(m + n) / 2]])
		{
			return pos[(m + n) / 2];
		}
		else
		{
			m = (m + n) / 2 + 1;
		}
	}
	if (elem < a[pos[n]]) //最后返回pos序列中不大于elem的最大元素
	{
		return pos[n - 1];
	}
	else
	{
		return pos[m];
	}
}

std::vector<int> longest_increment_sequence(std::vector<double> a)
{
	std::vector<int> result(1,1); //存放以每个位置元素为单调递增子序列的最后一个元素时，单增子序列的长度
	std::vector<int> element{ -1,0 }; //存放单增子序列的长度为i时，所对应的那个最小的元素的位置
	for (int i = 1;i < a.size();i++)
	{
		int max = 0;
		if (find_pos(a[i], element, a, 1, element.size() - 1) >= 0)
		{
			max = result[find_pos(a[i], element, a, 1, element.size() - 1)] + 1; //以a[i]作为结尾的最长子序列的长度
		}
		else
		{
			max = 1;
		}
		result.push_back(max);
		if (element.size() <= max) //如果element的大小小于或等于max，也就是说目前最长子序列的长度比max小，则需要将i推入element中
		{
			element.push_back(i);
		}
		else //如果element[max]这一位置所对应a中的元素的大小比a[i]大或者二者相等，那么需要对element中相对应位置的元素做修改
		{
			if (a[element[max]] >= a[i])
			{
				element[max] = i;
			}
		}
	}
	return result;
}
/*
调试代码：
int main()
{
	std::vector<double> a;
	uniform_int_distribution<int> rand(0, 10);
	default_random_engine e(time(0));
	for (int i = 0;i < 10;i++)
	{
		a.push_back(rand(e));
	}
	for (auto i : a)
	{
		std::cout << i << '\t';
	}
	std::cout << std::endl;
	auto c = longest_increment_sequence(a);
	for (auto i : c)
	{
		std::cout << i << '\t';
	}
	std::cout << std::endl;
	return 0;
}
*/