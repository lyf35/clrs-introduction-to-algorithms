#pragma once
#include<iostream>
#include<algorithm>
#include<cmath>
#include"List.h"
#include<vector>
#include<memory>

template<typename T> struct Fibonacci_heap_node
{
	std::shared_ptr<Fibonacci_heap_node<T>> parent; //parent和child结点可以为空
	std::shared_ptr<Fibonacci_heap_node<T>> child;
	std::shared_ptr<Fibonacci_heap_node<T>> left; //left和right结点不能为空，如果没有左右兄弟就指向它自己
	std::shared_ptr<Fibonacci_heap_node<T>> right;
	int degree;
	T key;
	bool mark; //确定是否执行cascade_cut
	bool inside; //用于确定该结点是否在堆里面
	std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>> root_ptr; //如果某个结点在根链表内，就设置它为指向自身
	Fibonacci_heap_node<T>(std::shared_ptr<Fibonacci_heap_node<T>> p, std::shared_ptr<Fibonacci_heap_node<T>> l, std::shared_ptr<Fibonacci_heap_node<T>> r, std::shared_ptr<Fibonacci_heap_node<T>> c, int d, T k, bool stat, bool in, std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>> ptr = NULL) : parent(p), left(l), right(r), child(c), degree(d), key(k), mark(stat), inside(in), root_ptr(ptr) {};
};

