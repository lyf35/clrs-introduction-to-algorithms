#pragma once
#include<iostream>
#include<cmath>
#include<tuple>
#include<vector>
#include"modular_linear_equation_solver.h"

int Chinese_remainder_theorem(std::vector<int> a, std::vector<int> n) //向量a和b的第i个位置对应了第i个方程，返回值为一个最小值
{
	std::vector<int> m(n.size(), 1); //m_i=n_0*n_1*……*n_i-1*n_i+1*……*n_k
	int n_tot = 1; //n=n_0*n_1*……*n_k
	for (auto i = 0;i < m.size();i++)
	{
		for (auto j = 0;j < m.size();j++)
		{
			if (i != j)
			{
				m[i] *= n[j];
			}
		}
		n_tot *= n[i];
	}
	std::vector<int> c(n.size(), 0); //c中保存m_i的乘法逆元
	for (auto i = 0;i < n.size();i++)
	{
		c[i] = modular_linear_equation_solver(m[i], 1, n[i]);
	}
	int a_tot = 0; //计算a=a[i]*c[i]的累加和
	for (auto i = 0;i < n.size();i++)
	{
		a_tot += a[i] * c[i];
	}
	std::cout << "the solution of the equation set is: " << a_tot%n_tot << " + i * " << n_tot << std::endl;
	return a_tot%n_tot;
}