#pragma once
#include<complex>
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<omp.h>

const double pi = 3.14159265358979323846;

std::vector<std::complex<double>> parallel_fast_fourier_transform(std::vector<std::complex<double>> input)
{
	int n = input.size();
	if (n == 1)
	{
		return input;
	}
	std::complex<double> omega_n(cos(2 * pi / n), sin(2 * pi / n));
	std::vector<std::complex<double>> a0, a1, y0, y1, omega(1, (1, 0)), y(n, 0);
	for (auto i = 1;i < n / 2;i++)
	{
		omega.emplace_back(omega[i - 1] * omega_n);
	}
	for (auto i = 0;i < n;i += 2)
	{
		a0.push_back(input[i]);
		a1.push_back(input[i + 1]);
	}
#pragma omp parallel sections
	{
#pragma omp section
	{
		y0 = parallel_fast_fourier_transform(a0);
	}
#pragma omp section
	{
		y1 = parallel_fast_fourier_transform(a1);
	}
	}
#pragma omp parallel for
	for (int i = 0;i < n / 2;i++)
	{
		y[i] = y0[i] + omega[i] * y1[i];
		y[i + n / 2] = y0[i] - omega[i] * y1[i];
	}
	return y;
}

/*
调试代码：（相比于普通的迭代版本fft，并行算法所需时间差不多可以节省一半，但是即使并行之后仍不如迭代算法快）
#include<iostream>
#include<vector>
#include"parallel_fast_fourier_transformation.h"
//#include"fast_fourier_transformation.h"
#include<random>
#include<ctime>

int main()
{
	double start, end;
	std::uniform_real_distribution<double> rand(-10, 10);
	std::default_random_engine e(time(0));
	std::vector<std::complex<double>> input;
	for (int i = 0;i <65536 ;i++)
	{
		input.emplace_back(rand(e), rand(e));
	}
	start = clock();
	auto result = parallel_fast_fourier_transform(input);
	//auto result = fast_fourier_transformation_recursive(input);
	end = clock();
	std::cout << end - start << std::endl;
	return 0;
}
*/