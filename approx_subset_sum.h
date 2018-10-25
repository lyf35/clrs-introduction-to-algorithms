#pragma once
#include<iostream>
#include<vector>
#include<set>

std::set<int> merge_lists(std::set<int> L1, int x)
{
	std::set<int> merged_list = L1; //可以先把L1中的元素直接复制到合并的集合内
	for (auto i = L1.begin();i != L1.end();i++)
	{
		merged_list.insert(*i + x);
	}
	return merged_list;
}

void delete_elem(std::set<int> &L, int t) //删除集合中大于t的元素
{
	auto i = L.begin();
	for (;i != L.end();i++)
	{
		if (*i > t)
		{
			break;
		}
	}
	L.erase(i, L.end());
}

int exact_subset_sum(std::vector<int> input, int sum) //寻找是否存在和刚好为t的一个子集，如果不存在，则返回小于t的最大值
{
	std::set<int> P{ 0 }; //第1-i个元素可以组成的和所组成的集合
	for (auto i = 0;i < input.size();i++)
	{
		auto temp = P;
		P = merge_lists(P, input[i]);
		delete_elem(P, sum);
	}
	int max = 0;
	for (auto i = P.begin();i != P.end();i++)
	{
		if (*i > max)
		{
			max = *i;
		}
	}
	return max;
}

std::set<int> trim(std::set<int> L, double delta) //根据所设置的delta值，对集合中满足y ~ y*(1+delta)的所有值都用y代替
{
	std::set<int> L_trim;
	auto iter = L.begin();
	int last = *iter;
	L_trim.insert(*iter);
	while (iter != L.end())
	{
		if ((double)*iter > (double)last*(1 + delta))
		{
			L_trim.insert(*iter);
			last = *iter;
		}
		iter++;
	}
	return L_trim;
}

int approx_subset_sum(std::vector<int> input, int sum, double approx) //一个寻找子集元素和不超过sum的近似算法，approx+1为近似度
{
	std::set<int> P{ 0 }; //第1-i个元素可以组成的和所组成的集合
	for (auto i = 0;i < input.size();i++)
	{
		auto temp = P;
		P = merge_lists(P, input[i]);
		delete_elem(P, sum);
		double delta = approx / (2 * input.size());
		P = trim(P, delta);
	}
	int max = 0;
	for (auto i = P.begin();i != P.end();i++)
	{
		if (*i > max)
		{
			max = *i;
		}
	}
	return max;
}

/*
调试代码：（以clrs P666的示例测试）
#include<iostream>
#include<string>
#include<vector>
#include<set>
#include"approx_subset_sum.h"

int main()
{
	std::vector<int> test{ 104,102,201,101 };
	std::cout << exact_subset_sum(test,308) << std::endl;
	std::cout << approx_subset_sum(test, 308, 0.4) << std::endl;
	return 0;
}
*/