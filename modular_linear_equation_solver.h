#pragma once
#include<iostream>
#include<cmath>
#include<tuple>
#include"Euclid_algorithm.h"

int modular_linear_equation_solver(int a, int b, int n) //计算方程ax=b(mod n)的解。由方程可得ax-kn=b，如果方程有解，那么相当于b可以用a和n的线性组合表示。如果有解，返回值为最小的x；无解则返回-1
{
	auto gcd_an = extended_euclid(a, n); //如果b可以被gcd(a,n)整除，那么说明方程有解
	if (b%std::get<0>(gcd_an) == 0)
	{
		std::cout << "the solution for equation " << a << "x=" << b << "(mod " << n << ") is: ";
		for (int i = 0;i < n / std::get<0>(gcd_an);i++)
		{
			std::cout << (((std::get<1>(gcd_an)*(b / std::get<0>(gcd_an))) % n) + n) % n + i* n / std::get<0>(gcd_an) << " "; //此处要注意，有可能取余符号的左边是负数，所以要做一些处理
		}
		std::cout << std::endl;
		return (((std::get<1>(gcd_an)*(b / std::get<0>(gcd_an))) % n) + n) % n;
	}
	else //反之说明方程无解
	{
		std::cout << "equation " << a << "x=" << b << "(mod " << n << ") has no solution." << std::endl;
		return -1;
	}
}