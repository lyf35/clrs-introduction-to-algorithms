#pragma once
#include<iostream>
#include"graph.h"
#include<set>
#include<random>
#include<ctime>

template<typename T, typename W> std::set<T> approx_vertex_cover(graph<T,W> G) //一个求最优顶点覆盖的近似算法，近似比为2，返回一个保存近似顶点的集合。由于此算法是取随机数来选取边，因此最后可能会出现图中只剩下一个孤立点的情况，所以有时运行会出现死循环，而且每次运行的结果可能都不一样，需要多运行几次
{
	std::set<T> pts;
	std::default_random_engine e(time(0));
	graph<T,W> G_del = G; //构造一个新的图以便于修改
	while (G_del.get_size() > 1) 
	{
		std::uniform_int_distribution<int> rand(0, G_del.get_size() - 1);
		int i = rand(e);
		int j = rand(e);
		while (!G_del.get_edge(i,j))
		{
			i = rand(e);
			j = rand(e);
		}
		pts.insert(G_del.get_vertex(i).key);
		pts.insert(G_del.get_vertex(j).key);
		if (i < j)
		{
			G_del.delete_vertex(j);
			G_del.delete_vertex(i);
		}
		else
		{
			G_del.delete_vertex(i);
			G_del.delete_vertex(j);
		}
	}
	return pts;
}

/*
调试代码：
int main()
{
	graph<char, int> test;
	test.insert_vertex('a');
	test.insert_vertex('b');
	test.insert_vertex('c');
	test.insert_vertex('d');
	test.insert_vertex('e');
	test.insert_vertex('f');
	test.insert_vertex('g');
	test.insert_edge(0, 1);
	test.insert_edge(1,2);
	test.insert_edge(2,3);
	test.insert_edge(2,4);
	test.insert_edge(3,4);
	test.insert_edge(4,5);
	test.insert_edge(3,6);
	test.insert_edge(3,5);
	auto i = approx_vertex_cover(test);
	for (auto j : i)
	{
		std::cout << j << " ";
	}
	std::cout << std::endl;
	return 0;
}
*/