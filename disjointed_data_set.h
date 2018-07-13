#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<memory>

template<typename T> struct set_forest_node :public std::enable_shared_from_this<set_forest_node<T>>
{
	std::shared_ptr<set_forest_node<T>> parent;
	T key;
	int rank;
	set_forest_node(T k, int r = 0) :parent(), key(k), rank(r) {};
	void node_fix()
	{
		parent = shared_from_this();
	}
};

template<typename T> std::shared_ptr<set_forest_node<T>> find_set(std::shared_ptr<set_forest_node<T>> x)
{
	if (x != x->parent)
	{
		x->parent = find_set(x->parent); //在向上搜索的过程中，同时对路径进行压缩
	}
	return x->parent;
}

template<typename T> std::shared_ptr<set_forest_node<T>> link(std::shared_ptr<set_forest_node<T>> x, std::shared_ptr<set_forest_node<T>> y)
{
	if (x->rank > y->rank)
	{
		y->parent = x;
		return x;
	}
	else
	{
		x->parent = y;
		if (x->rank == y->rank)
		{
			y->rank++;
		}
		return y;
	}
}

template<typename T> std::shared_ptr<set_forest_node<T>> union_set(std::shared_ptr<set_forest_node<T>> x, std::shared_ptr<set_forest_node<T>> y) //对两个集合进行合并
{
	return link(find_set(x), find_set(y));
}

/*
调试代码：
	std::default_random_engine e(std::time(0));
	std::uniform_int_distribution<int> rand_gen(0, 255);
	std::vector<std::shared_ptr<set_forest_node<int>>> test1;
	std::vector<std::shared_ptr<set_forest_node<int>>> test2;
	for (int i = 0;i < 20;i++)
	{
		int t = rand_gen(e);
		std::cout << "create:" << t << std::endl;
		test1.push_back(std::shared_ptr<set_forest_node<int>>(new set_forest_node<int>(t)));
		test1[i]->node_fix();
	}
	for (int i = 0;i < 10;i++)
	{
		int t = rand_gen(e);
		std::cout << "create:" << t << std::endl;
		test2.push_back(std::shared_ptr<set_forest_node<int>>(new set_forest_node<int>(t)));
		test2[i]->node_fix();
	}
	auto testp1 = test1[0];
	auto testp2 = test2[0];

	auto temp1 = test1[0];
	auto temp2 = test2[0];

	for (int i = 1;i < 20;i++)
	{
		temp1=link(test1[0], test1[i]);
	}

	for (int i = 1;i < 10;i++)
	{
		temp2 = link(test2[0], test2[i]);
	}

	std::cout << find_set(testp1)->key << "  " << find_set(temp1)->key << std::endl;
	std::cout << find_set(testp2)->key << "  " << find_set(temp2)->key << std::endl;

	union_set(temp1, temp2);
	std::cout << find_set(testp1)->key << "  " << find_set(temp1)->key << std::endl;
	std::cout << find_set(testp2)->key << "  " << find_set(temp2)->key << std::endl;

	return 0;
*/