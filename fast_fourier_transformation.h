#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<complex>

const double pi = 3.14159265358979323846;

std::vector<unsigned int> bit_reverse(unsigned int n)//构造一个长度为2^ceil(lg n)的按位逆序数组。用核算法分析可知时间复杂度为O(n)
{
	unsigned bit_size = ceil(log(n)/log(2)); //将n扩大为2的幂所需要的最小指数
	unsigned int arr_size = pow(2, bit_size); //arr_size的数值就是10……0（n个0）
	std::vector<unsigned int> rev(arr_size, 0); //保存逆序后的数值
	unsigned rev_num = 0; //存储二进制位逆序后的数值
	for (unsigned int i = 1;i < arr_size;i++)
	{
		unsigned temp = 1 << bit_size - 1; //1后面n-1个0，代表逆序数值的最低位
		while ((temp&rev_num) != 0) //从最高位开始判定，如果遇到某一位上是1，则需要向低位不断进位，直至遇到某一位为0才停止
		{
			rev_num ^= temp;
			temp >>= 1;
		}
		rev_num ^= temp; 
		rev[i] = rev_num; //将加1后的逆序值存入数组中
	}
	return rev;
}

std::vector<std::complex<double>> fast_fourier_transformation(std::vector<std::complex<double>> input) //fft的代码，默认输出会自动将数组规模扩充至最接近的2的指数幂
{
	const unsigned arr_size = input.size();
	const unsigned output_size = pow(2, ceil(log(arr_size) / log(2)));
	std::vector<std::complex<double>> output(output_size, 0); //存储fft的结果
	std::vector<unsigned> temp = bit_reverse(arr_size); //存储位逆序之后的结果
	for (unsigned i = 0;i < output_size;i++) //将输入数组中的元素按照位逆序的次序存入输出数组中
	{
		if (temp[i] < arr_size)
		{
			output[i] = input[temp[i]]; 
		}
	}
	for (unsigned i = 0;i < ceil(log(arr_size) / log(2));i++) //根据数组大小决定需要蝴蝶操作的层数
	{
		std::complex<double> omega(cos(2 * pi / pow(2, i + 1)), sin(2 * pi / pow(2, i + 1))); //每一层蝴蝶操作使用的omega_n值
		unsigned num = pow(2, i);//代表第i层蝴蝶操作中每一组数据中的元素个数的一半
		std::vector<std::complex<double>> omega_m(num, 0);//蝴蝶操作中使用的omega_n^m值
		omega_m[0] = 1; 
		for (unsigned m = 1;m < num;m++)
		{
			omega_m[m] = omega_m[m-1]*omega;
		}
		for (unsigned j = 0;j < output_size;j += pow(2, i + 1))//代表数组中的元素在第i层蝴蝶操作时可以被分成的组数
		{
			for (unsigned k = 0;k < num;k++)//代表每一组中需要做2^i次循环
			{
				auto temp1 = output[j + k] + omega_m[k] * output[j + k + num];
				auto temp2 = output[j + k] - omega_m[k] * output[j + k + num];
				output[j + k] = temp1;
				output[j + k + num] = temp2;
			}
		}
	}
	return output;
}

std::vector<std::complex<double>> fast_fourier_transformation_recursive(std::vector<std::complex<double>> input) //递归版本的fft，效率比迭代版本要低
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
	y0 = fast_fourier_transformation_recursive(a0);
	y1 = fast_fourier_transformation_recursive(a1);
	for (int i = 0;i < n / 2;i++)
	{
		y[i] = y0[i] + omega[i] * y1[i];
		y[i + n / 2] = y0[i] - omega[i] * y1[i];
	}
	return y;
}

std::vector<std::complex<double>> inverse_fast_fourier_transformation(std::vector<std::complex<double>> input) //逆fft的代码，默认输出会自动将数组规模扩充至最接近的2的指数幂
{
	const unsigned arr_size = input.size();
	const unsigned output_size = pow(2, ceil(log(arr_size) / log(2)));
	std::vector<std::complex<double>> output(output_size, 0); //存储ifft的结果
	std::vector<unsigned> temp = bit_reverse(arr_size); //存储位逆序之后的结果
	for (unsigned i = 0;i < output_size;i++) //将输入数组中的元素按照位逆序的次序存入输出数组中
	{
		if (temp[i] < arr_size)
		{
			output[i] = input[temp[i]];
		}
	}
	for (unsigned i = 0;i < ceil(log(arr_size) / log(2));i++) //根据数组大小决定需要蝴蝶操作的层数
	{
		std::complex<double> omega(cos(2 * pi / pow(2, i + 1)), -sin(2 * pi / pow(2, i + 1))); //每一层蝴蝶操作使用的-omega_n值
		unsigned num = pow(2, i);//代表第i层蝴蝶操作中每一组数据中的元素个数的一半
		std::vector<std::complex<double>> omega_m(num, 0);//蝴蝶操作中使用的-omega_n^m值
		omega_m[0] = 1;
		for (unsigned m = 1;m < num;m++)
		{
			omega_m[m] = omega_m[m - 1] * omega;
		}
		for (unsigned j = 0;j < output_size;j += pow(2, i + 1))//代表数组中的元素在第i层蝴蝶操作时可以被分成的组数
		{
			for (unsigned k = 0;k < num;k++)//代表每一组中需要做2^i次循环
			{
				auto temp1 = output[j + k] + omega_m[k] * output[j + k + num];
				auto temp2 = output[j + k] - omega_m[k] * output[j + k + num];
				output[j + k] = temp1;
				output[j + k + num] = temp2;
			}
		}
	}
	for (unsigned i = 0;i < output_size;i++)
	{
		output[i] /= output_size;
	}
	return output;
}

/*
调试用代码（clrs练习题30.3-1）：
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<random>
#include<complex>
#include"fast_fourier_transformation.h"

int main()
{
	std::vector<std::complex<double>> test{ std::complex<double>(0.0,0.0),std::complex<double>(2.0,0.0),std::complex<double>(3.0,0.0),std::complex<double>(-1.0,0.0),std::complex<double>(4.0,0.0),std::complex<double>(5.0,0.0),std::complex<double>(7.0,0.0),std::complex<double>(9.0,0.0) };
	auto result = fast_fourier_transformation(test);
	for (auto i : result)
	{
		std::cout << i << std::endl;
	}
	std::vector<std::complex<double>> test2{ std::complex<double>(29, 0),std::complex<double>(0.949747, -13.1924),std::complex<double>(-6, -1),std::complex<double>(-8.94975, -5.19239),std::complex<double>(-1, 0),std::complex<double>(-8.94975, 5.19239),std::complex<double>(-6, 1),std::complex<double>(0.949747, 13.1924) };
	std::cout << std::endl;
	auto result2 = inverse_fast_fourier_transformation(test2);
	for (auto i : result2)
	{
		std::cout << i << std::endl;
	}
	return 0;
}
*/