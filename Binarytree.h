#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include"List.h"

 //二叉树结构的代码，可以在后续通过继承的方式生成二叉搜索树，红黑树等结构
enum RBcolor { red, black };
enum orientation { left, right };
template<typename T> struct BinarytreeNode :public std::enable_shared_from_this<BinarytreeNode<T>>
{
	RBcolor color;
	std::shared_ptr<BinarytreeNode<T>> parent;
	std::shared_ptr<BinarytreeNode<T>> leftchild;
	std::shared_ptr<BinarytreeNode<T>> rightchild;
	T element;
	int height;
	void pointer_fixed(orientation x = left) //如果用子树组合出一个新树，则需要运行此代码
	{
		if (parent)
		{
			if (x == left)
			{
				parent->leftchild = shared_from_this();
			}
			else if (x == right)
			{
				parent->rightchild = shared_from_this();
			}
		}
		if (leftchild)
		{
			leftchild->parent = shared_from_this();
		}
		if (rightchild)
		{
			rightchild->parent = shared_from_this();
		}
	}
	BinarytreeNode(std::shared_ptr<BinarytreeNode<T>> p = NULL, std::shared_ptr<BinarytreeNode<T>> lc = NULL, std::shared_ptr<BinarytreeNode<T>> rc = NULL, T elem = 0, int h = 0, RBcolor c = black) :parent(p), leftchild(lc), rightchild(rc), element(elem), height(h), color(c) {};
};
template<typename U> bool operator<(BinarytreeNode<U> a, BinarytreeNode<U> b)
{
	return a.element < b.element;
}
template<typename U> bool operator>(BinarytreeNode<U> a, BinarytreeNode<U> b)
{
	return a.element > b.element;
}
template<typename U> bool operator==(BinarytreeNode<U> a, BinarytreeNode<U> b)
{
	return a.element == b.element;
}

