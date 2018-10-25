#pragma once
#include<iostream>
#include<vector>
#include<list>
#include<algorithm>
#include<memory>
#include"Binarytree.h"

template<typename T> class BinarySearchTree :public Binarytree<T>
{
public:
	BinarySearchTree(T elem) :Binarytree(elem) {};
	std::shared_ptr<BinarytreeNode<T>> search_element(T elem)
	{
		auto temp = visit_root();
		while (true)
		{
			if (elem > temp->element)
			{
				if (temp->rightchild)
				{
					temp = temp->rightchild;
				}
				else
				{
					return temp;
				}
			}
			else if(elem < temp->element)
			{
				if (temp->leftchild)
				{
					temp = temp->leftchild;
				}
				else
				{
					return temp;
				}
			}
			else
			{
				if (temp->leftchild&&temp->leftchild->element == elem)
				{
					temp = temp->leftchild;
				}
				else
				{
					return temp;
				}
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> search_element_recursive(std::shared_ptr<BinarytreeNode<T>> node, T elem)
	{
		if (node->element > elem&&node->leftchild)
		{
			search_element_recursive(node->leftchild, elem);
		}
		else if (node->element < elem&&node->rightchild)
		{
			search_element_recursive(node->rightchild, elem);
		}
		else
		{
			return node;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> insert_element(T elem)
	{
		auto temp = search_element(elem);
		std::shared_ptr<BinarytreeNode<T>> newelem(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0));
		if (elem > temp->element)
		{
			insert_rightchild(temp, newelem); //插入函数已经包含了更新高度的命令
		}
		else if (elem < temp->element)
		{
			insert_leftchild(temp, newelem);
		}
		else
		{
			auto temp2 = temp->leftchild;
			temp = insert_leftchild(temp, newelem);
			if (temp2)
			{
				temp2 = insert_leftchild(temp, temp2);
			}

		}
		return newelem;
	}
	std::shared_ptr<BinarytreeNode<T>> find_minimum(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (node->leftchild)
		{
			node = node->leftchild;
		}
		return node;
	}
	std::shared_ptr<BinarytreeNode<T>> find_maximum(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (node->rightchild)
		{
			node = node->rightchild;
		}
		return node;
	}
	std::shared_ptr<BinarytreeNode<T>> find_minimum_recursive(std::shared_ptr<BinarytreeNode<T>> node)
	{
		if(node->leftchild)
		{
			node = node->leftchild;
			find_mininum_recursive(node);
		}
		else
		{
			return node;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> find_maximum_recursive(std::shared_ptr<BinarytreeNode<T>> node)
	{
		if (node->rightchild)
		{
			node = node->rightchild;
			find_maximum_recursive(node);
		}
		else
		{
			return node;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> find_successor(T elem)
	{
		auto temp = search_element(elem);
		if (temp->rightchild)
		{
			return find_minimum(temp->rightchild);
		}
		else
		{
			if (temp->parent&&temp == temp->parent->leftchild) //一定要先确认父指针是否存在！
			{
				return temp->parent;
			}
			else
			{
				while (temp->parent&&temp == temp->parent->rightchild)
				{
					temp = temp->parent;
				}
				temp = temp->parent;
				return temp;
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> find_predecessor(T elem)
	{
		auto temp = search_element(elem);
		if (temp->leftchild)
		{
			return find_maximum(temp->leftchild);
		}
		else
		{
			if (temp->parent&&temp == temp->parent->rightchild)
			{
				return temp->parent;
			}
			else
			{
				while (temp->parent&&temp == temp->parent->leftchild)
				{
					temp = temp->parent;
				}
				temp = temp->parent;
				return temp;
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> delete_element(T elem) //二叉搜索树的删除元素程序，返回删除节点的父节点以便接下来进行高度调整等操作
	{
		auto del = search_element(elem); //待删除节点
		auto node = del->parent;
		if (del->element == elem)
		{
			if ((!del->leftchild) && (!del->rightchild)) //如果是叶节点，可以直接删除
			{
				delete_node(del); //函数内部已经包含了更新高度功能，无需再更新高度
			}
			else if (del->leftchild && (!del->rightchild)) //如果有左孩子但是没有右孩子，就要寻找它的前驱
			{
				auto pred = find_predecessor(elem); //前驱节点
				del->element = pred->element; //将前驱节点的值赋给del，然后删除这个前驱节点即可
				del = pred; //更新del的值
				node = del->parent;
				if (del->leftchild) //如果前驱还有左孩子，即前驱不是叶节点，根据中序遍历规则，此时这个前驱一定没有右孩子
				{
					auto temp = del->leftchild;
					temp->parent = del->parent; //用pred的左孩子替换自己的位置，并将pred的所有指针架空
					if (del->parent->rightchild == del)
					{
						del->parent->rightchild = temp;
						del->parent = del->rightchild = NULL;
					}
					else
					{
						del->parent->leftchild = temp;
						del->parent = del->leftchild = NULL;
					}
					updateheightabove(temp); //需要做更新高度操作
				}
				else //如果前驱没有左孩子，那么根据中序遍历的规则，先驱一定是叶节点
				{
					delete_node(del);
				}
			}
			else if (del->rightchild) //如果节点有右孩子，就在右孩子中寻找它的后继
			{
				auto succ = find_successor(elem);
				del->element = succ->element; //更新元素的值，并更新del为succ节点
				del = succ;
				node = del->parent;
				if (!del->rightchild) //根据中序遍历规则，如果后继没有右孩子，则必定是叶节点，直接删掉即可
				{
					delete_node(del);
				}
				else //后继有右孩子，则要将右孩子对应子树作为右孩子接入到这个后继节点上
				{
					auto temp = del->rightchild;
					temp->parent = del->parent;
					if (del->parent->rightchild == del)
					{
						del->parent->rightchild = temp;
						del->parent = succ->rightchild = NULL;
					}
					else
					{
						del->parent->leftchild = temp;
						del->parent = del->leftchild = NULL;
					}
					updateheightabove(temp);
				}
			}
		}
		return node;
	}
	//上面delete_element的另一种写法（clrs的伪代码改编而来）
	void transplant(std::shared_ptr<BinarytreeNode<T>> node1, std::shared_ptr<BinarytreeNode<T>> node2) //用node2替换node1作为孩子节点
	{
		if (node1->parent->leftchild == node1)
		{
			node1->parent->leftchild = node2;
			node2->parent = node1->parent;
			node1->parent = NULL;
			updateheightabove(node2);
		}
		else if (node1->parent->rightchild == node1)
		{
			node1->parent->rightchild = node2;
			node2->parent = node1->parent;
			node1->parent = NULL;
			updateheightabove(node2);
		}
	}
	void delete_element_v2(T elem)
	{
		auto temp = search_element(elem);
		if (temp->element == elem)
		{
			if (!temp->leftchild&&temp->rightchild)
			{
				transplant(temp, temp->rightchild);
				return;
			}
			else if (temp->leftchild&&!temp->rightchild)
			{
				transplant(temp, temp->leftchild);
				return;
			}
			else if (temp->leftchild&&temp->rightchild)
			{
				auto succ = find_successor(temp);
				if (succ == temp->rightchild)
				{
					transplant(temp, succ);
				}
				else
				{
					transplant(succ, succ->rightchild);
					temp->element = succ->element;
				}
			}
			else
			{
				delete_node(temp);
			}
		}
	}
};



/*
调试用代码：
template<typename T> class visit
{
public:
	void operator()(T elem)
	{
		std::cout << elem->element << '(' << elem->height << ')' << '\t';
	}
};

int main()
{
	visit<std::shared_ptr<BinarytreeNode<int>>> myvisit;
	BinarySearchTree<int> test(100);
	test.insert_element(50);
	test.insert_element(70);
	test.insert_element(30);
	test.insert_element(35);
	test.insert_element(30);
	test.insert_element(38);
	test.insert_element(30);
	test.insert_element(25);
	test.insert_element(32);
	test.insert_element(37);
	test.insert_element(39);
	test.insert_element(60);
	test.insert_element(65);
	test.insert_element(69);
	test.insert_element(68);
	test.insert_element(62);
	test.insert_element(600);
	test.insert_element(205);
	test.insert_element(300);
	test.delete_element(25);
	test.level_traverse(test.visit_root(), myvisit);

	std::cout << std::endl;
	test.delete_element(70);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	test.delete_element(50);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	test.inorder_traverse_recursive(test.visit_root(), myvisit);
	std::cout << std::endl;
	return 0;
}
*/
