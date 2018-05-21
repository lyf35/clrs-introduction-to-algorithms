#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<deque>
#include<memory>
#include<tuple>
#include<string>

template<typename T>struct BTreeNode
{
	std::shared_ptr<BTreeNode<T>> parent;
	std::vector<std::shared_ptr<BTreeNode<T>>> children;
	std::vector<T> element;
	bool leaf;
	BTreeNode(std::shared_ptr<BTreeNode<T>> p, std::vector<std::shared_ptr<BTreeNode<T>>> c, std::vector<T> elem, bool l) :parent(p), children(c), element(elem), leaf(l) {};
};

template<typename T> class BTree
{
private:
	std::shared_ptr<BTreeNode<T>> root;
	unsigned height;
	unsigned degree;
public:
	BTree(unsigned deg = 2) :degree(deg)
	{
		root = std::shared_ptr<BTreeNode<T>>(new BTreeNode<T>(NULL, std::vector<std::shared_ptr<BTreeNode<T>>>(), std::vector<T>(), true));
		height = 0;
	}
	std::tuple<std::shared_ptr<BTreeNode<T>>, int> find_node(T elem)//返回搜索结点所在的那个结点和位置
	{
		auto node = this->root;
		while (true) //从根结点一直往下
		{
			for (int i = 0;i < node->element.size();i++)
			{
				if (node->element[i] == elem)
				{
					return std::make_tuple(node, i);
				}
				if (node->element[i] > elem)
				{
					if (node->leaf == true)
					{
						return std::make_tuple(node, i);
					}
					else
					{
						node = node->children[i];
						break;
					}
				}
			}
			if (*(node->element.end() - 1) < elem)
			{
				if (node->leaf == true)
				{
					return std::make_tuple(node, node->element.size());
				}
				else
				{
					node = *(node->children.end() - 1);
				}
			}
		}
	}
	std::tuple<std::shared_ptr<BTreeNode<T>>,int> split_node(std::shared_ptr<BTreeNode<T>> node) //对某结点做分离操作，返回值之一是分离结点的父结点，另一个数代表分离结点中上升元素的位置
	{
		if (node->element.size() == 2 * degree - 1)
		{
			std::vector<std::shared_ptr<BTreeNode<T>>> tempvecnode;
			if (node->leaf == false)
			{
				tempvecnode.insert(tempvecnode.end(), node->children.begin() + degree, node->children.end());
			}
			std::vector<T> tempvecelem(node->element.begin() + degree, node->element.end());
			std::shared_ptr<BTreeNode<T>> newnode(new BTreeNode<T>(node->parent, tempvecnode, tempvecelem, node->leaf)); //将node的后一半孩子分给分裂出的新孩子
			if (tempvecnode.size() > 0) //注意，将node的后一半孩子分出去之后，也需要相应地修改这些孩子的父节点
			{
				for (auto i : tempvecnode)
				{
					i->parent = newnode;
				}
			}
			std::shared_ptr<BTreeNode<T>> node_p; //当前结点的父结点
			if (node->parent) //如果当前要分裂的结点不是根结点，可以直接处理。否则需要新生成一个结点作为根结点，并且高度+1
			{
				node_p = node->parent;
			}
			else
			{
				std::vector<std::shared_ptr<BTreeNode<T>>> tempvecnode2{node};
				std::vector<T> tempvecelem2;
				node_p = std::shared_ptr<BTreeNode<T>>(new BTreeNode<T>(NULL, tempvecnode2, tempvecelem2, false));
				root = node_p;
				node->parent = node_p;
				newnode->parent = node_p;
				height++;
			}
			for (auto i = node_p->element.begin();i <= node_p->element.end();i++) //将结点最中间的那个元素插入到父结点中去，并插入新孩子newnode
			{
				if (i == node_p->element.end() || (*i) > node->element[degree - 1]) //此处需要注意搜索到最右端时的情况
				{
					i = node_p->element.insert(i, node->element[degree - 1]); //注意这一步操作过后，之前的变量i已经失效，因此必须重新赋值！
					node_p->children.insert(node_p->children.begin() + (i - node_p->element.begin()) + 1, newnode);
					node->element.erase(node->element.begin() + degree - 1, node->element.end());
					if (node->leaf==false) //注意：待分裂的结点可能是叶结点，此时没有孩子
					{
						node->children.erase(node->children.begin() + degree, node->children.end());
					}
					int rank = i - node_p->element.begin();
					return std::make_tuple(node_p, rank); 
				}
			}
		}
		else
		{
			return std::make_tuple(node, 0);
		}
	}
	std::shared_ptr<BTreeNode<T>> BTree_insert(T elem) //将某个元素插入到B树中
	{
		auto node = this->root;
		while (true) //从根结点一直往下
		{
			if (node->element.size() == 2 * degree - 1) //检查当前结点是否需要分裂
			{
				auto temp = split_node(node);
				node = std::get<0>(temp);
				if (node->element[std::get<1>(temp)] > elem) //根据大小关系决定接下来从哪个孩子开始搜索
				{
					node = node->children[std::get<1>(temp)];
				}
				else
				{
					node = node->children[std::get<1>(temp) + 1];
				}
			}
			if (node->leaf == false) //如果不是叶结点，则寻找需要进一步深入搜索的那个子树
			{
				for (int i = 0;i < node->element.size();i++)
				{
					if (node->element[i] > elem)
					{
						node = node->children[i];
						break;
					}
				}
				if (*(node->element.end() - 1) < elem)
				{
					node = *(node->children.end() - 1);
				}
			}
			else if (node->leaf == true) //如果是叶节点，则在合适的位置插入，并返回
			{
				int i = 0;
				while (i < node->element.size() && node->element[i] < elem)
				{
					i++;
				}
				node->element.insert(node->element.begin() + i, elem);
				return node;
			}
		}
	}
	std::shared_ptr<BTreeNode<T>> merge_node(std::shared_ptr<BTreeNode<T>> node1, std::shared_ptr<BTreeNode<T>> node2, int m)//结点的合并操作，m代表node1和node2在父结点中，中间所夹那个元素的秩，且node2位于node1右边
	{
		auto node_p = node1->parent;
		node1->element.push_back(node_p->element[m]);
		node1->element.insert(node1->element.end(), node2->element.begin(), node2->element.end());
		if (node1->leaf == false)
		{
			node1->children.insert(node1->children.end(), node2->children.begin(), node2->children.end());
			for (auto i : node1->children) //要修改node2所有孩子的父亲指针，使其指向node1
			{
				i->parent = node1;
			}
		}
		node_p->element.erase(node_p->element.begin() + m); //删去父节点中的相应元素
		node_p->children.erase(node_p->children.begin() + m + 1); //node2在node1右边，删除node2
		if (node_p->element.size() == 0) //处理合并完成之后根结点上升的情况
		{
			node1->parent = NULL;
			root = node1;
			height--;
		}
		node2->parent = NULL;
		if (height == 0)
		{
			node1->leaf = true;
		}
		return node1;
	}
	void delete_element(T elem) //删除某个元素
	{
		auto node = root;
		int rank = -1;
		while (rank < 0) //从根结点一直往下，来寻找元素所在结点以及其位置
		{
			if (node != root&&node->element.size() < degree) //在查找当前节点时，如果节点中的元素个数过少，要先调整，以保证接下来的删除操作顺利进行
			{
				node = delete_fix(node);
			}
			for (int i = 0;i < node->element.size();i++)
			{
				if (node->element[i] == elem) //查找成功，rank代表待删除元素在当前结点中的位置
				{
					rank = i;
					break;
				}
				if (node->element[i] > elem)
				{
					if (node->leaf == true)
					{
						return;
					}
					else
					{
						node = node->children[i];
						break;
					}
				}
			}
			if (*(node->element.end() - 1) < elem)
			{
				if (node->leaf == true)
				{
					return;
				}
				else
				{
					node = *(node->children.end() - 1);
				}
			}
		}
		while (node->leaf == false) //如果元素不在叶结点中，则需要分三种情况
		{
			auto pred = node->children[rank];
			auto succ = node->children[rank + 1];
			if (pred->element.size() >= degree)//如果前驱子节点的元素个数大于树的度，则将待删除元素与前一个子节点中的最后一个元素做调换
			{
				node->element[rank] = *(pred->element.end() - 1);
				*(pred->element.end() - 1) = elem;
				rank = pred->element.size() - 1;
				node = pred;
			}
			else if (succ->element.size() >= degree) //看后继子节点的元素个数是否大于树的度，如果是，则将待删除元素与后继子节点的第一个元素做调换
			{
				node->element[rank] = *(succ->element.begin());
				*(succ->element.begin()) = elem;
				rank = 0;
				node = succ;
			}
			else //如果前驱和后继两个节点中的元素个数都过少，则先合并再删除即可直接结束
			{
				auto temp = merge_node(pred, succ, rank);
				temp->element.erase(temp->element.begin() + degree - 1);
				return;
			}
		}
		node->element.erase(node->element.begin() + rank); //此时，待删除元素已经变为叶结点中的元素，可以直接删除
		if (node != root&&node->element.size() < degree - 1) //对于非根节点来说，在下降时，已经保证父节点的元素个数足够做调整而不致引起下溢往上传播，因此可以直接修正
		{
			delete_fix(node);
		}
	}
	std::shared_ptr<BTreeNode<T>> delete_fix(std::shared_ptr<BTreeNode<T>> node)
	{
		auto node_p = node->parent;
		T tempelem = node->element[0];
		int r = 0;
		while (r < node_p->element.size()&&node_p->element[r] < tempelem)
		{
			r++;
		}
		if (r == 0) //node本身是第一个孩子的情况
		{
			auto next = node_p->children[r + 1];
			if (next->element.size() >= degree) //从右侧兄弟借一个关键字过来，同时也要记得把孩子节点转移过来
			{
				node->element.push_back(node_p->element[r]);
				node_p->element[r] = *(next->element.begin());
				next->element.erase(next->element.begin());
				if (node->children.size() > 0)
				{
					node->children.push_back(next->children[0]);
					next->children.erase(next->children.begin());
				}
			}
			else //右兄弟结点的元素数量已经达到最小值，无法再借出，只能合并
			{
				return merge_node(node, next, r);
			}
		}
		else
		{
			auto prev = node_p->children[r - 1];
			if (prev->element.size() >= degree) //从左侧的兄弟借一个关键字过来
			{
				node->element.insert(node->element.begin(), node_p->element[r - 1]);
				node_p->element[r - 1] = *(prev->element.end() - 1);
				prev->element.pop_back();
				if (node->children.size() > 0)
				{
					node->children.insert(node->children.begin(), prev->children.end() - 1, prev->children.end());
					prev->children.pop_back();
				}
			}
			else if (r < node_p->element.size()) //从右侧兄弟借一个关键字过来
			{
				auto next = node_p->children[r + 1];
				if (next->element.size() >= degree)
				{
					node->element.push_back(node_p->element[r]);
					node_p->element[r] = *(next->element.begin());
					next->element.erase(next->element.begin());
					if (node->children.size() > 0)
					{
						node->children.push_back(next->children[0]);
						next->children.erase(next->children.begin());
					}
				}
				else //左右兄弟结点的元素数量已经达到最小值，无法再借出，只能选择与左/右兄弟合并
				{
					return merge_node(prev, node, r - 1);
				}
			}
			else //没有右兄弟，只能选择与左兄弟合并
			{
				return merge_node(prev, node, r - 1);
			}
		}
		return node;
	}
	template<typename U> void level_traverse(U op) //层次遍历操作
	{
		std::deque<std::shared_ptr<BTreeNode<T>>> layer1{ root };
		std::deque<std::shared_ptr<BTreeNode<T>>> layer2;
		bool is_layer1 = true;
		while (layer1.size() > 0 || layer2.size() > 0)
		{
			if (is_layer1)
			{
				for (auto i = layer1.begin();i != layer1.end();i++)
				{
					op(*i);
					if ((*i)->leaf == false)
					{
						layer2.insert(layer2.end(), (*i)->children.begin(), (*i)->children.end());
					}
				}
				layer1.clear();
				std::cout << std::endl;
				is_layer1 = false;
			}
			else
			{
				for (auto i = layer2.begin();i != layer2.end();i++)
				{
					op(*i);
					if ((*i)->leaf == false)
					{
						layer1.insert(layer1.end(), (*i)->children.begin(), (*i)->children.end());
					}
				}
				layer2.clear();
				std::cout << std::endl;
				is_layer1 = true;
			}
		}	
	}
};

/*
调试用代码：
template<typename T> struct visit
{
	void operator()(T obj)
	{
		for (auto i : obj->element)
		{
			std::cout << i << " ";
		}
		std::cout << "|";
	}
};

int main()
{
	visit<std::shared_ptr<BTreeNode<int>>> myvisit;
	BTree<int> test(4);
	for (int i = 0;i < 50;i++)
	{
		test.BTree_insert(i);
		test.level_traverse(myvisit);
		std::cout << std::endl;
	}
	for (int i = 0;i < 15;i ++)
	{
		static std::uniform_int_distribution<int> eng(0,29);
		static std::default_random_engine e(time(0));
		int m = eng(e);
		std::cout << "delete" << m << std::endl;
		test.delete_element(m);
		test.level_traverse(myvisit);
		std::cout << std::endl;
	}
	return 0;
}
*/