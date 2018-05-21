#pragma once
#include<iostream>
#include<vector>

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