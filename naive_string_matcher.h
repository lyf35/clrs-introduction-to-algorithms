#pragma once
#include<iostream>
#include<string>
#include<algorithm>

void naive_string_matcher(std::string t, std::string p) //朴素字符串匹配算法，即每次只移动一位。p代表模式字符串，t代表文本字符串
{
	int p_length = p.size();
	int t_length = t.size();
	for (auto i = 0;i < t_length - p_length + 1;i++)
	{
		for (auto j = 0;j < p_length;j++)
		{
			if (p[j] != t[j + i])
			{
				break;
			}
			if (j == p_length - 1 && p[j] == t[j + i])
			{
				std::cout << "The pattern occurs with shift " << i << std::endl;
			}
		}
	}
}