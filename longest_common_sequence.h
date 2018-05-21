#pragma once
#include<iostream>
#include<tuple>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>

std::vector<std::vector<int>> solve_longest_common_subsequence(std::string a, std::string b)
{
	std::vector<std::vector<int>> result(a.size() + 1, std::vector<int>(b.size() + 1, 0));
	for (int i = 1;i <= a.size();i++)
	{
		for (int j = 1;j <= b.size();j++)
		{
			if (a[i - 1] == b[j - 1])
			{
				result[i][j] = result[i - 1][j - 1] + 1;
			}
			else
			{
				result[i][j] = result[i - 1][j] > result[i][j - 1] ? result[i - 1][j] : result[i][j - 1];
			}
		}
	}
	return result;
}

void print_longest_common_subsequence(std::vector<std::vector<int>> lcs_table, std::string a)
{
	std::string result;
	int j = lcs_table.begin()->size() - 1;
	int i = lcs_table.size() - 1;
	while (lcs_table[i][j] > 0)
	{
		if (lcs_table[i][j] == lcs_table[i - 1][j - 1] + 1 && lcs_table[i][j] > lcs_table[i][j - 1] && lcs_table[i][j] > lcs_table[i - 1][j])
		{
			result += a[i - 1]; //注意lcs_table是从1开始填入string对应的0位置元素的
			i--;
			j--;
		}
		else if (lcs_table[i][j] == lcs_table[i - 1][j])//结果不唯一，此处优先向上搜索
		{
			i--;
		}
		else if (lcs_table[i][j] == lcs_table[i][j - 1])//代表向左搜索成功
		{
			j--;
		}
	}
	for (int i = result.size() - 1;i >= 0;i--)
	{
		std::cout << result[i] << " ";
	}
	std::cout << std::endl;
}
