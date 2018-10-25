#pragma once
#include<iostream>
#include<cmath>
#include<random>
#include<ctime>
#include"modular_exponentiation.h"

bool witness(int a, int n) //寻找n为合数的“证据”，判断a是否为“证据”
{
	int t = 1;
	int u = n - 1;
	while (u % 2 == 0)
	{
		u /= 2;
		t++;
	}
	int x = modular_exponentiation(a, u, n);
	for (int i = 1;i <= t;i++)
	{
		int temp = (x*x) % n;
		if (temp == 1 && x != 1 && x != n - 1) //如果n是质数，那么x^2=1 (mod n) 的解是平凡的，即±1
		{
			return true;
		}
		x = temp;
	}
	if (x != 1) //如果n是质数，根据费马定理一定有 a^n-1 (mod n)=1
	{
		return true;
	}
	return false;
}

bool miller_rabin(int n, int s) //miller-rabin素数测试的程序，n代表待测试的数，s代表测试的次数。返回值为1代表合数，0代表质数
{
	std::uniform_int_distribution<int> rand(1, n - 1);
	std::default_random_engine e(time(0));
	for (int j = 1;j <= s; j++)
	{
		int a = rand(e);
		if (witness(a, n))
		{
			return 1;
		}
	}
	return 0;
}