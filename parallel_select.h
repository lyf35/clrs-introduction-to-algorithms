#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<omp.h>

//一个并行的选择算法（参照clrs9.3），返回一个数组中第i大的数（i=1~n）
template<typename T> size_t partition(std::vector<T>& vec, int p, int r, T x) //修改过的划分算法，以x的值为pivot，对数组的p~r位置进行划分
{
	int i = p - 1;
	for (auto j = p;j <= r;j++)
	{
		if (vec[j] <= x)
		{
			i++;
			std::swap(vec[i], vec[j]);
		}
	}
	return i + 1; //返回值为partition之后大于x的第一个数的位置
}

template<typename T> T find_median(std::vector<T> input, int i, int j) //寻找i~j中的中位数，使用插入排序即可。此外，如果j-i+1为偶数，程序每次返回中位数中较小者
{
	if (i == j)
	{
		return input[i];
	}
	for (int k = i + 1;k <= j;k++)
	{
		T temp = input[k];
		int m = k - 1;
		while (m >=i&&input[m] > temp)
		{
			input[m + 1] = input[m];
			m--;
		}
		input[m + 1] = temp;
	}
	return input[floor((i + j) / 2.0)];
}

template<typename T> T parallel_select(std::vector<T> input, int i, int j, int r)//返回input中第r小的元素，input还没有排好序
{
	int n = j - i + 1;
	if (j == i)
	{
		return input[i];
	}
	else
	{
		int size = ceil(double(n) / 5.0) ;
		std::vector<T> M(size);
#pragma omp parallel for
		for (int k = 0;k < size;k++)
		{
			M[k] = find_median(input, k * 5 + i, i + (k + 1) * 5 - 1 >= n - 1 + i ? n - 1 + i : i + (k + 1) * 5 - 1);
		}
		T x = parallel_select(M, 0, size - 1, size / 2); //寻找M中的中位数
		int k = partition(input, i, j, x); //按照x的大小，对原数组做划分，返回值为i,j中大于x的第一个数的位置，即挑出的x值在整个数组里面的排序序号
		if (k == r)
		{
			return x;
		}
		else
		{
			if (k > r)
			{
				return parallel_select(input, i, k - 1, r);
			}
			else
			{
				return parallel_select(input, k, j, r);
			}
		}
	}
}


/*
测试代码：
#include<iostream>
#include<vector>
#include"parallel_select.h"
#include<random>
#include<ctime>

int main()
{
	std::vector<double> input;
	std::uniform_real_distribution<double> rand;
	std::default_random_engine e(time(0));
	for (int i = 0;i <10000;i++)
	{
		input.push_back(rand(e));
	}
	auto i = parallel_select(input, 0, 9999, 100);
	std::cout << i << std::endl;
	return 0;
}
*/