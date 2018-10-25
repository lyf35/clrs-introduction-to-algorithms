#pragma once
#include<iostream>
#include<cmath>
#include"vector.h"
#include<vector>
#include<tuple>
#include"RedBlackTree.h"
#include<algorithm>

bool operator<(std::tuple<point, int, bool> a, std::tuple<point, int, bool> b) //对线段端点按照横坐标进行排序，如果横坐标相同，左端点优先；如果都是左/右端点且横坐标相同，则纵坐标小者优先
{
	if (std::get<0>(a).x < std::get<0>(b).x)
	{
		return true;
	}
	else if (std::get<0>(a).x == std::get<0>(b).x&&std::get<2>(a) < std::get<2>(b))
	{
		return true;
	}
	else if (std::get<0>(a).x == std::get<0>(b).x&&std::get<2>(a) == std::get<2>(b) && std::get<0>(a).y < std::get<0>(b).y)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool operator==(std::tuple<point, int, bool> a, std::tuple<point, int, bool> b) //如果等于的话，则tuple的每一个数据都相等
{
	if (std::get<0>(a).x == std::get<0>(b).x&&std::get<0>(a).y == std::get<0>(b).y&&std::get<1>(a) == std::get<1>(b) && std::get<2>(a) == std::get<2>(b))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool operator>(std::tuple<point, int, bool> a, std::tuple<point, int, bool> b)
{
	return !((a < b)||(a==b));
}


bool any_segment_intersect(std::vector<point> left,std::vector<point> right) //分别存储线段i的左右端点
{
	std::vector<std::tuple<point, int, bool>> pts;
	for (auto i = 0;i < left.size();i++)
	{
		pts.push_back(std::make_tuple(left[i], i, 0)); //tuple的第一个元素是点，第二个元素代表它原本对应的线段编号，第三个元素代表左右端点（0为左，1为右）
		pts.push_back(std::make_tuple(right[i], i, 1));
	}
	sort(pts.begin(), pts.end()); //对线段端点按照横坐标进行排序，如果横坐标相同，左端点优先；如果都是左/右端点且横坐标相同，则纵坐标小者优先
	auto i = pts.begin();
	RBTree<std::tuple<point, int, bool>> T(*(i++)); //用于保存当前扫除线上的线段条数
	while (i != pts.end())
	{
		if (std::get<2>(*i) == 0) //代表当前处理的点是某个线段的左端点
		{
			T.insert_element(*i);
			if ((T.find_successor(*i) && segment_intersect(left[std::get<1>(*i)], right[std::get<1>(*i)], left[std::get<1>(T.find_successor(*i)->element)], right[std::get<1>(T.find_successor(*i)->element)]))
				|| (T.find_predecessor(*i) && segment_intersect(left[std::get<1>(*i)], right[std::get<1>(*i)], left[std::get<1>(T.find_predecessor(*i)->element)], right[std::get<1>(T.find_predecessor(*i)->element)])))
			{
				return true;
			}
		}
		else //代表当前处理的点是某个线段的右端点
		{
			if (T.find_successor(*i) && T.find_predecessor(*i) && segment_intersect(left[std::get<1>(T.find_successor(*i)->element)], right[std::get<1>(T.find_successor(*i)->element)], left[std::get<1>(T.find_predecessor(*i)->element)], right[std::get<1>(T.find_predecessor(*i)->element)]))
			{
				return true;
			}
			T.delete_element(*i);
		}
		i++;
	}
	return false;
}

/*
测试主函数：
int main() //无交点
{
	std::vector<point> test_pts1{ point(0,0),point(1,2),point(1,3),point(5,0),point(6,5),point(7,4) };
	std::vector<point> test_pts2{ point(1,0),point(2,0),point(3,4),point(5,4),point(7,5),point(8,4) };
	std::cout << any_segment_intersect(test_pts1, test_pts2) << std::endl;
	return 0;
}

int main() //有交点
{
	std::vector<point> test_pts1{ point(0,0),point(1,2),point(1,1),point(5,0),point(6,5),point(7,4) };
	std::vector<point> test_pts2{ point(1,0),point(2,0),point(3,4),point(5,4),point(7,5),point(8,4) };
	std::cout << any_segment_intersect(test_pts1, test_pts2) << std::endl;
	return 0;
}
*/