template<typename T> class Binarytree
{
private:
	std::shared_ptr<BinarytreeNode<T>> root;
public:
	Binarytree(std::shared_ptr<BinarytreeNode<T>> node) :root(node) {};
	Binarytree(T elem)
	{
		root = std::shared_ptr<BinarytreeNode<T>>(new BinarytreeNode<T>(NULL, NULL, NULL, elem, 0));
	}
	std::shared_ptr<BinarytreeNode<T>> visit_root()
	{
		return root;
	}
	void updateheightabove(std::shared_ptr<BinarytreeNode<T>> node) //更新树高
	{
		while (node)
		{
			int lh = node->leftchild ? node->leftchild->height : -1;
			int rh = node->rightchild ? node->rightchild->height : -1;
			int i = lh > rh ? lh : rh;
			node->height = ++i;
			node = node->parent;
		}
	}
	int find_height(std::shared_ptr<BinarytreeNode<T>> node)
	{
		if (node)
		{
			return node->height;
		}
		else
		{
			return -1;
		}
	}
	std::shared_ptr<BinarytreeNode<T>> insert_leftchild(std::shared_ptr<BinarytreeNode<T>> node, std::shared_ptr<BinarytreeNode<T>> lc)
	{
		node->leftchild = lc;
		lc->parent = node;
		updateheightabove(lc);
		return lc;
	}
	std::shared_ptr<BinarytreeNode<T>> insert_rightchild(std::shared_ptr<BinarytreeNode<T>> node, std::shared_ptr<BinarytreeNode<T>> rc)
	{
		node->rightchild = rc;
		rc->parent = node;
		updateheightabove(rc);
		return rc;
	}
	std::shared_ptr<BinarytreeNode<T>> find_leftchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->leftchild;
	}
	std::shared_ptr<BinarytreeNode<T>> find_rightchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->rightchild;
	}
	std::shared_ptr<BinarytreeNode<T>> find_parent(std::shared_ptr<BinarytreeNode<T>> node)
	{
		return node->parent;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_leftchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->leftchild;
		node->leftchild = NULL;
		if (temp)
		{
			temp->parent = NULL;
			updateheightabove(node->rightchild);
		}
		else if (!node->rightchild)
		{
			updateheightabove(node);
		}
		return temp;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_rightchild(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->rightchild;
		node->rightchild = NULL;
		if (temp)
		{
			temp->parent = NULL;
			updateheightabove(node->leftchild);
		}
		else if(!node->leftchild)
		{
			updateheightabove(node);
		}
		return temp;
	}
	std::shared_ptr<BinarytreeNode<T>> delete_node(std::shared_ptr<BinarytreeNode<T>> node)
	{
		auto temp = node->parent;
		if (node != visit_root() && temp->leftchild==node)
		{
			temp->leftchild = NULL;
			node->parent = NULL;
			updateheightabove(temp);
		}
		if (node != visit_root() && temp->rightchild==node)
		{
			temp->rightchild = NULL;
			node->parent = NULL;
			updateheightabove(temp);
		}
		return node;
	}
	//递归形式的三种遍历
	template<typename U> void preorder_traverse_recursive (std::shared_ptr<BinarytreeNode<T>> node, U op)//U可以是任意函数，或是具有（）运算符的类
	{
		op(node);
		if (node->leftchild.get())
		{
			preorder_traverse_recursive(node->leftchild, op);
		}
		if (node->rightchild.get())
		{
			preorder_traverse_recursive(node->rightchild, op);
		}
	}
	template<typename U> void inorder_traverse_recursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		if (node->leftchild.get())
		{
			inorder_traverse_recursive(node->leftchild, op);
		}
		op(node);
		if (node->rightchild.get())
		{
			inorder_traverse_recursive(node->rightchild, op);
		}
	}
	template<typename U> void postorder_traverse_recursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		if (node->leftchild.get())
		{
			postorder_traverse_recursive(node->leftchild, op);
		}
		if (node->rightchild.get())
		{
			postorder_traverse_recursive(node->rightchild, op);
		}
		op(node);
	}
	//下面是非递归形式的四种遍历
	template<typename U> void preorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container; //只使用list的insert_end和delete_end函数，其实相当于是在使用栈，以下同理
		container.insert_end(NULL);
		while (node)
		{
			op(node);
			if (find_rightchild(node).get())
			{
				container.insert_end(find_rightchild(node));
			}
			if (!(node = find_leftchild(node)).get())
			{
				node = container.delete_end();
			}
		}
	}
	void leftmost_trace(std::shared_ptr<BinarytreeNode<T>> node, List<std::shared_ptr<BinarytreeNode<T>>>& container)
	{
		while (node)
		{
			container.insert_end(node);
			node = find_leftchild(node);
		}
	}
	template<typename U> void inorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_end(NULL);
		leftmost_trace(node, container);
		while (true) //一定要注意，一条一直沿着最左侧向下搜索的支路要保证在整个过程中只搜索一遍！
		{
			node = container.delete_end();
			if (node != NULL)
			{
				op(node);
			}
			else
			{
				break;
			}
			if (find_rightchild(node))
			{
				leftmost_trace(find_rightchild(node), container);
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> leftmost_trace_v2(std::shared_ptr<BinarytreeNode<T>> node)
	{
		while (find_leftchild(node))
		{
			node = find_leftchild(node);
		}
		return node;
	}
	template<typename U> void inorder_traverse_nonrecursive_v2(std::shared_ptr<BinarytreeNode<T>> node, U op) //不使用栈作为临时存储的递归
	{
		node = leftmost_trace_v2(node); //从整个树的最左子节点开始访问
		while(node) //只要node不是空结点就一直循环下去
		{
			op(node);
			if (find_rightchild(node)) //如果node有右子树，就直接深入右子树的最左节点开始访问
			{
				node = leftmost_trace_v2(find_rightchild(node)); 
			}
			else //如果node没有右子树，那么需要回溯
			{
				while (find_parent(node)->rightchild == node)
				{
					node = find_parent(node); //如果node是右孩子，要一直向上追溯
					if (node == visit_root())
					{
						return;
					}
				}
				node = find_parent(node); //退出上一步while循环的时候，node已经是左孩子，此时这个node一定访问过，因此再向上追溯一步
			}
		}
	}
	void insert_container(std::shared_ptr<BinarytreeNode<T>> p, List<std::shared_ptr<BinarytreeNode<T>>>& container)
	{
		while(p)
		{
			if (find_leftchild(p))
			{
				if (find_rightchild(p))
				{
					container.insert_end(find_rightchild(p));
				}
				container.insert_end(p = find_leftchild(p));
			}
			else
			{
				container.insert_end(p = find_rightchild(p));
			}
		}
		container.delete_end();
	}
	template<typename U> void postorder_traverse_nonrecursive(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_end(node);
		while (container.get_size())
		{
			if (container.get_end()->element != node->parent) //若栈顶节点不是当前结点的父亲，则需要对栈顶节点所在位置做搜索。也就是说，此时栈顶节点是右节点，而当前节点是它的左节点
			{
				insert_container(container.get_end()->element, container);
			}
			node = container.delete_end();
			op(node);
		}
	}
	template<typename U> void level_traverse(std::shared_ptr<BinarytreeNode<T>> node, U op)
	{
		List<std::shared_ptr<BinarytreeNode<T>>> container;
		container.insert_begin(node);
		while (container.get_beg() != container.get_tail())
		{
			node = container.delete_end();
			op(node);
			if (node->leftchild)
			{
				container.insert_begin(node->leftchild);
			}
			if (node->rightchild)
			{
				container.insert_begin(node->rightchild);
			}
		}
	}
	std::shared_ptr<BinarytreeNode<T>> left_rotate(std::shared_ptr<BinarytreeNode<T>> x) //x为根节点，对x做左旋操作，使x变为左孩子节点，x的右孩子y上升为根节点，并将y的左孩子接入x的右孩子处
	{
		auto y = x->rightchild; //y替换x的位置
		if (y == NULL) //如果y是叶节点则下面的步骤不用再进行下去
		{
			return NULL;
		}
		auto y_lc = y->leftchild; //y的左孩子，要接入到x的右孩子
		if (x == root)
		{
			root = y;
			root->parent = NULL;
		}
		else if (x->parent->leftchild == x)
		{
			x->parent->leftchild = y;
			y->parent = x->parent;
		}
		else if (x->parent->rightchild == x)
		{
			x->parent->rightchild = y;
			y->parent = x->parent;
		}
		x->parent = y;
		y->leftchild = x;
		x->rightchild = y_lc;
		if (y_lc)
		{
			y_lc->parent = x;
		}
		updateheightabove(y->rightchild);
		updateheightabove(x);
		return x->parent;
	}
	std::shared_ptr<BinarytreeNode<T>> right_rotate(std::shared_ptr<BinarytreeNode<T>> x) //x为根节点，对x做右旋操作，使x变为右孩子节点，x的左孩子y上升为根节点，并将y的右孩子接入x的左孩子处，相当于left-rotate的镜像操作
	{
		auto y = x->leftchild; //y要替换x的位置
		if (y == NULL)
		{
			return NULL;
		}
		auto y_rc = y->rightchild; //y的右孩子，要接入到x的左孩子
		if (x == root)
		{
			root = y;
			root->parent = NULL;
		}
		else if (x->parent->leftchild == x)
		{
			x->parent->leftchild = y;
			y->parent = x->parent;
		}
		else if (x->parent->rightchild == x)
		{
			x->parent->rightchild = y;
			y->parent = x->parent;
		}
		x->parent = y;
		y->rightchild = x;
		x->leftchild = y_rc;
		if (y_rc)
		{
			y_rc->parent = x;
		}
		updateheightabove(y->leftchild);
		updateheightabove(x);
		return x->parent;
	}
};

/*
测试使用的主函数代码：
Binarytree<int> test(1);
auto temp = test.insert_leftchild(test.visit_root(), std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 2)));
auto temp2 = test.insert_rightchild(test.visit_root(), std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 3)));
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 4)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 5)));
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 6)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 7)));
temp = test.find_leftchild(temp);
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 8)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 9)));
temp = test.find_rightchild(test.find_parent(temp));
test.insert_leftchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 10)));
test.insert_rightchild(temp, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 11)));
temp2 = test.find_leftchild(temp2);
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 12)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 13)));
temp2 = test.find_rightchild(test.find_parent(temp2));
test.insert_leftchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 14)));
test.insert_rightchild(temp2, std::shared_ptr<BinarytreeNode<int>>(new BinarytreeNode<int>(NULL, NULL, NULL, 15)));
visit<int> my_visit;
test.preorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.preorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.inorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.inorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;
test.inorder_traverse_nonrecursive_v2(test.visit_root(), my_visit);
test.postorder_traverse_recursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.postorder_traverse_nonrecursive(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

test.level_traverse(test.visit_root(), my_visit);
std::cout << "success!" << std::endl;

*/