#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<omp.h>

template<typename T> int binary_search(T x, std::vector<T> A, int p, int r) //在数组A的p和r两下标之间搜索x
{
	int low = p;
	int high = p < r + 1 ? r + 1 : p;
	while (low < high)
	{
		int mid = (low + high) / 2;
		if (x <= A[mid])
		{
			high = mid;
		}
		else
		{
			low = mid + 1;
		}
	}
	return high;
}

template<typename T> void parallel_merge(std::vector<T> A, int p1, int r1, int p2, int r2, std::vector<T>& B, int p3) //p1,r1;p2,r2分别对应两个要归并的位置，p3对应归并之后的数组要存储的位置，A为原数组，B为归并之后的数组
{
	int n1 = r1 - p1 + 1;
	int n2 = r2 - p2 + 1;
	if (n1 < n2) //保证归并时n1>=n2
	{
		std::swap(p1, p2);
		std::swap(r1, r2);
		std::swap(n1, n2);
	}
	if (n1 == 0)
	{
		return;
	}
	else
	{
		int q1 = (p1 + r1) / 2;
		int q2 = binary_search(A[q1], A, p2, r2);//在另一个归并位置中搜索p1~r1的中位数在p2~r2中所对应的位置
		int q3 = p3 + (q1 - p1) + (q2 - p2); //计算归并之后，p1~r1中位数所应该在的位置
		B[q3] = A[q1]; //先将中位数复制过来
#pragma omp parallel sections //使用并行算法，将原数组A中的两个区域进行归并，并存储在数组B中
		{
#pragma omp section
		{
			parallel_merge(A, p1, q1 - 1, p2, q2 - 1, B, p3);
		}
#pragma omp section
		{
			parallel_merge(A, q1 + 1, r1, q2, r2, B, q3 + 1);
		}
		}
	}
}

template<typename T> void parallel_merge_sort(std::vector<T> A, int p, int r, std::vector<T>& B, int s) //A对应于即将做归并排序的数组，p和r表示归并的位置，B表示结果存储的矩阵，s表示归并结果在B中开始的位置
{
	int n = r - p + 1;
	if (n == 1)
	{
		B[s] = A[p];
	}
	else
	{
		std::vector<T> C(n);
		int q = (p + r) / 2;
		int q_pos = q - p + 1;
#pragma omp parallel sections
		{
#pragma omp section
		{
			parallel_merge_sort(A, p, q, C, 0);
		}
#pragma omp section
		{
			parallel_merge_sort(A, q + 1, r, C, q_pos);
		}
		}
		parallel_merge(C, 0, q_pos - 1, q_pos, n - 1, B, s);
	}
}

/*
测试代码：如果使用普通的merge_sort，待排序数组规模越大，效率比多线程的mergesort高的也越多，可能是因为多线程的mergesort要不断创建新数组/复制数组
#include<iostream>
#include<vector>
#include"parallel_merge_sort.h"
#include"merge_sort.h"
#include<random>
#include<ctime>

int main()
{
	double start, end;
	std::uniform_real_distribution<double> rand;
	std::default_random_engine e(time(0));
	std::vector<double> input, output(10000, 0.0);
	for (int i = 0;i <10000 ;i++)
	{
		input.push_back(rand(e));
	}
	start = clock();
	parallel_merge_sort(input, 0, 9999, output, 0);
	//merge_sort(input);
	end = clock();
	std::cout << end - start << std::endl;
	return 0;
}
*/