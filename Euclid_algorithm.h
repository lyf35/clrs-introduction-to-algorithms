#pragma once
#include<iostream>
#include<cmath>
#include<tuple>

int euclid(int a, int b) //欧几里得算法，即用辗转相除法求两数的最大公约数
{
	if (b == 0)
	{
		return a;
	}
	else
	{
		return euclid(b, a%b);
	}
}

std::tuple<int, int, int> extended_euclid(int a, int b) //扩展版的欧几里得算法，返回值中第一个为最大公约数，第二个和第三个返回值中为ax+by=gcd(a,b)中x和y的值
{
	if (b == 0)
	{
		return std::make_tuple(a, 1, 0); //递归的基础情形
	}
	else
	{
		std::tuple<int, int, int> temp = extended_euclid(b, a%b); //第一个返回值为gcd(a,b)，后两个返回值为bx+(a%b)y=gcd(a,b)中的x和y
		std::tuple<int, int, int> temp2 = std::make_tuple(std::get<0>(temp), std::get<2>(temp),std::get<1>(temp)-std::get<2>(temp)*floor(a/b)); //根据temp的返回值，来推断ax'+by'=gcd(a,b)中的x'和y'。与上式联立，可解得：x'=y，y'=x-y*floor(a/b)
		return temp2;
	}
}

/*
测试主函数，计算99和78的gcd以及ax+by=gcd(a,b)的表达式
int main()
{
	std::cout << euclid(99, 78) << std::endl;
	auto i = extended_euclid(99, 78);
	std::cout << std::get<0>(i) << '\t' << std::get<1>(i) << '\t' << std::get<2>(i) << std::endl;
	return 0;
}
*/