#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>

std::vector<int> compute_prefix_function(std::string a) //计算前缀函数pi[i]=k，代表某个位置上字符串可以按照从后往前的顺序与某个前缀相匹配，即a[i-k]~a[i]=a[0]~a[k]，且这一前缀必须严格小于当前位置的长度
{
	std::vector<int> prefix_pos(a.size(), 0);
	int temp = -1; //记录匹配前缀字符串的长度，-1代表没有匹配任何前缀字符串
	prefix_pos[0] = temp;
	for (auto i = 1;i < a.size();i++)
	{
		while (a[temp + 1] != a[i] && temp>=0) //在这一步中，temp记录的是上一个位置的前缀字符串对应位置。如果前缀字符串的下一位置无法与当前位置匹配，则需要跳转至上一位置前缀字符串的前缀字符串继续检查 
		{
			temp = prefix_pos[temp];
		}
		if (a[temp + 1] == a[i]) //while循环结束，这一步相当于是判断因为何原因结束跳转
		{
			temp++;
		}
		prefix_pos[i] = temp;
	}
	return prefix_pos;
}

void kmp_matcher(std::string t, std::string p) //用前缀函数来辅助匹配
{
	std::vector<int> prefix_func = compute_prefix_function(p);
	int pos = -1;
	for (size_t i = 0; i < t.length(); i++)
	{
		while (t[i] != p[pos+1] && pos >= 0) //注意pos仍保存的是上一位中p的匹配位置，i对应的是当前位置。如果无法匹配则需要跳转
		{
			pos = prefix_func[pos];
		}
		if (t[i] == p[pos+1])
		{
			pos++;
		}
		if (pos == p.size() - 1)
		{
			std::cout << "Pattern occurs at shift " << i - p.size() + 1 << std::endl;
			pos = prefix_func[pos]; //当前位置已经匹配完成，因此在比较下一个字符的之前，可以跳转至这一位置的前缀
		}
	} 
}