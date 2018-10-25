#pragma once
#include<iostream>
#include<vector>

using namespace std;

vector<vector<double>> operator*(vector<vector<double>> a, vector<vector<double>> b)//两矩阵相乘的函数
{
	vector<vector<double>> temp;
	vector<double> line;
	double numtemp = 0.0;
	if (a.begin()->size() != b.size())//乘号左边矩阵的行数必须等于右边矩阵的列数
	{
		cout << "两矩阵无法做乘法运算！" << endl;
		return temp;
	}
	for (auto i = 0;i != a.size();i++)//最终矩阵的行数等于a矩阵的行数
	{
		for (auto j = 0;j != b.begin()->size();j++)//最终矩阵的列数等于b矩阵的列数
		{
			for (auto m = 0; m != a.begin()->size();m++)//a矩阵每行的元素与b矩阵每列的元素相乘
			{
				numtemp += a[i][m] * b[m][j];
			}
			line.push_back(numtemp);
			numtemp = 0.0;
		}
		temp.push_back(line);
		line.clear();
	}
	return temp;
}

vector<vector<double>> unitmatrix(unsigned n)//单位矩阵
{
	vector<vector<double>> matrix(n, { vector<double>(n,0.0) });
	for (unsigned i = 0;i < n;i++)
	{
		matrix[i][i] = 1;
	}
	return matrix;
}

vector<vector<double>> transposition(vector<vector<double>> a)//矩阵转置函数
{
	unsigned line = a.size(), column = a.begin()->size();
	vector<vector<double>> temp(column, { vector<double>(line,0.0) });
	for (auto i = 0;i != a.size();i++)
	{
		for (auto j = 0;j != a.begin()->size();j++)
		{
			temp[j][i] = a[i][j];
		}
	}
	return temp;
}

vector<vector<double>> operator+(vector<vector<double>> a, vector<vector<double>> b)//两个矩阵相加的函数
{
	vector<vector<double>> temp(a.size(), { vector<double>(a.begin()->size(),0.0) });
	if (a.size() != b.size() || a.begin()->size() != b.begin()->size())
	{
		cout << "两个矩阵无法相加！" << endl;
		return temp;
	}
	for (auto i = 0;i != a.size();i++)
	{
		for (auto j = 0;j != a.begin()->size();j++)
		{
			temp[i][j] = a[i][j]+b[i][j];
		}
	}
	return temp;
}

vector<vector<double>> operator-(vector<vector<double>> a, vector<vector<double>> b)//两个矩阵相减的函数
{
	vector<vector<double>> temp(a.size(), { vector<double>(a.begin()->size(),0.0) });
	if (a.size() != b.size() || a.begin()->size() != b.begin()->size())
	{
		cout << "两个矩阵无法相减！" << endl;
		return temp;
	}
	for (auto i = 0;i != a.size();i++)
	{
		for (auto j = 0;j != a.begin()->size();j++)
		{
			temp[i][j] = a[i][j] - b[i][j];
		}
	}
	return temp;
}

vector<vector<double>> operator*(double a, vector<vector<double>> b)//一个数乘以一个矩阵的函数
{
	vector<vector<double>> temp(b.size(), { vector<double>(b.begin()->size(),0.0) });
	for (auto i = 0;i != b.size();i++)
	{
		for (auto j = 0;j != b.begin()->size();j++)
		{
			temp[i][j] = a*b[i][j];
		}
	}
	return temp;
}

vector<double> operator+(vector<double> a, vector <double> b) //两向量相加的运算符
{
	vector<double> temp;
	if (a.size() != b.size())
	{
		cout << "两矩阵阶数不等，无法相加" << endl;
		return temp;
	}
	for (int i = 0;i < a.size();i++)
	{
		temp.push_back(a[i] + b[i]);
	}
	return temp;
}

vector<double> operator*(double a, vector <double> b) //一个标量与一个向量相乘的运算符
{
	vector<double> temp;
	for (int i = 0;i < b.size();i++)
	{
		temp.push_back(a*b[i]);
	}
	return temp;
}

vector<double> operator/(vector <double> b, double a) //一个向量被除一个标量的运算符
{
	vector<double> temp;
	for (int i = 0;i < b.size();i++)
	{
		temp.push_back(b[i] / a);
	}
	return temp;
}