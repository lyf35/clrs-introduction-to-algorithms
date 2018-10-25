#pragma once
#include<set>
#include<vector>

template<typename T> std::vector<bool> greedy_set_cover(std::vector<std::set<T>> subset) //利用贪心算法，每次选择未覆盖点最多的子集
{
	std::vector<bool> set_choice(subset.size(), 0);
	bool is_set_exist = 1; //用于标记子集中是否还有未覆盖点
	while (is_set_exist)
	{
		int max_set = 0; //代表最大集合的编号
		for (int i = 0;i < subset.size();i++) //挑选出包含未覆盖点最多的点集
		{
			if (subset[i].size()>subset[max_set].size())
			{
				max_set = i;
			}
		}
		set_choice[max_set] = 1;
		for (auto iter = subset[max_set].begin();iter != subset[max_set].end();iter++) //删除掉已选择的点
		{
			for (int i = 0;i < subset.size();i++)
			{
				if (i != max_set)
				{
					subset[i].erase(*iter);
				}
			}
		}
		subset[max_set].clear();
		int cnt = 0;
		for (int i = 0;i < subset.size();i++) //统计集合中空集的个数，如果所有的都为空集则可以结束while循环
		{
			if (subset[i].size() == 0)
			{
				cnt++;
			}
		}
		if (cnt == subset.size())
		{
			is_set_exist = 0;
		}
	}
	return set_choice;
}

/*
调试代码（clrs图35-3）：
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"greedy_set_cover.h"

int main()
{
	std::vector<std::set<int>> test{ {1,2,3,4,5,6},{5,6,8,9},{1,4,7,10},{2,5,7,8,11},{3,6,9,12},{10,11} };
	auto result = greedy_set_cover(test);
	for (auto i = 0;i < result.size();i++)
	{
		if (result[i])
		{
			std::cout << i << " ";
		}
	}
	std::cout << std::endl;
	return 0;
}

*/