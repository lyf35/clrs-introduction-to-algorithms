#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>

std::tuple < std::vector<double>::iterator, std::vector<double>::iterator, double > find_max_crossing_subarray(std::vector<double>::iterator beg, std::vector<double>::iterator end)
{
	double sum = 0;
	double cnt = 0;
	std::vector<double>::iterator left, right;
	int pos = (end - beg) / 2;
	while (pos >= 0)
	{
		cnt += *(beg + pos);
		if (cnt > sum)
		{
			sum = cnt;
			left = beg + pos;
		}
		pos--;
	}
	pos = (end - beg) / 2 + 1;
	cnt = sum;
	while (beg + pos < end)
	{
		cnt += *(beg + pos);
		if (cnt > sum)
		{
			sum = cnt;
			right = beg + pos;
		}
		pos++;
	}
	return std::make_tuple(left, right, sum);
}
std::tuple < std::vector<double>::iterator, std::vector<double>::iterator, double > find_max_subarray(std::vector<double>::iterator beg, std::vector<double>::iterator end)
{
	if (beg == end)
	{
		return std::make_tuple(beg, beg, 0);
	}
	if (beg + 1 == end)
	{
		return std::make_tuple(beg, beg, *beg);
	}
	else
	{
		auto mid = beg + (end - beg) / 2;
		auto leftinfo = find_max_subarray(beg, mid);
		auto rightinfo = find_max_subarray(mid + 1, end);
		auto crossinfo = find_max_crossing_subarray(beg, end);
		if (std::get<2>(leftinfo) >= std::get<2>(rightinfo) && std::get<2>(leftinfo) >= std::get<2>(crossinfo))
		{
			return leftinfo;
		}
		else if (std::get<2>(crossinfo) >= std::get<2>(rightinfo) && std::get<2>(crossinfo) >= std::get<2>(leftinfo))
		{
			return crossinfo;
		}
		else
		{
			return rightinfo;
		}
	}
}
std::tuple < std::vector<double>::iterator, std::vector<double>::iterator, double > find_max_subarray_exhaustion(std::vector<double>::iterator beg, std::vector<double>::iterator end)
{
	std::vector<double>::iterator left, right;
	double sum = 0;
	if (beg == end)
	{
		return std::make_tuple(beg, beg, 0);
	}
	else
	{
		for (int i = 0;i < end - beg;i++)
		{
			double temp = 0;
			std::vector<double>::iterator temp_left = beg + i;
			for (int j = i;j < end - beg;j++)
			{
				temp += *(beg + j);
				if (temp > sum)
				{
					left = temp_left;
					right = beg + j;
					sum = temp;
				}
			}
		}
		return std::make_tuple(left, right, sum);
	}
}

std::vector<std::vector<double>> operator+(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	if (a.begin()->size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The two matrix can not add." << std::endl;
		return { { 0 } };
	}
	else
	{
		for (int i = 0;i < a.size();i++)
		{
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
		for (int i = 0;i < a.size();i++)
		{
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
	for (int i = 0;i < c11.size();i++)
	{
		for (int j = 0;j < c11.size();j++)
		{
			c[i][j]= c11[i][j];
			c[i + c11.size()][j + c11.size()]= c22[i][j];
			c[i + c11.size()][j] = c21[i][j];
			c[i][j + c11.size()] = c12[i][j];
		}
	}
	return c;
}
std::vector<std::vector<double>> Strassen_algorithm(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b)
{
	if (a.size() != a.begin()->size() || b.size() != b.begin()->size() || a.size() != b.size())
	{
		std::cout << "The size of the martix can not match the algorithm." << std::endl;
		return{ {0} };
	}
	else
	{
		if (a.size() == 1 && b.size() == 1)
		{
			return{ {a[0][0] * b[0][0]} };
		}
		else
		{
			std::vector<std::vector<double>> a11(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> b11(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> a22(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> b22(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s1(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s2(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s3(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s4(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s5(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s6(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s7(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s8(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s9(a.size() / 2, std::vector<double>(a.size() / 2));
			std::vector<std::vector<double>> s10(a.size() / 2, std::vector<double>(a.size() / 2));
			for (int i = 0;i < a.size() / 2;i++)
			{
				for (int j = 0;j < a.size() / 2;j++)
				{
					a11[i][j] = a[i][j];
					a22[i][j] = a[i + a.size() / 2][j + a.size() / 2];
					b11[i][j] = b[i][j];
					b22[i][j] = b[i + a.size() / 2][j + a.size() / 2];
					s1[i][j] = b[i][j + a.size() / 2] - b[i + a.size() / 2][j + a.size() / 2];
					s2[i][j] = a[i][j] + a[i][j + a.size() / 2];
					s3[i][j] = a[i + a.size() / 2][j] + a[i + a.size() / 2][j + a.size() / 2];
					s4[i][j] = b[i + a.size() / 2][j] - b[i][j];
					s5[i][j] = a[i][j] + a[i + a.size() / 2][j + a.size() / 2];
					s6[i][j] = b[i][j] + b[i + a.size() / 2][j + a.size() / 2];
					s7[i][j] = a[i][j + a.size() / 2] - a[i + a.size() / 2][j + a.size() / 2];
					s8[i][j] = b[i + a.size() / 2][j] + b[i + a.size() / 2][j + a.size() / 2];
					s9[i][j] = a[i][j] - a[i + a.size() / 2][j];
					s10[i][j] = b[i][j] + b[i][j + a.size() / 2];
				}
			}
			auto p1 = Strassen_algorithm(a11, s1);
			auto p2 = Strassen_algorithm(s2, b22);
			auto p3 = Strassen_algorithm(s3, b11);
			auto p4 = Strassen_algorithm(a22, s4);
			auto p5 = Strassen_algorithm(s5, s6);
			auto p6 = Strassen_algorithm(s7, s8);
			auto p7 = Strassen_algorithm(s9, s10);
			auto c11 = p5 + p4 - p2 + p6;
			auto c12 = p1 + p2;
			auto c21 = p3 + p4;
			auto c22 = p5 + p1 - p3 - p7;
			return merge(c11, c12, c21, c22);
		}
	}
}