#pragma once
#include<iostream>
#include<cmath>
#include"vector.h"
#include<vector>
#include<stack>
#include<algorithm>

std::vector<point> graham_scan(std::vector<point> pts)
{
	std::vector<point> convex_point;
	std::sort(pts.begin(), pts.end(), [](point a, point b) {if (a.y < b.y) { return true; } else if (a.y == b.y&&a.x < b.x) { return true; } else { return false; }}); //按照y坐标对所有的点进行排序，如果y值相等，则取x较小者为小
	auto pivot = *pts.begin(); //点集中的第一个点一定是凸包中的点，然后要以这个点为枢纽点，将剩下的点按照极角的相对大小排序。如果极角大小一样，则取x值较小者为更小的
	pts.erase(pts.begin());
	std::sort(pts.begin(), pts.end(), [pivot](point a, point b) {if (cross_product(vector(pivot, a), vector(pivot, b)) > 0) { return true; } else if (cross_product(vector(pivot, a), vector(pivot, b)) == 0) { return a.x < b.x; } else { return false; }}); 
	convex_point.push_back(pivot);
	auto i = pts.begin();
	auto pts_top_down = *(i++);
	auto pts_top = *(i++); //分别代表栈顶元素和栈顶的下一个元素
	convex_point.push_back(pts_top_down);
	convex_point.push_back(pts_top); //先将前三个元素推入栈内
	while (i != pts.end())
	{
		while(cross_product(vector(pts_top, pts_top_down), vector(pts_top, *i)) > 0) //从pivot开始，凸包对应的多边形应该一直是沿着逆时针走，如果出现顺时针的情况，则说明有凹进去的边，那么肯定不对，需要将结点弹出。不断循环直至满足要求为止
		{
			convex_point.pop_back();
			pts_top = *(convex_point.end() - 1);
			pts_top_down = *(convex_point.end() - 2);
		}
		pts_top_down = pts_top;
		pts_top = *i;
		convex_point.push_back(pts_top);
		i++;
	}
	return convex_point;
}

/*
调试用主函数：
int main()
{
	std::vector<point> test_pts{ point(0,0),point(1,0),point(7,0),point(7,3),point(7,5),point(6,7),point(1,8),point(1,4),point(2,3),point(3,3),point(4,6),point(4,7) };
	auto result = graham_scan(test_pts);
	for (auto i : result)
	{
		std::cout << i.x << '\t' << i.y << std::endl;
	}
	return 0;
}
*/