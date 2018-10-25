#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<numeric>
#include<set>
#include<algorithm>

class linear_programming
{
private:
	/*
	符号所对应的含义：
	集合N中的元素代表非基本变量，大小为n
	集合B中的元素代表基本变量，大小为m
	矩阵A中的第i行对应第i个变量。如果全是0，代表这一行对应的变量是非基本变量；如果有非0元素，那么这一行对应于第i个基本变量，代表线性规划松弛型中的一个约束条件，这一行的元素对应非基本变量的系数。A的规模为(n+m)*(n+m)
	向量b中第i个元素对应第i个基本变量对应约束中的常数项，规模为（n+m）
	向量c中第i个元素对应待优化表达式中第i个变量的系数，规模为（n+m）
	数值v代表待优化表达式中的常数项
	向量result代表这个线性规划的一组可行解，如果全部都是double_max代表这个线性规划的解无界
	*/
	std::set<int> N;
	std::set<int> B;
	std::vector<std::vector<double>> A;
	std::vector<double> b;
	std::vector<double> c;
	double v;
	std::vector<double> result;

public:
	linear_programming(std::set<int> N_input, std::set<int> B_input, std::vector<std::vector<double>> A_input, std::vector<double> b_input, std::vector<double> c_input) :N(N_input), B(B_input), A(A_input), b(b_input), c(c_input), v(0), result(c_input.size(),0) {};

	linear_programming initialize_simplex() //对线性规划进行初始化，使其可以用于单纯形算法
	{
		double min = LDBL_MAX; 
		int num = 0; //保存最小约束值对应的序号
		for (int k = 0;k < b.size();k++) //选取所有约束值中的最小值以及其对应的序号
		{
			if (b[k] < min)
			{
				min = b[k];
				num = k;
			}
		}
		if (min >= 0) //如果基础的松弛型可解，则直接返回即可
		{
			return *this;
		}
		linear_programming aux = build_auxillary_linear_programming(); //如果不可解，则需要求解辅助线性规划
		aux.pivot(num, A.size());
		aux.simplex(); //对辅助线性规划调用单纯形算法求解
		if (abs(aux.result[aux.result.size() - 1]) <= 1e-10) //在辅助线性规划的最优解中，如果辅助变量的值为0，说明这个线性规划有可行解，根据辅助线性规划的结果对原线性规划进行修改
		{
			if (aux.B.find(aux.result.size() - 1) != aux.B.end()) //如果辅助变量是基本变量，则需要执行一次转动操作将辅助变量变为非基本变量。可以选择任意一个非基本变量作为被替换者
			{
				aux.pivot(aux.result.size() - 1, *(N.begin()));
			}
			for (int i = 0;i < A.size();i++) //把辅助线性规划的约束条件赋给原始线性规划，并将其中的辅助变量全部去掉
			{
				for (int j = 0;j < A.size();j++)
				{
					A[i][j] = aux.A[i][j];
				}
				b[i] = aux.b[i];
			}
			B = aux.B; //将辅助线性规划的基本变量与非基本变量也复制过来
			N = aux.N;
			N.erase(N.find(aux.result.size() - 1));
			for (int i = 0;i < A.size();i++)//需要对原始的目标优化函数也做一定的修改
			{
				if (c[i] != 0 && B.find(i) != B.end()) //如果目标优化函数中存在基本变量，需要将其替换掉
				{
					for (int j = 0;j < A.size();j++)
					{
						c[j] -= A[i][j] * c[i];
					}
					v += b[i]*c[i];
					c[i] = 0;
				}
			}
		}
		else //如果辅助变量的值不为0，说明辅助线性规划无解。给result向量中的所有元素赋值为双精度浮点数的最小值，代表无解
		{
			for (auto i = result.begin();i != result.end();i++)
			{
				*i = LDBL_MIN;
			}
		}
		return *this;
	}

	linear_programming pivot(int l, int e)//l对应的是一个基本变量的序号，e对应的是一个非基本变量的序号，通过pivot函数将e变为基本变量，l变为非基本变量。几何上相当于从凸区域的一个顶点旋转至另一个顶点
	{
		std::vector<std::vector<double>> A_mod(A.size(), std::vector<double>(A.begin()->size(), 0));
		std::vector<double> b_mod(b.size(), 0);
		std::vector<double> c_mod(c.size(), 0);
		//先计算第e个变量（即将变为基本变量）所对应的约束表达式
		b_mod[e] = b[l] / A[l][e];
		for (auto i = N.begin();i != N.end();i++) 
		{
			if (*i != e)
			{
				A_mod[e][*i] = A[l][*i] / A[l][e];
			}
		}
		A_mod[e][l] = 1 / A[l][e];
		//再对其余的约束进行修改，即把第e个元素对应的表达式代入到其它的约束条件中进行消元
		for (auto i = B.begin();i != B.end();i++)
		{
			if (*i != l)
			{
				b_mod[*i] = b[*i] - A[*i][e] * b_mod[e];
				for (auto j = N.begin();j != N.end();j++)
				{
					if (*j != e)
					{
						A_mod[*i][*j] = A[*i][*j] - A[*i][e] * A_mod[e][*j];
					}
				}
				A_mod[*i][l] = -A[*i][e] * A_mod[e][l];
			}
		}
		//修改目标优化函数
		v += c[e] * b_mod[e];
		for (auto i = N.begin();i != N.end();i++)
		{
			if (*i != e)
			{
				c_mod[*i] = c[*i] - c[e] * A_mod[e][*i];
			}
		}
		c_mod[l] = -c[e] * A_mod[e][l];
		//对集合中的元素做修改
		N.erase(N.find(e));
		N.insert(l);
		B.erase(B.find(l));
		B.insert(e);
		A = A_mod;
		b = b_mod;
		c = c_mod;
		return *this;
	}

