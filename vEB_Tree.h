#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<memory>

int up_root(double x)
{
	return pow(2, ceil(log(x) / log(2.0) / 2.0));
}
int down_root(double x)
{
	return pow(2, floor(log(x) / log(2.0) / 2.0));
}
int high(int x, int u)
{
	return floor(x / down_root(u));
}
int low(int x, int u)
{
	return x%down_root(u);
}
int index(int x, int y, int u)
{
	return x*down_root(u) + y;
}

struct vEB_node //规定在veb_node中数值等于-1的变量都代表不存在
{
	int u;
	int min;
	int max;
	std::shared_ptr<vEB_node> summary;
	std::vector<std::shared_ptr<vEB_node>> cluster;
	vEB_node(int size, int minimum = -1, int maximum = -1) :u(size), min(minimum), max(maximum) //此处给出的u必须是2的幂
	{
		if (u != 2)
		{
			summary = std::shared_ptr<vEB_node>(new vEB_node(up_root(u)));
			cluster = std::vector<std::shared_ptr<vEB_node>>(up_root(u), nullptr);
			//注意：此处不能用cluster = std::vector<std::shared_ptr<vEB_node>>(up_root(u), std::shared_ptr<vEB_node>(new vEB_node(down_root(u))))这样的初始化方式！因为这样写就相当于是将vector的每个位置都存入一个相同的指针。但是veb树的实质是cluster中的每个位置上存储着一个不同的子veb结点
			for (auto i = 0;i < up_root(u);i++)
			{
				cluster[i] = std::shared_ptr<vEB_node>(new vEB_node(down_root(u)));
			}
		}
	}
};

