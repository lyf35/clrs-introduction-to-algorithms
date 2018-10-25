#pragma once
#include<iostream>
#include<vector>
#include<cmath>
#include<numeric>
#include<algorithm>
#include<iterator>
#include"function.h"

using namespace std;

class matrix
{
	friend vector<vector<double>> operator*(vector<vector<double>> a, vector<vector<double>> b);
	friend vector<vector<double>> unitmatrix(unsigned n);
	friend vector<vector<double>> transposition(vector<vector<double>> a);

private:
	vector<vector<double>> A; //存放原始矩阵。为了可以使任意阶的矩阵都可以被分解，所以用vector来存储
	vector<vector<double>> Q; //分别存放QR分解后的矩阵
	vector<vector<double>> R;
	vector<vector<double>> L; //分别存放LUP分解之后的矩阵
	vector<vector<double>> U;
	vector<vector<double>> P;

public:
	matrix(const vector<vector<double>> init) :A(init), Q(unitmatrix(init.size())), R(init), L(unitmatrix(init.size())), U(unitmatrix(init.size())), P(unitmatrix(init.size())) {};
	bool QR_decomposition()//返回值为1代表分解成功，返回值为0代表分解失败
	{
		if (A.size() < A.begin()->size())//如果列数大于行数则无法进行分解，直接返回0
		{
			cout << "列数大于行数，无法进行分解！" << endl;
			return 0;
		}
		vector<vector<double>> temp;//存储当前步的豪斯霍尔德矩阵
		for (auto i = 0;i != min(A.begin()->size(), A.size() - 1);i++)//列数有多少，便一共需进行多少次分解
		{
			vector<vector<double>> H(A.size() - i, { vector<double>(A.size() - i,0.0) });
			vector<double> x;
			vector<double> u;
			//构造变量x
			for (int j = i;j < A.size();j++)
			{
				x.push_back(R[j][i]);
			}
			//构造变量u
			double x_second_norm = sqrt(inner_product(x.begin(),x.end(),x.begin(),0.0));//计算x的二范数
			x[0] = x[0] - x_second_norm;//由于计算u的时候只有x内的第一个元素被减去sigma*v，故只需要对x的第一个元素做变化即可
			double xmod_second_norm = sqrt(inner_product(x.begin(), x.end(), x.begin(), 0.0));//计算x减去sigma*v后所得向量的2范数
			copy(x.begin(), x.end(), back_inserter(u));
			for_each(u.begin(), u.end(), [xmod_second_norm](double& a) {a = a / xmod_second_norm;});
			//构造豪斯霍尔德矩阵，先计算豪斯霍尔德矩阵元素
			for (auto hi = 0;hi < A.size() - i;hi++)
			{
				for (auto hj = 0;hj < A.size() - i;hj++)
				{
					H[hi][hj] = -2 * u[hi] * u[hj];
					if (hi == hj)
					{
						H[hi][hj] += 1.0;
					}
				}
			}
			//如果i>0，即只提取了R列向量的部分元素，则需要对所得矩阵进行扩展
			if (i > 0)
			{
				for (auto &iter : H)
				{
					iter.insert(iter.begin(), i, 0.0);
				}
				for (int m = i;m > 0;m--)
				{
					vector<double> temp(H.begin()->size(), 0.0);
					temp[m - 1] = 1;
					H.insert(H.begin(), temp);
				}
			}
			R = H*R;
			Q = H*Q;
		}
		Q = transposition(Q);
		return 1;
	}
	bool LUP_decomposition() //在分解的时候选取主元
	{
		if (A.size() != A.begin()->size())
		{
			std::cout << "待分解矩阵的行列不相等！";
			return 0;
		}
		vector<vector<double>> A_temp = A; 
		int size = A.size();
		for (int i = 0;i < size;i++) //为了确保精度，使用选主元的高斯消去法
		{
			//先选取主元
			double col_max = A_temp[i][i]; //用于保存每一列从i到n的最大元素
			int line = i; //保存最大元素所在的行
			for (int x = i + 1;x < size;x++) 
			{
				if (col_max < A_temp[x][i])
				{
					col_max = A_temp[x][i];
					line = x;
				}
			}
			std::swap(A_temp[i], A_temp[line]); //将主元换至第i行
			std::swap(P[i], P[line]);
			//选取完主元之后，再做LU分解
			for (int x = i + 1;x < size;x++) //计算每一行的系数，并相应更新矩阵中这一行的值
			{
				A_temp[x][i] /= A_temp[i][i];
				for (int y = x;y < size;y++)
				{
					A_temp[x][y] -= A_temp[x][i] * A_temp[i][y];
				}
			}
			//执行完这些步骤之后，将临时矩阵中的值写入到L和U中
			for (int i = 1;i < size;i++)
			{
				for (int j = 0;j < i;j++)
				{
					L[i][j] = A_temp[i][j];
				}
			}
			for (int i = 0;i < size;i++)
			{
				for (int j = i;j < size;j++)
				{
					U[i][j] = A_temp[i][j];
				}
			}
		}
		return 1;
	}
	bool LU_decomposition() //在分解的时候不选取主元。此函数在调用时，需要确保原矩阵是对称正定矩阵
	{
		if (A.size() != A.begin()->size())
		{
			std::cout << "待分解矩阵的行列不相等！";
			return 0;
		}
		vector<vector<double>> A_temp = A;
		int size = A.size();
		for (int i = 0;i < size;i++) 
		{
			//先计算每一行的系数，并相应更新矩阵中这一行的值
			for (int x = i + 1;x < size;x++) 
			{
				A_temp[x][i] /= A_temp[i][i];
				for (int y = x;y < size;y++)
				{
					A_temp[x][y] -= A_temp[x][i] * A_temp[i][y];
				}
			}
			//将临时矩阵中的值写入到L和U中
			for (int i = 1;i < size;i++)
			{
				for (int j = 0;j < i;j++)
				{
					L[i][j] = A_temp[i][j];
				}
			}
			for (int i = 0;i < size;i++)
			{
				for (int j = i;j < size;j++)
				{
					U[i][j] = A_temp[i][j];
				}
			}
		}
		return 1;
	}
	vector<vector<double>> solve_LUP(vector<vector<double>> b) //进行LUP分解并求解方程。运行此函数之前要自行调用LUP分解的函数
	{
		b = P*b;
		vector<vector<double>> Y = b;
		for (int i = 0;i < b.size();i++)
		{
			for (int j = 0;j < i;j++)
			{
				if (L[i][j] != 0)
				{
					Y[i][0] -= Y[j][0] * L[i][j];
				}
			}
		}
		vector<vector<double>> X = Y;
		for (int i = b.size() - 1;i >= 0;i--)
		{
			for (int j = b.size() - 1;j > i;j--)
			{
				if (U[i][j] != 0)
				{
					X[i][0] -= X[j][0] * U[i][j];
				}
			}
			X[i][0] /= U[i][i];
		}
		return X;
	}
	vector<vector<double>> inverse_matrix() //利用LUP分解的结果求矩阵的逆
	{
		vector<vector<double>> inverse = A;
		for (int i = 0;i < A.size();i++)
		{
			vector<vector<double>> X(A.size(), vector<double>(1, 0));
			X[i][0] = 1;
			X = solve_LUP(X);
			for (int j = 0;j < A.size();j++)
			{
				A[i][j] = X[j][0];
			}
		}
		return A;
	}
	vector<vector<double>> solve_QR(vector<vector<double>> b)//进行QR分解并求解。运行此函数之前要自行调用QR分解的函数
	{
		vector<vector<double>> R_temp(R);//为了使R矩阵不被修改，用一个临时变量存储R以便接下来求解矩阵
		vector<vector<double>> y = transposition(Q)*b;
		vector<vector<double>> X(b.size(), vector<double>(1, 0));
		for (int i = R.begin()->size() - 1;i > 0;i--)
		{
			for (int j = i - 1;j >= 0;j--)
			{
				y[j][0] = y[j][0] - R_temp[j][i] / R_temp[i][i] * y[i][0];
				R_temp[j][i] = 0;
			}
		}
		for (int i = R.begin()->size() - 1;i >= 0;i--)
		{
			X[i][0] = y[i][0] / R[i][i];
		}
		return X;
	}
	void output_QR(ostream& os)//输出分解所得矩阵Q和R的函数，可以自由选择输出到控制台或文本中
	{
		os << "Q矩阵：" << endl;
		for (auto i = 0;i < Q.size();i++)
		{
			for (auto j = 0;j < Q.begin()->size();j++)
			{
				os << Q[i][j] << '\t';
			}
			os << endl;
		}
		os << "R矩阵：" << endl;
		for (auto i = 0;i < R.size();i++)
		{
			for (auto j = 0;j < R.begin()->size();j++)
			{
				os << R[i][j] << '\t';
			}
			os << endl;
		}
	}
	void output_LUP(ostream& os)//输出分解所得矩阵Q和R的函数，可以自由选择输出到控制台或文本中
	{
		os << "L矩阵：" << endl;
		for (auto i = 0;i < L.size();i++)
		{
			for (auto j = 0;j < L.begin()->size();j++)
			{
				os << L[i][j] << '\t';
			}
			os << endl;
		}
		os << "U矩阵：" << endl;
		for (auto i = 0;i < U.size();i++)
		{
			for (auto j = 0;j < U.begin()->size();j++)
			{
				os << U[i][j] << '\t';
			}
			os << endl;
		}
		os << "P矩阵：" << endl;
		for (auto i = 0;i < P.size();i++)
		{
			for (auto j = 0;j < P.begin()->size();j++)
			{
				os << P[i][j] << '\t';
			}
			os << endl;
		}
	}
};

/*
测试主函数：
#include<iostream>
#include<vector>
#include"matrix.h"

using namespace std;

int main()
{
	vector<vector<double>> orig{ {1,-1,0,0,0},{-1,2,-1,0,0},{0,-1,2,-1,0},{0,0,-1,2,-1},{0,0,0,-1,2} };
	vector<vector<double>> in{ { 1 },{ 1 },{ 1 },{ 1 },{ 1 } };
	matrix a(orig);
	a.QR_decomposition();
	auto result_QR = a.solve_QR(in);
	cout << "用QR分解的结果解方程：" << endl;
	for (auto i : result_QR)
	{
		cout << i[0] << '\t';
	}
	cout << endl;
	a.output_QR(cout);
	a.LUP_decomposition();
	cout << "用LUP分解的结果求逆矩阵：" << endl;
	auto inv = a.inverse_matrix();
	for (auto i = 0;i < inv.size();i++)
	{
		for (auto j = 0;j < inv.begin()->size();j++)
		{
			cout << inv[i][j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
	a.output_LUP(cout);
}
*/
