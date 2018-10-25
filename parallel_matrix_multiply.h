#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<omp.h>

std::vector<std::vector<double>> operator+(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	if (a.begin()->size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The two matrix can not add." << std::endl;
		return{ { 0 } };
	}
	else
	{
#pragma omp parallel for
		for (int i = 0;i < a.size();i++)
		{
#pragma omp parallel for
			for (int j = 0;j < a.begin()->size();j++)
			{
				a[i][j] += b[i][j];
			}
		}
		return a;
	}
}
std::vector<std::vector<double>> operator-(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	if (a.begin()->size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The two matrix can not subtract." << std::endl;
		return{ { 0 } };
	}
	else
	{
#pragma omp parallel for
		for (int i = 0;i < a.size();i++)
		{
#pragma omp parallel for
			for (int j = 0;j < a.begin()->size();j++)
			{
				a[i][j] -= b[i][j];
			}
		}
		return a;
	}
}
std::vector<std::vector<double>> merge(std::vector<std::vector<double>> c11, std::vector<std::vector<double>> c12, std::vector<std::vector<double>> c21, std::vector<std::vector<double>> c22)
{
	std::vector<std::vector<double>> c(c11.size() * 2, std::vector<double>(c11.size() * 2, 0));
#pragma omp parallel for
	for (int i = 0;i < c11.size();i++)
	{
#pragma omp parallel for
		for (int j = 0;j < c11.size();j++)
		{
			c[i][j] = c11[i][j];
			c[i + c11.size()][j + c11.size()] = c22[i][j];
			c[i + c11.size()][j] = c21[i][j];
			c[i][j + c11.size()] = c12[i][j];
		}
	}
	return c;
}


std::vector<std::vector<double>> parallel_Strassen_algorithm(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b) //并行化的strassen算法，运行时需要保证a和b的大小都是2的幂指数
{
	if (a.size() != a.begin()->size() || b.size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The size of the martix can not match the algorithm." << std::endl;
		return{ { 0 } };
	}
	else
	{
		if (a.size() == 1 && b.size() == 1)
		{
			return{ { a[0][0] * b[0][0] } };
		}
		else
		{
			int size = a.size() / 2;
			std::vector<std::vector<double>> a11(size, std::vector<double>(size));
			std::vector<std::vector<double>> b11(size, std::vector<double>(size));
			std::vector<std::vector<double>> a22(size, std::vector<double>(size));
			std::vector<std::vector<double>> b22(size, std::vector<double>(size));
			std::vector<std::vector<double>> s1(size, std::vector<double>(size));
			std::vector<std::vector<double>> s2(size, std::vector<double>(size));
			std::vector<std::vector<double>> s3(size, std::vector<double>(size));
			std::vector<std::vector<double>> s4(size, std::vector<double>(size));
			std::vector<std::vector<double>> s5(size, std::vector<double>(size));
			std::vector<std::vector<double>> s6(size, std::vector<double>(size));
			std::vector<std::vector<double>> s7(size, std::vector<double>(size));
			std::vector<std::vector<double>> s8(size, std::vector<double>(size));
			std::vector<std::vector<double>> s9(size, std::vector<double>(size));
			std::vector<std::vector<double>> s10(size, std::vector<double>(size));
#pragma omp parallel for
			for (int i = 0;i < size;i++)
			{
#pragma omp parallel for
				for (int j = 0;j < size;j++)
				{
					a11[i][j] = a[i][j];
					a22[i][j] = a[i + size][j + size];
					b11[i][j] = b[i][j];
					b22[i][j] = b[i + size][j + size];
					s1[i][j] = b[i][j + size] - b[i + size][j + size];
					s2[i][j] = a[i][j] + a[i][j + size];
					s3[i][j] = a[i + size][j] + a[i + size][j + size];
					s4[i][j] = b[i + size][j] - b[i][j];
					s5[i][j] = a[i][j] + a[i + size][j + size];
					s6[i][j] = b[i][j] + b[i + size][j + size];
					s7[i][j] = a[i][j + size] - a[i + size][j + size];
					s8[i][j] = b[i + size][j] + b[i + size][j + size];
					s9[i][j] = a[i][j] - a[i + size][j];
					s10[i][j] = b[i][j] + b[i][j + size];
				}
			}
			std::vector<std::vector<double>> p1, p2, p3, p4, p5, p6, p7, c11, c12, c21, c22;
#pragma omp parallel sections
			{
#pragma omp section
			{
				p1 = parallel_Strassen_algorithm(a11, s1);
			}
#pragma omp section
			{
				p2 = parallel_Strassen_algorithm(s2, b22);
			}
#pragma omp section
			{
				p3 = parallel_Strassen_algorithm(s3, b11);
			}
#pragma omp section
			{
				p4 = parallel_Strassen_algorithm(a22, s4);
			}
#pragma omp section
			{
				p5 = parallel_Strassen_algorithm(s5, s6);
			}
#pragma omp section
			{
				p6 = parallel_Strassen_algorithm(s7, s8);
			}
#pragma omp section
			{
				p7 = parallel_Strassen_algorithm(s9, s10);
			}
			}
			c11 = p5 + p4 - p2 + p6;
			c12 = p1 + p2;
			c21 = p3 + p4;
			c22 = p5 + p1 - p3 - p7;
			return merge(c11, c12, c21, c22);
		}
	}
}

/*
测试主函数：（与串行相比，并行的执行速度提高了一倍）
#include<iostream>
#include<vector>
#include"parallel_matrix_multiply.h"
//#include"Strassen_algorithm.h"
#include<random>
#include<ctime>

int main()
{
	double start, end;
	std::uniform_real_distribution<double> rand;
	std::default_random_engine e(time(0));
	std::vector<std::vector<double>> test1(32, std::vector<double>(32, 0.0));
	std::vector<std::vector<double>> test2(32, std::vector<double>(32, 0.0));
	for (int i = 0;i < 32;i++)
	{
		for (int j = 0;j < 32;j++)
		{
			test1[i][j] = rand(e);
			test2[i][j] = rand(e);
		}
	}
	start = clock();
	auto i = parallel_Strassen_algorithm(test1, test2);
	//auto i = Strassen_algorithm(test1, test2);
	end = clock();
	std::cout << end - start << std::endl;
	return 0;
}
*/