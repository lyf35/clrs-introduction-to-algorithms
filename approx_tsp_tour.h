#pragma once
#include<iostream>
#include"graph.h"

template<typename T, typename W> void approx_tsp_tour(graph<T, W> G) //近似的旅行商算法，要求点之间的距离满足三角不等式，近似度为2
{
	auto mst_tree = G.MST_Prim(); //先计算图的一个最小生成树
	MST_preorder_traversal(mst_tree, 0, -1);
	std::cout << std::endl;
}

template<typename W> void MST_preorder_traversal(std::vector<std::vector<std::shared_ptr<graph_edge<W>>>> M, int i, int p) //M为MST对应的邻接矩阵，i代表当前所在结点。为了避免在搜索子树的时候搜索至父结点，再用一个变量p表示当前搜索结点的父结点。如果是根节点，可以将p设置为-1，这样就能访问结点i的所有子树。将MST的结点按照前序遍历的顺序输出
{
	std::cout << i << " ";
	for (int j = 0;j < M.size();j++)
	{
		if (M[i][j] && j != p)
		{
			MST_preorder_traversal(M, j, i);
		}
	}
}

/*
调试代码（clrs图35-2）
#include<iostream>
#include<vector>
#include"graph.h"
#include"approx_tsp_tour.h"
#include"vector.h"

int main()
{
	graph<point, double> test;
	test.insert_vertex(point(1,4));
	test.insert_vertex(point(1,2));
	test.insert_vertex(point(0,1));
	test.insert_vertex(point(3,4));
	test.insert_vertex(point(4,3));
	test.insert_vertex(point(3,2));
	test.insert_vertex(point(5,2));
	test.insert_vertex(point(2,0));
	for (auto i = 0;i < test.get_size();i++)
	{
		for (auto j = 0;j < i;j++)
		{
			test.insert_edge(i, j, double(), distance(test.get_vertex(i).key, test.get_vertex(j).key));
		}
	}
	approx_tsp_tour(test);
	return 0;
}
*/