#pragma once
#include<iostream>
#include<sstream>
#include<string>
#include<vector>
#include<list>
#include<algorithm>
#include<cmath>
#include<memory>
#include"chapter10.h"

template<typename T> class BinarySearchTree :public Binarytree<T>
{
public:
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
			else if (elem < temp->element)
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
				return temp;
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
		std::shared_ptr<BinarytreeNode<T>> newelem = new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0);
		if (elem > temp->element)
		{
			insert_rightchild(temp, newelem);
			updateheightabove(newelem);
		}
		else
		{
			insert_leftchild(temp, newelem);
			updateheightabove(newelem);
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
			if (temp == temp->parent->leftchild)
			{
				return temp->parent;
			}
			else
			{
				while (temp == temp->parent->rightchild)
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
			return find_minimum(temp->leftchild);
		}
		else
		{
			if (temp == temp->parent->rightchild)
			{
				return temp->parent;
			}
			else
			{
				while (temp == temp->parent->leftchild)
				{
					temp = temp->parent;
				}
				temp = temp->parent;
				return temp;
			}
		}
	}
	void delete_element(T elem)
	{
		auto temp = search_element(elem);
		if (temp->element == elem)
		{
			if (!temp->leftchild&&!temp->rightchild) //如果是叶节点就直接删掉即可
			{
				delete_node(temp); //函数内部已经包含了更新高度功能，无需再进行操作
				return;
			}
			else if (temp->leftchild&&!temp->rightchild) //如果有左孩子但是没有右孩子，就要寻找它的前驱
			{
				auto pred = find_predecessor(elem);
				temp->element = pred->element;
				if (pred->leftchild) //如果前驱还有左孩子，即前驱不是叶节点，根据中序遍历规则，此时这个前驱一定没有右孩子
				{
					auto temp2 = pred->leftchild;
					temp2->parent = pred->parent; //用pred的左孩子替换自己的位置，并将pred的所有指针架空
					pred->parent->leftchild = temp2;
					pred->parent = pred->leftchild= NULL;
					updateheightabove(temp2); //需要做更新高度操作
				}
				else //如果前驱没有左孩子，那么根据中序遍历的规则，先驱一定是叶节点
				{
					delete_node(pred);
				}
				return;
			}
			else if (temp->rightchild) //如果节点有右孩子，就在右孩子中寻找它的后继
			{
				auto succ = find_successor(elem);
				temp->element = succ->element;
				if (!succ->rightchild) //根据中序遍历规则，如果后继没有右孩子，则必定是叶节点，直接删掉即可
				{
					delete_node(succ);
				}
				else //后继有右孩子，则要将右孩子对应子树作为右孩子接入到这个后继节点上
				{
					auto temp2 = succ->rightchild;
					temp2->parent = succ->parent;
					succ->parent->rightchild = temp2;
					succ->parent = succ->rightchild = NULL;
					updateheightabove(temp2);
				}
				return;
			}
		}
	}
	/*
	上面delete_element的另一种写法（clrs的伪代码改编而来）
	void transplant(std::shared_ptr<BinarytreeNode<T>> node1, std::shared_ptr<BinarytreeNode<T>> node2) //用node2替换node1作为孩子节点
	{
		if (node1->parent->leftchild == node1)
		{
			node1->parent->leftchild = node2;
			node2->parent = node1->parent;
			node1->parent = NULL;
			updateheightabove(node2);
		}
		else if(node1->parent->rightchild == node1)
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
	*/

};
