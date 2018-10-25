#pragma once
#include<iostream>
#include<string>
#include<algorithm>
#include<cmath>
#include<vector>
#include"modular_exponentiation.h"

void Rabin_Karp_matcher(char* t, char* p) //利用“万物皆数”的思想，由于每一个字符在ASCII码中都有对应的int值，因此可以利用这一特点将字符串转化为一个数字来表示。p表示模式，t表示待匹配字符串
{
	int q = 4594591; //用于模除运算的质数
	int t_length = strlen(t);
	int p_length = strlen(p);
	std::vector<int> str_value(t_length - p_length + 1, 0); //用于保存转化为数字之后的字符串值
	int modular_exp_value = modular_exponentiation(128, p_length - 1, q); //计算最高位对应幂指数的值
	//计算字符串t的1-|p|位置所对应的数字以及字符串p所对应的数字，在计算时下标越小的位置对应于高位
	str_value[0] = int(t[0]);
	int p_value = int(p[0]);
	for (int i = 1;i < p_length;i++)
	{
		str_value[0] *= 128;
		str_value[0] += int(t[i]);
		str_value[0] %= q;
		p_value *= 128;
		p_value += int(p[i]);
		p_value %= q;
	}
	//然后计算字符串t后面位置所对应的数字，并同时进行检查
	for (int i = 1;i < t_length - p_length + 1; i++)
	{
		str_value[i] = ((128 * (str_value[i - 1] - modular_exp_value*int(t[i - 1])) + int(t[p_length + i - 1])) + q) % q;
		if (str_value[i] == p_value)
		{
			for (int j = 0;j < p_length;j++)
			{
				if (t[i + j] != p[j])
				{
					break;
				}
				if (j == p_length - 1 && t[i + j] == p[j])
				{
					std::cout << "Pattern occurs with shift " << i << std::endl;
				}
			}
		}
	}
} 