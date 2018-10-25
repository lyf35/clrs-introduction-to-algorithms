#pragma once
#include<iostream>
#include<cmath>
#include<vector>

int modular_exponentiation(int a, int b, int n) //计算a^b(mod n)
{
	int d = 1;
	int s = a;
	int bit = 1;
	for (int i = 0;i <= floor(log2(b));i++) //代表程序要循环b以二进制表示的位数次。循环按照位数从低位到高位的顺序进行
	{
		if (b&bit) //代表b的二进制表示中，某一位上的数值为1，此时需要做相应的操作
		{
			d = (s*d) % n;
		}
		bit <<= 1;
		s = (s*s) % n; //对应于b的第i+1位上是a的2^i+1次方
	}
	return d;
}