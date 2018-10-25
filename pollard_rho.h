#pragma once
#include<iostream>
#include<cmath>
#include<random>
#include<ctime>
#include<set>
#include"Euclid_algorithm.h"

std::set<int> pollard_rho(int n) //求整数因子分解的程序
{
	std::set<int> result;
	std::uniform_int_distribution<int> rand(0, n - 1);
	std::default_random_engine e(time(0));
	int i = 1;
	int x = rand(e);
	int y = x;
	int k = 2;
	int temp = result.size();
	int count = 10000;//设置当循环1000步时，set的元素个数还没变化，就停止并输出。但是这样也无法保证结果完全正确，需要多运行几次
	while (true)
	{
		i++;
		x = (x*x - 1) % n;
		int d = euclid(abs(y - x), n);
		if (d != 1 && d != n)
		{
			result.insert(d);
			if (result.size() == temp) //判断result中是否添加了新元素
			{
				count--;
				if (count == 0)
				{
					return result;
				}
			}
			else
			{
				temp = result.size();
			}
		}
		if (i == k)
		{
			y = x;
			k *= 2;
		}
	}
}