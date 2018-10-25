#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>
#include<map>
#include"knuth_Morris_Pratt_algorithm.h"

std::vector<std::map<char,int>> compute_transition_function(std::string t,std::string p) //计算有限自动机的转移函数，转移函数的值k代表下一次比较时对p的第k个字符进行比较，前面k-1个字符都已经匹配成功
{
	std::string b = t;
	sort(b.begin(), b.end());
	auto c = unique(b.begin(), b.end());
	b.erase(c, b.end()); //将待匹配字符串排序并去重
	std::vector<std::map<char, int>> automaton(p.size() + 1);
	std::vector<int> prefix_func = compute_prefix_function(p); //计算前缀函数
	for (auto i = 0;i <= p.size();i++)
	{
		for (auto k : b)
		{
			if (i < p.size() && k == p[i]) //如果k与当前位置的字母相同，则转移到下一个字符
			{
				automaton[i][k] = i + 1;
			}
			else //如果k与当前位置的字母不同，或是k已经完成匹配，则需要根据前缀函数来判断跳转位置
			{
				int temp = i - 1; //默认0-i-1位置上已经匹配，i位置无法匹配，因此需要寻找i-1的前缀函数
				while (temp >= 0 && k != p[temp + 1]) //如果i-1位置的前缀仍无法匹配，则重复寻找
				{
					temp = prefix_func[temp];
				}
				if (k == p[temp + 1]) //如果匹配到则说明这一位置可以与第i个位置进行匹配
				{
					temp++;
				}
				automaton[i][k] = temp + 1; //保存下一个应进行比较的位置
			}
		}
	}
	return automaton;
}

void finite_automaton_matcher(std::string t, std::string p) //使用有限自动机进行匹配
{
	auto automaton = compute_transition_function(t,p);
	int a = 0;
	for (auto i = 0;i < t.size();i++)
	{
		a = automaton[a][t[i]];
		if (a == p.size())
		{
			std::cout << "Pattern occurs at shift " << i - a + 1 << std::endl;
		}
	}
}