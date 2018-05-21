#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include"BinarySearchTree.h"

template<typename T> class RBTree :public BinarySearchTree<T>  //红黑树是一种特殊的二叉搜索树
{
public:
	RBTree(T elem) :BinarySearchTree(elem) {};
	std::shared_ptr<BinarytreeNode<T>> insert_element(T elem)
	{
		auto temp = search_element(elem);
		std::shared_ptr<BinarytreeNode<T>> newelem = std::shared_ptr<BinarytreeNode<T>>(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0, red));
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
		insert_fixup(newelem);
		return newelem;
	}
	void insert_fixup(std::shared_ptr<BinarytreeNode<T>> node) //插入节点为红色可能会破坏红黑树的性质，即造成两个红色节点相连，因此对于这种情况需要自下而上对红黑树做调整
	{
		/*共分为6种情况，这6种情况可分为对称的两部分。一种是插入节点位于祖父节点的左树，且它的叔节点（祖父节点右子树）是红色；第二种是插入节点为左子且位于祖父节点左树，且它的叔节点为黑色；第三种是插入节点为右子且位于祖父节点的左树，且叔节点为黑色。另外三种情况相当于上述三种情况的镜像。因此，在自下而上的时候需要针对这6种情况分别处理。*/
		while (true) 
		{
			auto p = node->parent; //父节点
			if (p->color == red&&node->color == red) //连续两个红色节点，发生冲突
			{
				auto g = p->parent; //祖父节点
				auto u = (g->leftchild == p) ? g->rightchild : g->leftchild; //叔节点
				if (u&&u->color == red) //插入节点的叔节点（祖父节点右子树）是红色，将父节点和叔节点变为黑色，同时将祖父节点变为红色。
				{
					u->color = p->color = black;
					g->color = red;
					node = g; //由于祖父节点是红色，可能发生冲突，故需要进一步处理
					if (node == visit_root())//也或者祖父节点是根节点，将其置于黑即可，然后直接退出循环。如果不是，那么循环还要从祖父节点继续进行下去
					{
						node->color = black;
						break; 
					}
				}
				else //叔节点是黑色。此时需要做旋转操作，并将旋转后所得新树的根节点设为黑色，两个子节点设为红色
				{
					if (p == g->leftchild) //父节点是左子
					{
						if (node == p->leftchild) //该节点是父节点的左子，此时需要以父节点为支点做右旋操作
						{
							right_rotate(g); //右旋之后p变为了子树的根节点
							p->color = black;
							p->leftchild->color = p->rightchild->color = red;
						}
						else //该节点是父节点的右子，需要先以父节点为支点做左旋操作，然后以g为支点做右旋操作
						{
							left_rotate(p);
							right_rotate(g);
							node->color = black;
							node->leftchild->color = node->rightchild->color = red;
						}
					}
					else //父节点是右子
					{
						if (node == p->rightchild) //该节点是父节点的右子，以父节点为支点做左旋操作即可
						{
							left_rotate(g);
							p->color = black;
							p->leftchild->color = p->rightchild->color = red;
						}
						else //该节点是父节点的左子，先以父节点为支点做右旋，再以g为支点做左旋操作即可
						{
							right_rotate(p);
							left_rotate(g);
							node->color = black;
							node->leftchild->color = node->rightchild->color = red;
						}
					}
					break; //这一步做完之后，因根节点直接为黑色，一定满足红黑树的要求，故可以直接退出循环
				}
			}
			else
			{
				break;
			}
		}
	}
	void delete_element(T elem) //基本框架与BST的删除函数相似，因为之后需要根据删除的节点是左节点还是右节点采取相关操作，而无法通过BST删除函数的返回值确定删除节点是左子或右子，故在BST删除函数的基础上重写删除函数
	{
		auto del = search_element(elem); //待删除节点
		auto node = del->parent; //记录是否要调整红黑树结构的节点，即删除元素的父节点
		orientation orit; //由于节点删除完成之后，之前位于del位置的新节点可能是null。因此用一个值来确定本来它是左子还是右子
		if (del->element == elem)
		{
			if ((!del->leftchild) && (!del->rightchild)) //如果是叶节点，可以直接删除
			{
				orit = (node->leftchild == del) ? left : right;
				delete_node(del); //函数内部已经包含了更新高度功能，无需再更新高度
			}
			else if (del->leftchild && (!del->rightchild)) //如果有左孩子但是没有右孩子，就要寻找它的前驱
			{
				auto pred = find_predecessor(elem); //前驱节点
				del->element = pred->element; //将前驱节点的值赋给del，然后删除这个前驱节点即可
				del = pred; //更新del的值
				node = del->parent;
				orit = (node->leftchild == del) ? left : right;
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
				orit = (node->leftchild == del) ? left : right;
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
			if (del->color == black&&node) //如果删除元素的颜色为黑色，那么需要对红黑树的结构做调整
			{
				delete_fixup(node, orit);
			}
		}
	}
	void delete_fixup(std::shared_ptr<BinarytreeNode<T>> node, oritation orit) //改变结构之后要改变高度
	{
		while (node)
		{
			auto lc = node->leftchild;
			auto rc = node->rightchild;
			if (orit == left) //之前被删除的节点在node左边
			{
				if (lc&&lc->color == red) //如果被删除节点现在被红色节点代替，直接设置为黑色即可
				{
					lc->color = black;
					break;
				}
				else //处理被删除节点被黑色节点代替的情况
				{
					if (rc->color == red) //如果rc是红色节点，需要做左旋操作并改变颜色，然后继续对node处理
					{
						left_rotate(node); //此时node仍是lc的父节点，但是node的父节点已经变成了rc。lc仍不满足红黑树的要求，因此需要对lc再做循环
						node->color = red;
						rc->color = black;
					}
					else //rc是黑色节点，由于lc比rc的高度小1，此时rc的左右孩子必然可以访问（即使是NULL）。此时需判断rc两个左孩子的颜色。
					{
						if (!(rc->leftchild&&rc->leftchild->color == red) && !(rc->rightchild&&rc->rightchild->color == red)) //左右都是黑色
						{
							rc->color = red;
							orit = (node == node->parent->leftchild) ? left : right; //node节点所对应的子树整体高度下降1，因此下一步循环需要替换node节点为它的父节点
							if (node == visit_root()) //如果此时node是根节点那么染黑直接退出循环即可，如果不是，那么需要进一步向上
							{
								node->color = black;
								break;
							}
							node = node->parent;
						}
						else //左右至少一个是红色
						{
							if ((rc->leftchild&&rc->leftchild->color == red) && !(rc->rightchild&&rc->rightchild->color == red)) //如果是左红右黑的情况，需要做一步右旋操作然后再处理
							{
								right_rotate(rc); 
								rc = node->rightchild;
								rc->color = black; //此时，rclc变为node的rc，需要将其染黑
							}
							rc->rightchild->color = black; //右红的情况，直接做左旋并交换node和rc的颜色即可
							std::swap(node->color, rc->color);
							left_rotate(node);
							break; //红黑树得以修复，可以直接退出
						}
					}
				}
			}
			else //被删除节点在node右边，相当于上述if的镜像情况，将if中所有的左右调换即可
			{
				if (rc&&rc->color == red) //如果被删除节点现在被红色节点代替，直接设置为黑色即可
				{
					rc->color = black;
					break;
				}
				else //处理被删除节点被黑色节点代替的情况
				{
					if (lc->color == red) //如果lc是红色节点，需要做右旋操作并改变颜色，然后继续对node处理
					{
						right_rotate(node); //此时node仍是rc的父节点，但是node的父节点已经变成了lc。lc对应的子树仍不满足红黑树的要求，因此需要对lc再做循环，但是由于lc是红色，因此再循环一次一定能退出
						node->color = red;
						lc->color = black;
					}
					else //lc是黑色节点，由于rc比lc的高度小1，此时lc的左右孩子必然可以访问（即使是NULL）。此时需判断lc两个左孩子的颜色。
					{
						if (!(lc->leftchild&&lc->leftchild->color == red) && !(lc->rightchild&&lc->rightchild->color == red)) //左右都是黑色
						{
							lc->color = red;
							orit = (node == node->parent->leftchild) ? left : right; //node节点所对应的子树整体高度下降1，因此下一步循环需要替换node节点为它的父节点
							if (node == visit_root()) //如果此时node是根节点那么染黑直接退出循环即可，如果不是，那么需要进一步向上
							{
								node->color = black;
								break;
							}
							node = node->parent;
						}
						else //左右至少一个是红色
						{
							if ((lc->rightchild&&lc->rightchild->color == red) && !(lc->leftchild&&lc->leftchild->color == red)) //如果是右红左黑的情况，需要做一步左旋操作然后再处理
							{
								left_rotate(rc);
								lc = node->leftchild;
								lc->color = black; //此时，lcrc变为node的lc，需要将其染黑
							}
							lc->leftchild->color = black; //左红的情况，直接做右旋并交换node和lc的颜色即可
							std::swap(node->color, lc->color);
							right_rotate(node);
							break; //红黑树得以修复，可以直接退出
						}
					}
				}
			}
		}
	}
};


/*
红黑树的测试代码（也可以用于AVL树）：
template<typename T> class visit
{
public:
	void operator()(T elem)
	{
		std::cout << elem->element << '(' << elem->height << ')' << '(' << elem->color << ')' << '\t';
	}
};

int main()
{
	visit<std::shared_ptr<BinarytreeNode<int>>> myvisit;
	RBTree<int> test(100);
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
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(25);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(65);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(70);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(50);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(100);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;


	test.delete_element(60);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(68);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(30);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;

	test.delete_element(30);
	test.level_traverse(test.visit_root(), myvisit);
	std::cout << std::endl;
	std::cout << std::endl;
	return 0;
}
*/