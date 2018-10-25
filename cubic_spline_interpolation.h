#pragma once
#include<iostream>
#include<cmath>
#include<vector>
#include"function.h"
#include"matrix.h"

using namespace std;

class cubic_spline_interpolation
{
	friend class matrix;
private:
	vector<double> x;//存放参数
	vector<double> y;
	vector<double> h;
	vector<double> d;
	vector<vector<double>> M;
	vector <vector<double>> para;//存放最后计算得到的每个区间插值函数的系数
	unsigned mode;//存放边界条件
	double bound1, bound2;

public:
	void calculation_h()//计算变量h的函数
	{
		h.push_back(0.0);
		for (int i = 1;i < x.size();i++)
		{
			double temp = x[i] - x[i - 1];
			h.push_back(temp);
		}
	}
	void calculation_d()//计算变量d的函数
	{
		d.push_back(0.0);
		for (int i = 1;i < x.size()-1;i++)
		{
			double temp = 6 / (h[i] + h[i + 1])*((y[i + 1] - y[i]) / h[i + 1] - (y[i] - y[i - 1]) / h[i]);
			d.push_back(temp);
		}
		switch (mode)
		{
		case 1:
			d[1] = d[1] - h[1] / (h[1] + h[2])*bound1;
			d[d.size() - 1] = d[d.size() - 1] - h[d.size() - 1] / (d[d.size() - 1] + d[d.size() - 2])*bound2;
			break;
		case 2:
			d[0] = 6 / h[1] * ((y[1] - y[0]) / h[1] - bound1);
			d.push_back(6 / h[h.size() - 1] * (bound2 - (y[h.size() - 1] - y[h.size() - 2]) / h[h.size() - 1]));
			break;
		case 3:
			d.push_back(6 / (h[1] + h[h.size() - 1])*((y[1] - y[h.size() - 1]) / h[1] - (y[h.size() - 1] - y[h.size() - 2]) / h[h.size() - 1]));
			break;
		}
	}
	//初始化变量
	cubic_spline_interpolation(vector<double> x_input, vector<double> y_input) :x(x_input), y(y_input), M(x.size(), { vector<double>(1,0.0) }), para(x.size(), { vector<double>(4,0.0) })
	{
		cout << "请选择边界条件，1代表已知端点处的二阶导数，2代表已知端点处的一阶导数，3代表周期性边界条件：";
		cin >> mode;
		switch (mode)
		{
		case 1:
			cout << "请分别输入左端点和右端点处的二阶导数值：";
			cin >> bound1 >> bound2;
			M.resize(M.size() - 2);
			break;
		case 2:
			cout << "请分别输入左端点和右端点处的一阶导数值：";
			cin >> bound1 >> bound2;
			break;
		case 3:
			M.pop_back();
			break;
		}

		calculation_h();
		calculation_d();
	};
	cubic_spline_interpolation(vector<double> x_input, vector<double> y_input, unsigned mod, double bon1, double bon2) :x(x_input), y(y_input), M(x.size(), { vector<double>(1,0.0) }), para(x.size(), { vector<double>(4,0.0) }), mode(mod), bound1(bon1), bound2(bon2) 
	{
		switch (mode)
		{
		case 1:
			M.resize(M.size() - 2);
			break;
		case 3:
			M.pop_back();
			break;
		}
		calculation_h();
		calculation_d();
	};
	void calculation_para()//计算插值函数各项系数，按照三次项、二次项、一次项、常数项的顺序存放，i=1对应区间[x0,x1]，以此类推
	{
		for (auto i = 1;i < x.size();i++)
		{
			para[i][0] = M[i][0] / 6 / h[i] - M[i - 1][0] / 6 / h[i];
			para[i][1] = x[i] * M[i - 1][0] / 2 / h[i] - x[i - 1] * M[i][0] / 2 / h[i];
			para[i][2] = x[i - 1] * x[i - 1] * M[i][0] / 2 / h[i] - x[i] * x[i] * M[i - 1][0] / 2 / h[i] - y[i - 1] / h[i] + h[i] * M[i - 1][0] / 6 + y[i] / h[i] - h[i] * M[i][0] / 6;
			para[i][3] = x[i] * x[i] * x[i] * M[i - 1][0] / 6 / h[i] - x[i - 1] * x[i - 1] * x[i - 1] * M[i][0] / 6 / h[i] + y[i - 1] * x[i] / h[i] - h[i] * M[i - 1][0] * x[i] / 6 - y[i]*x[i - 1] / h[i] + h[i] * M[i][0] * x[i - 1] / 6;
		}
	}
	void solve_QR()//求解矩阵M
	{
		if (mode==1)//第一类边界条件：已知两端点处的二阶导数
		{
			vector<vector<double>> A(x.size() - 2, { vector<double>(x.size() - 2,0.0) }), b(x.size() - 2, { vector<double>(1,0.0) });
			for (auto i = 0;i < A.size();i++)
			{
				for (auto j = 0;j < A.size();j++)
				{
					if (i == j)
					{
						A[i][j] = 2;
					}
					if (i + 1 == j)
					{
						A[i][j] = h[j + 1] / (h[j] + h[j + 1]);
					}
					if (i == j + 1)
					{
						A[i][j] = h[i + 1] / (h[i + 1] + h[i + 2]);
					}
				}
				b[i][0] = d[i + 1];
			}
			matrix solve_QR_m1 (A);
			solve_QR_m1.QR_decomposition();
			M = solve_QR_m1.solve_QR(b);
			M.push_back({ bound2 });
			M.insert(M.begin(), { bound1 });
		}
		if (mode == 2)//第二类边界条件：已知两端点处的一阶导数
		{
			vector<vector<double>> A(x.size(), { vector<double>(x.size(),0.0) }), b(x.size(), { vector<double>(1,0.0) });
			for (auto i = 0;i < A.size();i++)
			{
				for (auto j = 0;j < A.size();j++)
				{
					if (i == j)
					{
						A[i][j] = 2;
					}
					if (i + 1 == j)
					{
						if (i == 0)
						{
							A[i][j] = 1;
						}
						else
						{
							A[i][j] = h[i + 1] / (h[i] + h[i + 1]);
						}
					}
					if (i == j + 1)
					{
						if (i == A.size() - 1)
						{
							A[i][j] = 1;
						}
						else
						{
							A[i][j] = h[i] / (h[i + 1] + h[i]);
						}
					}
				}
				b[i][0] = d[i];
			}
			matrix solve_QR_m2(A);
			solve_QR_m2.QR_decomposition();
			M = solve_QR_m2.solve_QR(b);
		}
		if (mode == 3)//第三类边界条件：周期性边界条件
		{
			vector<vector<double>> A(x.size() - 1, { vector<double>(x.size() - 1,0.0) }), b(x.size() - 1, { vector<double>(1,0.0) });
			for (auto i = 0;i < A.size();i++)
			{
				for (auto j = 0;j < A.size();j++)
				{
					if (i == j)
					{
						A[i][j] = 2;
					}
					if (i + 1 == j)
					{
						A[i][j] = h[j + 1] / (h[j] + h[j + 1]);
					}
					if (i == j + 1)
					{
						if (i != A.size() - 1)
						{
							A[i][j] = h[i + 1] / (h[i + 1] + h[i + 2]);
						}
						else
						{
							A[i][j] = h[i + 1] / (h[i + 1] + h[1]);
						}
					}
				}
				b[i][0] = d[i + 1];
			}
			A[0][A.size() - 1] = h[1] / (h[1] + h[2]);
			A[A.size() - 1][0] = h[1] / (h[1] + h[A.size()]);
			matrix solve_QR_m3(A);
			solve_QR_m3.QR_decomposition();
			M = solve_QR_m3.solve_QR(b);
			M.insert(M.begin(), *(M.rbegin()));
		}
	}
	ostream& print(ostream& os)//输出三次样条插值函数的系数
	{
		for (auto i = 1;i < x.size();i++)
		{
			if (abs(para[i][0]) > 1E-7)//由于para变量内存储的是double类型的变量，在判断是否等于一个数字的时候如果直接相比可能会受到小数点部分的影响，因此用这种方法比较
			{
				if (abs(para[i][0] - 1) < 1E-7)
				{
					os << "x^3";
				}
				else if (abs(para[i][0] + 1) < 1E-7)
				{
					os << "-x^3";
				}
				else
				{
					os << para[i][0] << "x^3";
				}
			}
			if (abs(para[i][1]) > 1E-7)
			{
				if (para[i][1] > 0)
				{
					os << "+";
				}
				if (abs(para[i][1] - 1) < 1E-7)
				{
					os << "x^2";
				}
				else if (abs(para[i][1] + 1) < 1E-7)
				{
					os << "-x^2";
				}
				else
				{
					os << para[i][1] << "x^2";
				}
			}
			if (abs(para[i][2]) > 1E-7)
			{
				if (para[i][2] > 0)
				{
					os << "+";
				}
				if (abs(para[i][2] - 1) < 1E-7)
				{
					os << "x";
				}
				else if (abs(para[i][2] + 1) < 1E-7)
				{
					os << "-x";
				}
				else
				{
					os << para[i][2] << "x";
				}
			}
			if (abs(para[i][3]) > 1E-7)
			{
				if (para[i][3] > 0)
				{
					os << "+";
				}
				os << para[i][3] << endl;
			}
		}
		return os;
	}
};

/*
三次样条插值的测试主函数:
int main()
{
	cubic_spline_interpolation a({ -3,-1,0,3,4 }, { 7,11,26,56,29 });
	a.solve_QR();
	a.calculation_para();
	a.print(cout);
	return 0;
}
*/