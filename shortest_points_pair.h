#pragma once
#include<iostream>
#include<cmath>
#include"vector.h"
#include<utility>
#include<vector>
#include<algorithm>

std::pair<point, point> calculate_shortest_distance(std::vector<point> pts) //当点集中点的个数小于等于三个时，直接用蛮力求解
{
	if (pts.size() == 2)
	{
		return std::make_pair(pts[0], pts[1]);
	}
	else
	{
		auto i12 = distance(pts[0], pts[1]);
		auto i13 = distance(pts[0], pts[2]);
		auto i23 = distance(pts[1], pts[2]);
		if (i12 < i13)
		{
			if (i13 < i23)
			{
				return std::make_pair(pts[0], pts[1]);
			}
			else
			{
				if (i12 < i23)
				{
					return std::make_pair(pts[0], pts[1]);
				}
				else
				{
					return std::make_pair(pts[1], pts[2]);
				}
			}
		}
		else
		{
			if (i23 < i13)
			{
				return std::make_pair(pts[1], pts[2]);
			}
			else
			{
				return std::make_pair(pts[0], pts[2]);
			}
		}
	}
}

std::pair<point, point> shortest_points_pair(std::vector<point> pts) //按照欧几里得距离计算平面内最近距离点对，也可以换用其它的距离定义，但是也要同时修改计算最大距离时最后判断距离的点数
{
	sort(pts.begin(), pts.end(), [](point a, point b) {if (a.x < b.x) { return true; } else if (a.x == b.x&&a.y < b.y) { return true; } else { return false; }});//按照横坐标大小排序，如果横坐标相等以纵坐标小者为小
	int num_size = pts.size() / 2;
	std::vector<point> left(pts.begin(), pts.begin() + num_size), right(pts.begin() + num_size, pts.end());
	std::vector<point> left_sortby_x = left, right_sortby_x = right, left_sortby_y = left, right_sortby_y = right;
	sort(left_sortby_y.begin(), left_sortby_y.end(), [](point a, point b) {if (a.y < b.y) { return true; } else if (a.y == b.y&&a.x < b.x) { return true; } else { return false; }});
	sort(right_sortby_y.begin(), right_sortby_y.end(), [](point a, point b) {if (a.y < b.y) { return true; } else if (a.y == b.y&&a.x < b.x) { return true; } else { return false; }});
	std::pair<point, point> left_pts_pair, right_pts_pair, total_pts_pair;
	if (left.size() <= 3) //对左端的点集使用分治策略处理
	{
		left_pts_pair = calculate_shortest_distance(left);
	}
	else
	{
		left_pts_pair = shortest_points_pair(left);
	}
	if (right.size() <= 3) //对右端点集采用分治策略处理
	{
		right_pts_pair = calculate_shortest_distance(right);
	}
	else
	{
		right_pts_pair = shortest_points_pair(right);

	}
	total_pts_pair = distance(left_pts_pair.first, left_pts_pair.second) > distance(right_pts_pair.first, right_pts_pair.second) ? right_pts_pair : left_pts_pair; //取左右两端点对中距离更小者
	double dist = distance(total_pts_pair.first, total_pts_pair.second); //储存最小距离
	if (distance(*(left_sortby_x.end() - 1), *(right_sortby_x.begin())) >= dist) //如果左侧点集的最右点与右侧点集最左点之间的距离大于找出的最小距离，那么在左/右点集连接处肯定不会存在两点分属于两点集并且距离小于当前最小距离，可以直接返回
	{
		return total_pts_pair;
	}
	else
	{
		std::vector<point> le_dist;
		double mid = ((*(left_sortby_x.end() - 1)).x + (*(right_sortby_x.begin())).x) / 2; //代表左右两点集的中心线，在下一步挑选点的时候，只考虑那些落在中心线两端不超过dist范围内的点
		auto i = left_sortby_y.begin();
		auto j = right_sortby_y.begin();
		while (i != left_sortby_y.end() && j != right_sortby_y.end()) //用归并，将左右点集中在中心线两端dist区域内的点按照纵坐标排序
		{
			if ((*i).x < mid - dist)
			{
				i++;
				break;
			}
			if ((*j).x > mid + dist)
			{
				j++;
				break;
			}
			if ((*i).y < (*j).y)
			{
				le_dist.push_back(*i);
				i++;
			}
			else if ((*i).y == (*j).y && (*i).x < (*j).x)
			{
				le_dist.push_back(*i);
				i++;
			}
			else
			{
				le_dist.push_back(*j);
				j++;
			}
		}
		if (i == left_sortby_y.end())
		{
			while (j != right_sortby_y.end())
			{
				if ((*j).x <= mid + dist)
				{
					le_dist.push_back(*j);
				}
				j++;
			}
		}
		else
		{
			while (i != left_sortby_y.end())
			{
				if ((*i).x >= mid - dist)
				{
					le_dist.push_back(*i);
				}
				i++;
			}
		}
		for (auto ptr = le_dist.begin();ptr != le_dist.end();ptr++) //在中间区域内寻找，看能否找到距离更小的点对
		{
			for (int i = 1;i <= 5;i++) //由于此处求解的是欧几里得距离定义下的最小距离，因此最多只需要考虑某个点后面的5个点即可
			{
				if (ptr + i != le_dist.end())
				{
					if (distance(*(ptr), *(ptr + i)) < dist) //如果找到距离更小的点对，则更新dist和total_dis_pair的值
					{
						dist = distance(*(ptr), *(ptr + i));
						total_pts_pair = std::make_pair(*(ptr), *(ptr + i));
					}
				}
				else
				{
					break;
				}
			}
		}
		return total_pts_pair;
	}
}

/*
调试用主函数：
int main()
{
	std::vector<point> test_pts{ point(0,0),point(1,2),point(1,3),point(5,0),point(6,5),point(7,4),point(12,0),point(12.5,0),point(13,2),point(13,5),point(13,6.7),point(13,7.1),point(13,-5),point(13,-8),point(13,8),point(13,10),point(15,3),point(15,7),point(16,10),point(15,20),point(17,30) };
	auto i = shortest_points_pair(test_pts);
	std::cout << i.first.x << "," << i.first.y << "  " << i.second.x << "," << i.second.y << std::endl;
	return 0;
}
*/