template<typename T> class Fibonacci_heap
{
	friend class List<T>;
private:
	List<std::shared_ptr<Fibonacci_heap_node<T>>> root; //可以直接让root的head结点保存最小值
	int size;
public:
	Fibonacci_heap<T>() : root(circular_link), size(0) {};
	int get_size()
	{
		return size;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> insert(T elem) //在Fibonacci堆里面插入元素
	{
		std::shared_ptr<Fibonacci_heap_node<T>> temp = std::shared_ptr<Fibonacci_heap_node<T>>(new Fibonacci_heap_node<T>(NULL, NULL, NULL, NULL, 0, elem, false, true)); //新建一个结点
		temp->left = temp;
		temp->right = temp;
		root.insert_end(temp);
		temp->root_ptr = root.get_head()->prev;
		if (root.get_head()->element->key > elem) //可能需要移动head指针
		{
			root.change_head(root.get_head()->prev);
		}
		size++;
		return temp;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> insert_node(std::shared_ptr<Fibonacci_heap_node<T>> node)
	{
		root.insert_end(node);
		node->root_ptr = root.get_head()->prev;
		if (root.get_head()->element->key > node->key)
		{
			root.change_head(root.get_head()->prev);
		}
		node->left = node; //让结点的左右指针都指向自己
		node->right = node;
		node->parent = NULL;
		size++;
		node->inside = true;
		return node;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> minimum()
	{
		return root.get_head()->element;
	}
	std::shared_ptr<Fibonacci_heap_node<T>> extract_min()
	{
		auto temp = root.get_head(); //待提取的元素。注意可能此时只有这一个元素，因此删除这个结点的操作要留在最后
		size--;
		if (size >= 1)
		{
			auto temp_child = temp->element->child;
			if (temp->element->child)
			{
				auto sig = temp_child; //接下来用于判断循环结束的标志
				while (true) //把temp_child及其兄弟全部插入根列表中，并同时将其parent指针置为空。由于兄弟之间的指针是循环的，因此只需指针循环一遍即可
				{
					root.insert_end(temp_child);
					temp_child->parent = NULL;
					temp_child->root_ptr = root.get_head()->prev;
					auto i = temp_child;
					temp_child = temp_child->right; //接下来的操作会改变指针指向，因此需要先向右移一位
					i->left = i; //将分离出的结点的左右孩子改为自循环的，方便接下来的合并操作
					i->right = i;
					if (temp_child == sig)
					{
						break;
					}
				}
			}
			root.change_head(temp->next);
			root.delete_node(temp); //从链表中删去最小结点，并且最后返回它
			consolidate(); //对结点做合并
			temp->element->root_ptr = NULL; //注意返回之前记得修改最小元素结点的孩子指针和结点的度
			temp->element->child = NULL; 
			temp->element->degree = 0;
			temp->element->inside = false;
			return temp->element;
		}
		else
		{
			temp->element->root_ptr = NULL;
			temp->element->inside = false;
			return temp->element;
		}
	}
	void consolidate() //对度相同的结点做合并，直至根链表中没有度相同的结点。由于之前将结点插入根列表的时候没有修改最小值的位置，因此需要同时修改最小值的位置
	{
		if (root.get_size() <= 1) //不用合并
		{
			return;
		}
		auto temp = root.get_head();
		std::vector<std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>>> root_elem(ceil(log(size) / log(1.618) + 1), NULL); //vector中的下标就代表当前存储结点的度
		root_elem[root.get_head()->element->degree] = temp;
		temp = temp->next;
		while (temp != root.get_head()) //循环一圈之后结束
		{
			while (true) //如果vector中存储有与当前结点度相同的结点，需要将两结点做合并。由于合并之后还是有可能重复，因此需要做迭代循环
			{
				std::shared_ptr<listnode<std::shared_ptr<Fibonacci_heap_node<T>>>> x = root_elem[temp->element->degree];
				if (x) //vector中存储有和当前结点temp度一样的结点，需要做合并
				{
					root_elem[temp->element->degree] = NULL;
					if (x->element->key < temp->element->key) //每次插入操作都是将x作为孩子插入到位置靠前的temp结点上，为了保持最小堆性质，可能需要对两个listnode的元素做调换
					{
						auto temp2 = temp->element;
						temp->element = x->element;
						x->element = temp2;
					}
					if (!temp->element->child) //根据temp结点当前是否有孩子来进行不同的操作，如果没有孩子，就把temp结点作为孩子插进去
					{
						temp->element->child = x->element;
					}
					else //如果已经有孩子，就将其插入到temp的child结点的左边
					{
						x->element->right = temp->element->child;
						x->element->left = temp->element->child->left;
						temp->element->child->left->right = x->element; //注意这两条语句的顺序
						temp->element->child->left = x->element;
					}
					x->element->root_ptr = NULL;
					x->element->parent = temp->element;
					temp->element->degree++;
				}
				else //如果vector中没有与temp度数相同的结点，就把temp存入到vector中，并直接跳出循环
				{
					root_elem[temp->element->degree] = temp; //将当前元素存入vector，并指向下一元素
					temp = temp->next;
					break;
				}
			}
		}
		root = List<std::shared_ptr<Fibonacci_heap_node<T>>>(circular_link); //接下来使用数组中的元素，再对list进行重组
		for (auto i : root_elem) 
		{
			if (i)
			{
				auto temp = root.insert_end(i->element);
				temp->element->parent = NULL;
				temp->element->root_ptr = root.get_head()->prev;
				if (temp->element->key < root.get_head()->element->key)
				{
					root.change_head(temp);
				}
			}
		}
	}
	void decrease_key(std::shared_ptr<Fibonacci_heap_node<T>> p, T newval) //降低某个结点的key值
	{
		p->key = newval;
		if (!(p->parent)) //如果没有父结点，那么这个结点一定位于根链表中
		{
			if (p->key < root.get_head()->element->key) //如果结点的key小于当前根链表中最小的key，那么需要设置指针p所对应的那个结点是head
			{
				root.change_head(p->root_ptr);
			}
		}
		else if (p->key < p->parent->key) //如果不满足最小堆性质，就把它及它的孩子直接切掉放入根链表中
		{
			cut(p->parent, p);
		}
	}
	void cut(std::shared_ptr<Fibonacci_heap_node<T>> x, std::shared_ptr<Fibonacci_heap_node<T>> y) //x是父结点，y是子结点，将y从x上面拿下来并放入根链表中
	{
		if (y&&x->child == y) //如果x的孩子指针刚好是y，则需要对其做修改
		{
			if (y->left != y)
			{
				x->child = y->left;
			}
			else //相当于x只有一个孩子
			{
				x->child = NULL;
			}
		}
		y->left->right = y->right;
		y->right->left = y->left;
		y->left = y;
		y->right = y;
		y->parent = NULL;
		root.insert_end(y);
		y->root_ptr = root.get_head()->prev;
		x->degree--;
		if (y->key < root.get_head()->element->key)
		{
			root.change_head(root.get_head()->prev);
		}
		if (!x->root_ptr&&x->mark == true)
		{
			cascade_cut(x); //如果之前已经失去一个孩子的话，此时失去第二个孩子，需要调用此函数做修正
		}
		else
		{
			x->mark = true;
		}
	}
	void cascade_cut(std::shared_ptr<Fibonacci_heap_node<T>> x) //x已经失去两个孩子，删去x及它上面被标记的元素并放入根链表里面
	{
		while (x->parent) //一直上升，直到遇到某个结点的mark标记为false（此条件在cut内做判断）或至根结点
		{
			x->mark = false;
			cut(x->parent, x); //直接调用cut函数即可，如果x_p的标记为true，cut函数会自动递归调用
		}
	}
	void union_heap(Fibonacci_heap<T> f2) //将两个Fibonacci堆合并起来
	{
		auto iter = f2.root.get_head()->next;
		while (iter != f2.root.get_head())
		{
			insert_node(iter->element);
			iter = iter->next;
		}
		auto mark = insert_node(iter->element);
		if (mark->key < root.get_head()->element->key)
		{
			std::swap(root.get_head()->element, mark);
		}
		size += f2.size;
	}
	template<typename U> void traverse(U op) //对斐波那契堆中的结点做一次遍历
	{
		auto temp = root.get_head();
		traverse_node(temp->element, op);
		temp = temp->next;
		std::cout << std::endl;
		while (temp != root.get_head())
		{
			traverse_node(temp->element, op);
			std::cout << std::endl;
			temp = temp->next;
		}
	}
	template<typename U> void traverse_node(std::shared_ptr<Fibonacci_heap_node<T>> node, U op)
	{
		std::vector<std::shared_ptr<Fibonacci_heap_node<T>>> nodevec1{ node };
		std::vector<std::shared_ptr<Fibonacci_heap_node<T>>> nodevec2;
		bool vec1 = true;
		while (nodevec1.size() || nodevec2.size())
		{
			if (vec1)
			{
				for (auto i : nodevec1)
				{
					op(i);
					auto temp_child = i->child;
					auto sig = temp_child; //接下来用于判断循环结束的标志
					if (temp_child)
					{
						while (true) //把temp_child及其兄弟全部插入根列表中，并同时将其parent指针置为空。由于兄弟之间的指针是循环的，因此只需指针循环一遍即可
						{
							nodevec2.push_back(temp_child);
							temp_child = temp_child->right; //接下来的操作会改变指针指向，因此需要先向右移一位
							if (temp_child == sig)
							{
								break;
							}
						}
					}
				}
				nodevec1.clear();
				std::cout << std::endl;
				vec1 = false;
			}
			else
			{
				for (auto i : nodevec2)
				{
					op(i);
					auto temp_child = i->child;
					auto sig = temp_child; //接下来用于判断循环结束的标志
					if (temp_child)
					{
						while (true) //把temp_child及其兄弟全部插入根列表中，并同时将其parent指针置为空。由于兄弟之间的指针是循环的，因此只需指针循环一遍即可
						{
							nodevec1.push_back(temp_child);
							temp_child = temp_child->right; //接下来的操作会改变指针指向，因此需要先向右移一位
							if (temp_child == sig)
							{
								break;
							}
						}
					}
				}
				nodevec2.clear();
				std::cout << std::endl;
				vec1 = true;
			}
		}
	}
};

/*
调试代码：
template<typename T> struct visit
{
	void operator()(T obj)
	{
		std::cout << obj->key << " ";
		std::cout << "|";
	}
};

int main()
{
	visit<std::shared_ptr<Fibonacci_heap_node<int>>> myvisit;
	Fibonacci_heap<int> test;
	for (int i = 0;i < 5;i++)
	{
		test.insert(i);
	}
	Fibonacci_heap<int> test2;
	for (int i = 55;i < 60;i++)
	{
		test2.insert(i);
	}
	test.union_heap(test2);
	std::shared_ptr<Fibonacci_heap_node<int>> temp = std::shared_ptr<Fibonacci_heap_node<int>>(new Fibonacci_heap_node<int>(NULL, NULL, NULL, NULL, 0, 1000, false)); //新建一个结点
	temp->left = temp;
	temp->right = temp;
	test.insert_node(temp);
	for (int i = 0;i < 5;i ++)
	{
		test.traverse(myvisit);
		std::cout << std::endl;
		std::cout << test.minimum()->key << std::endl;
		test.extract_min();
		std::cout << std::endl;
	}
	test.decrease_key(temp, 0);
	test.traverse(myvisit);
	std::cout << std::endl;
	return 0;
}
*/