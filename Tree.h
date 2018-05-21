#pragma once
#include<memory>
//左孩子右兄弟表示任意的有根树结构代码
template<typename T> struct TreeNode
{
	T value;
	std::shared_ptr<TreeNode<T>> parent;
	std::shared_ptr<TreeNode<T>> leftchild;
	std::shared_ptr<TreeNode<T>> sibling;
	TreeNode(T num) :value(num), parent(NULL), leftchild(NULL), sibling(NULL) {};
};
template<typename T> class Tree
{
private:
	std::shared_ptr<TreeNode<T>> root;
public:
	Tree(T num) :root(std::shared_ptr<TreeNode<T>>(new TreeNode<T>(num))) {};
	std::shared_ptr<TreeNode<T>> visit_root()
	{
		return root;
	}
	std::shared_ptr<TreeNode<T>> get_leftchild(std::shared_ptr<TreeNode<T>> n)
	{
		return n->leftchild;
	}
	std::shared_ptr<TreeNode<T>> get_rightsibling(std::shared_ptr<TreeNode<T>> n)
	{
		return n->sibling;
	}
	std::shared_ptr<TreeNode<T>> get_parent(std::shared_ptr<TreeNode<T>> n)
	{
		return n->parent;
	}
	std::shared_ptr<TreeNode<T>> insert_leftchild(std::shared_ptr<TreeNode<T>> node, std::shared_ptr<TreeNode<T>> insert)
	{
		node->leftchild = insert;
		insert->parent = node;
		return insert;
	}
	std::shared_ptr<TreeNode<T>> insert_sibling(std::shared_ptr<TreeNode<T>> node, std::shared_ptr<TreeNode<T>> insert)
	{
		node->sibling = insert;
		insert->parent = node->parent;
		return insert;
	}
	std::shared_ptr<TreeNode<T>> delete_leftchild(std::shared_ptr<TreeNode<T>> node)
	{
		auto temp = node->leftchild;
		temp->parent = NULL;
		node->leftchild = NULL;
		return temp;
	}
	std::shared_ptr<TreeNode<T>> delete_sibling(std::shared_ptr<TreeNode<T>> node)
	{
		auto temp = node->sibling;
		temp->parent = NULL;
		node->sibling = NULL;
		return temp;
	}
	template<typename U> void layer_traverse(std::shared_ptr<TreeNode<T>> node, U op) //按照层的顺序将所有元素输出
	{
		List<std::shared_ptr<TreeNode<T>>> container; //使用一个list作为队列来保存待访问结点
		container.insert_begin(node);
		while (container.get_size()) //只要队列中有元素就一直循环下去
		{
			std::shared_ptr<TreeNode<T>> temp = container.delete_end();
			op(temp->value);
			if (temp->leftchild)
			{
				container.insert_begin(temp = temp->leftchild);
				while (temp->sibling)
				{
					container.insert_begin(temp = temp->sibling);
				}
			}
		}
	}
};

/*
test code:
Tree<int> test(1);
auto temp = test.insert_leftchild(test.visit_root(), std::shared_ptr<TreeNode<int>>(new TreeNode<int>(2)));
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(3)));
auto temp2 = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(4)));
temp2 = test.insert_sibling(temp2, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(5)));
temp2 = test.insert_sibling(temp2, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(6)));
temp = test.insert_leftchild(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(7)));
auto temp3 = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(8)));
temp = temp3;
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(9)));
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(10)));
temp = test.insert_leftchild(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(17)));
temp = test.insert_sibling(temp, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(18)));
temp3 = test.insert_leftchild(temp3, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(15)));
temp3 = test.insert_sibling(temp3, std::shared_ptr<TreeNode<int>>(new TreeNode<int>(16)));
visit<int> my_visit;
test.layer_traverse(test.visit_root(), my_visit);
std::cout << std::endl;
return 0;
*/