class vEB_Tree
{
private:
	std::shared_ptr<vEB_node> root;
public:
	vEB_Tree(int n = 2) :root(new vEB_node(n)) {};
	std::shared_ptr<vEB_node> get_root()
	{
		return root;
	}
	int minimum(std::shared_ptr<vEB_node> n)
	{
		return n->min;
	}
	int maximum(std::shared_ptr<vEB_node> n)
	{
		return n->max;
	}
	bool tree_member(std::shared_ptr<vEB_node> n, int x)
	{
		if (x == n->max || x == n->min)
		{
			return true;
		}
		else if (n->u == 2)
		{
			return false;
		}
		else
		{
			return tree_member(n->cluster[high(x, n->u)], low(x, n->u));
		}
	}
	int predecessor(std::shared_ptr<vEB_node> n, int x) //注意：x不一定存储在veb树中
	{
		if (n->u == 2) //基础情况
		{
			if (x == 1 && n->min == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
		else if (n->max != -1 && x > n->max) //在搜索时进入到某个veb子树中，树中的max值比x还小，那么max值就为x的前驱
		{
			return n->max;
		}
		else
		{
			int min_low = minimum(n->cluster[high(x, n->u)]); //进入x本应该在的那个cluster，并找出这个cluster中的最小值
			if (min_low != -1 && low(x, n->u) > min_low) //如果前驱在x应属于的那个cluster内
			{
				int offset = predecessor(n->cluster[high(x, n->u)], low(x, n->u)); //进入x所在的cluster，并递归查找
				return index(high(x, n->u), offset, n->u); //将high和low的组合值转化为对应的整数值
			}
			else //x不在它应属于的那个cluster，需要向前找到一个新的cluster
			{
				int pred_cluster = predecessor(n->summary, high(x, n->u));
				if (pred_cluster == -1) //如果找不到向前的那个cluster，也就是说x所在的那个cluster之前的所有cluster都没有元素存储在里面
				{
					if (n->min != -1 && x > n->min) //如果x比当前结点的min值要大，说明当前结点存储的min就是x的前驱
					{
						return n->min;
					}
					else //当前结点没有x的前驱元素
					{
						return -1;
					}
				}
				else //找到前驱的cluster
				{
					int offset = maximum(n->cluster[pred_cluster]); //直接在前驱结点中找最大值
					return index(pred_cluster, offset, n->u); //返回前驱值
				}
			}
		}
	}
	int successor(std::shared_ptr<vEB_node> n, int x)
	{
		if (n->u == 2) //基础情况
		{
			if (x == 0 && n->max == 1)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
		else if (n->min != -1 && x < n->min) //如果x比当前结点的最小值还要小，则直接返回当前结点最小值
		{
			return n->min;
		}
		else
		{
			int max_low = maximum(n->cluster[high(x, n->u)]);
			if (max_low != -1 && low(x, n->u) < max_low) //x比当前cluster的最大值要小，后继一定在这个结点内
			{
				int offset = successor(n->cluster[high(x, n->u)], low(x, n->u));
				return index(high(x, n->u), offset, n->u);
			}
			else //后继不在这个cluster，需要向后找到一个新的cluster
			{
				int succ_cluster = successor(n->summary, high(x, n->u));
				if (succ_cluster == -1) //找不到后继的cluster
				{
					return -1;
				}
				else
				{
					int offset = minimum(n->cluster[succ_cluster]); //在后继的cluster中找最小值
					return index(succ_cluster, offset, n->u);
				}
			}
		}
	}
	void empty_insert(std::shared_ptr<vEB_node> n, int x)
	{
		n->max = x;
		n->min = x;
	}
	void insert_element(std::shared_ptr<vEB_node> n, int x)
	{
		if (n->min == -1) //如果一个结点为空，直接将x作为最小值插入
		{
			empty_insert(n, x);
		}
		else
		{
			if (x < n->min) //如果x比当前结点的最小值还要小，则需要将x和当前的最小值做替换，并把原来的最小值插入进去
			{
				int temp = x;
				x = n->min;
				n->min = temp;
			}
			if (n->u > 2) //如果当前结点的u值大于2，需要找到x的合适位置插入
			{
				if (minimum(n->cluster[high(x, n->u)]) != -1) //如果x应在的结点是非空结点
				{
					insert_element(n->cluster[high(x, n->u)], low(x, n->u));
				}
				else //x所在结点为空
				{
					insert_element(n->summary, high(x, n->u));
					empty_insert(n->cluster[high(x, n->u)], low(x, n->u));

				}
			}
			if (x > n->max) //有必要的话，需要更新结点中的max值
			{
				n->max = x;
			}
		}
	}
	void delete_element(std::shared_ptr<vEB_node> n, int x)
	{
		if (n->min == n->max) //某个结点中min=max，说明该结点中只有这一个元素，直接删除即可
		{
			n->min = -1;
			n->max = -1;
		}
		else if (n->u == 2) //结点中min和max不相等，并且结点中只有min和max
		{
			if (x == 0)
			{
				n->min = 1;
			}
			else
			{
				n->min = 0;
			}
			n->max = n->min;
		}
		else //结点的u大于2的情况，且结点中的元素不止一个
		{
			if (x == n->min) //如果要删除的元素刚好是最小值，需要先从第一个cluster中找出一个元素替换掉当前的min值，然后删除掉cluster中的那个相同元素
			{
				int first_cluster = minimum(n->summary);
				x = index(first_cluster, minimum(n->cluster[first_cluster]), n->u);
				n->min = x;
			}
			delete_element(n->cluster[high(x, n->u)], low(x, n->u));
			//删除结束之后，要做一系列修正
			if (minimum(n->cluster[high(x, n->u)]) == -1) //如果删除完成后，x所在cluster变空，需要修改相应的summary中的数值
			{
				delete_element(n->summary, high(x, n->u));
				if (x == n->max) //如果删除的元素x刚好是当前结点的最大元素，需要对最大值做更新。由于此时x所在的那个cluster已经变空，因此需要重新寻找cluster来找到最大值
				{
					int summary_max = maximum(n->summary);
					if (summary_max == -1)
					{
						n->max = n->min;
					}
					else
					{
						n->max = index(summary_max, maximum(n->cluster[summary_max]), n->u);
					}
				}
			}
			else if (x == n->max) //x所在cluster不为空，此时如果删除的值等于最大值，那么需要对最大值做修正。由于删除x之后，x所在的cluster不为空，故此时最大值还在其中
			{
				n->max = index(high(x, n->u), maximum(n->cluster[high(x, n->u)]), n->u);
			}
		}
	}
};

/*
调试代码：
	vEB_Tree test(256);
	std::default_random_engine e(std::time(0));
	std::uniform_int_distribution<int> rand_gen(0, 255);
	for (int i = 1;i < 10;i++)
	{
		int t = rand_gen(e);
		std::cout <<"insert:"<< t << std::endl;
		test.insert_element(test.get_root(),t);
	}

	std::cout << "predecessor of 5: " << test.predecessor(test.get_root(), 5) << std::endl;
	std::cout << "successor of 8: " << test.successor(test.get_root(), 8) << std::endl;
	if (test.tree_member(test.get_root(), 9))
	{
		std::cout << "element 9 exists: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;

		test.delete_element(test.get_root(), 9);
		std::cout << "element 9 does not exist: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;
	}
	else
	{
		std::cout << "element 9 does not exist: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;
		
		test.insert_element(test.get_root(), 9);
		std::cout << "element 9 exists: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;

		test.delete_element(test.get_root(), 9);
		std::cout << "element 9 does not exist: ";
		std::cout << test.tree_member(test.get_root(), 9) << std::endl;

	}
	return 0;
*/