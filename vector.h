#pragma once
#include<iostream>
#include<cmath>

struct point
{
	double x;
	double y;
	point(double a = LDBL_MAX, double b = LDBL_MAX)
	{
		x = a;
		y = b;
	}
};

double distance(point a, point b) //计算两点之间的欧几里得距离。可以根据需要修改为不同的距离计算公式
{
	return sqrt((pow((b.x - a.x), 2) + pow((b.y - a.y), 2)));
}

bool on_segment(point a1, point a2, point a3) //判断a3是否在a1和a2组成的线段上
{
	if (a1.x <= a3.x&&a1.y <= a3.y&&a3.x <= a2.x&&a3.y <= a2.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

struct vector
{
	double i;
	double j;
	vector(point a, point b)
	{
		i = b.x - a.x;
		j = b.y - a.y;
	}
};

double cross_product(vector a, vector b) //计算向量a与b的叉乘，如果a在b的逆时针方向，则叉乘的值为负；如果a在b的顺时针方向，叉乘的值为正（右手定则）
{
	return a.i*b.j - a.j*b.i;
}

bool segment_intersect(point a1, point a2, point b1, point b2) //判断线段a1a2和b1b2是否相交
{
	auto d1 = cross_product(vector(b1, a1), vector(b2, a1)); 
	auto d2 = cross_product(vector(b1, a2), vector(b2, a2));
	auto d3 = cross_product(vector(a1, b1), vector(a2, b1));
	auto d4 = cross_product(vector(a1, b2), vector(a2, b2));
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2>0)) && ((d3>0 && d4 < 0) || (d3 < 0 && d4>0)))
	{
		return true;
	}
	else
	{
		if (d1 == 0 && on_segment(b1, b2, a1))
		{
			return true;
		}
		if (d2 == 0 && on_segment(b1, b2, a2))
		{
			return true;
		}
		if (d3 == 0 && on_segment(a1, a2, b1))
		{
			return true;
		}
		if (d4 == 0 && on_segment(a1, a2, b2))
		{
			return true;
		}
		return false;
	}
}

bool operator<(vector a, vector b) //如果向量a在b的顺时针方向，则判定为小于。若共线，则取i或j小的那个向量为小者
{
	if (cross_product(a, b)>=0)
	{
		if (cross_product(a, b) > 0)
		{
			return 1;
		}
		else if (a.i < b.i || a.j < b.j)
		{
			return 1;
		}
	}
	return 0;
}
bool operator>(vector a, vector b) //如果向量a在b的逆时针方向，则判定为大于。若共线，则取i或j小的那个向量为小者
{
	if (cross_product(a, b)<=0)
	{
		if (cross_product(a, b) < 0)
		{
			return 1;
		}
		else if (a.i > b.i || a.j > b.j)
		{
			return 1;
		}
	}
	return 0;
}
bool operator==(vector a, vector b) //如果向量a与b共线且长度相等方向相同，则判定为等于
{
	if (cross_product(a, b) == 0 && a.i == b.i&&a.j == b.j)
	{
		return 1;
	}
	return 0;
}