	linear_programming simplex() //求解单纯形算法的程序。执行这个函数之前要先执行一次initialize_simplex()函数来确定线性规划是否有可行解
	{
		if (result[0] == LDBL_MIN)
		{
			std::cout << "infeasible" << std::endl;
			return *this;
		}
		std::vector<double> temp(c.size(), LDBL_MAX); 
		auto i = N.begin();
		while(i != N.end())
		{
			temp = std::vector<double>(c.size(), LDBL_MAX);
			if (c[*i] > 0) //寻找待优化表达式中系数为正数的变量i
			{
				for (auto j = B.begin();j != B.end();j++) //计算每一个约束条件的中变量i可以取的最大值
				{
					if (A[*j][*i] > 0)
					{
						temp[*j] = b[*j] / A[*j][*i];
					}
				}
				double min = LDBL_MAX; //用于保存约束值
				int num = 0; //保存最小约束值对应的序号
				for (auto k = B.begin();k != B.end();k++) //选取所有约束值中的最小值以及其对应的序号
				{
					if (temp[*k] < min)
					{
						min = temp[*k];
						num = *k;
					}
				}
				if (min == LDBL_MAX) //如果约束值中的最小值为无穷大，那么说明这个线性规划无界，可以直接返回
				{
					std::cout << "Unbounded" << std::endl;
					for_each(result.begin(), result.end(), [](double i) {i = LDBL_MAX;});
					return *this;
				}
				else //如果最小值不是无穷大，那么对原松弛型做转动操作
				{
					pivot(num, *i);
					i = N.begin(); //由于转动之后的集合发生改变，因此需要重新对i进行赋值，并直接进行新一步的循环
					continue;
				}
			}
			i++;
		}
		for (int i = 0;i < c.size();i++)
		{
			if (B.find(i) != B.end())
			{
				result[i] = b[i];
			}
			else
			{
				result[i] = 0;
			}
		}
		return *this;
	}

	linear_programming build_auxillary_linear_programming() //根据原始线性规划构造一个辅助线性规划，并设m+n为新变量的编号
	{
		std::set<int> N_aux = N;
		std::set<int> B_aux = B;
		std::vector<std::vector<double>> A_aux = A;
		std::vector<double> b_aux = b;
		std::vector<double> c_aux(A.size() + 1, 0);
		N_aux.insert(c.size());
		for (auto i = A_aux.begin();i != A_aux.end();i++)
		{
			i->push_back(0);
		}
		for (auto i = B.begin();i != B.end();i++)
		{
			A_aux[*i][A_aux.begin()->size() - 1] = -1;
		}
		A_aux.push_back(std::vector<double>(A_aux.begin()->size(), 0));
		b_aux.push_back(0);
		c_aux[A.size()] = -1;
		linear_programming aux(N_aux, B_aux, A_aux, b_aux, c_aux);
		return aux;
	}

	void output_result(std::ostream& os)
	{
		for (auto i : result)
		{
			os << i << '\t';
		}
		os << std::endl;
	}
};

/*
测试：
clrs P507公式(29.53)-(29.57)所对应的线性规划：
std::set<int> N = { 0,1,2 };
std::set<int> B = { 3,4,5 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0,0 },{ 0,0,0,0,0,0 },{ 0,0,0,0,0,0 },{ 1,1,3,0,0,0 },{ 2,2,5,0,0,0 },{ 4,1,2,0,0,0 } };
std::vector<double> b = { 0,0,0,30,24,36 };
std::vector<double> c = { 3,1,2,0,0,0 };

clrs P524 练习29.5-5 对应线性规划有一个有限目标值最优解的情况
std::set<int> N = { 0,1 };
std::set<int> B = { 2,3,4 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 1,-1,0,0,0 },{ -1,-1,0,0,0 },{ -1,4,0,0,0 } };
std::vector<double> b = { 0,0,8,-3,2 };
std::vector<double> c = { 1,3,0,0,0 };

clrs P524 练习29.5-6 对应线性规划无可行解的情况
std::set<int> N = { 0,1 };
std::set<int> B = { 2,3,4 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ 1,2,0,0,0 },{ -2,-6,0,0,0 },{ 0,1,0,0,0 } };
std::vector<double> b = { 0,0,4,-12,1 };
std::vector<double> c = { 1,-2,0,0,0 };

clrs P524 练习29.5-7 对应线性规划无界的情况
std::set<int> N = { 0,1 };
std::set<int> B = { 2,3,4 };
std::vector<std::vector<double>> A = { { 0,0,0,0,0 },{ 0,0,0,0,0 },{ -1,1,0,0,0 },{ -1,-1,0,0,0 },{ -1,4,0,0,0 } };
std::vector<double> b = { 0,0,-1,-3,2 };
std::vector<double> c = { 1,3,0,0,0 };

#include<iostream>
#include<vector>
#include<set>
#include"simplex_algorithm.h"

int main()
{
	//将上述的变量粘贴在此处
	linear_programming test(N, B, A, b, c);
	test.initialize_simplex();
	test.simplex();
	test.output_result(std::cout);
	return 0;
}
*/