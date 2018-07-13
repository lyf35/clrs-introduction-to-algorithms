#pragma once

//以下是clrs 10.2-8题目的代码
template<typename T> struct listnode_np
{
	listnode_np<T>* np; //np指针只和它前一位和后一位变量自身的地址有关，与其它listnode的的np值无关。这一指针只会根据它前后变量是哪一个而改变，而且不会影响其它变量。经过尝试，这里使用标准库的智能指针程序无法编译通过，因为做异或运算的时候，要先将智能指针变为普通指针，并且最后还要变回去，在写异或运算符的时候最后类型对不上。
	T element;
	listnode_np<T>() = default;
	listnode_np<T>(listnode_np<T>* ptr, T elem) : np(ptr), element(elem) {};
	listnode_np<T>* operator = (listnode_np<T>* a)
	{
		np = a->np;
		element = a->element;
		return this;
	}
	~listnode_np()
	{
		delete np;
	}
};
template<typename T> class list_np //只使用一个指针来同时包含np的prev和next信息，是一种拿时间换空间的策略
{
	template<typename U> friend listnode_np<T>* my_xor(const listnode_np<T>* left, const listnode_np<T>* right);
	template<typename U> friend listnode_np<T>* prev(const listnode_np<T>* left, const listnode_np<T>* right);
	template<typename U> friend listnode_np<T>* next(const listnode_np<T>* left, const listnode_np<T>* right);
private:
	listnode_np<T>* head;
	listnode_np<T>*	tail;
	listnode_np<T>* null;
	int size;
public:
	list_np() = default;
	void init()
	{
		head = new listnode_np<T>();
		tail = new listnode_np<T>();
		null = new listnode_np<T>();
		size = 0;
		head->np = my_xor(null, tail);
		tail->np = my_xor(null, head);
	}
	void copy(list_np l)
	{
		head = l.head;
		tail = l.tail;
		size = l.size;
		null = l.null;
	}
	void insert_end(T elem); //在队列的最后插入元素
	void insert_begin(T elem); //在队列的前端插入元素，原理与在队列后插入一样
	void insert_before(T elem, int index); //在index指示位置的前端插入元素
	T get(int index); //索引某一位置的元素
	T delete_begin(); //删除第一个元素
	T delete_end();
	T delete_pos(int index);
	void print(std::ostream& os);
	void reverse();
};
template<typename T> listnode_np<T>* my_xor(const listnode_np<T>* left, const listnode_np<T>* right)//ptr类型不能直接做异或运算，因此要先将指针转换类型，让编译器以为是在操纵unsigned类型的指针并调用自带的异或运算符。计算完成之后再将类型转为ptr
{
	return (listnode_np<T>*)((unsigned long(left)) ^ (unsigned long(right))); //转成unsigned long是因为c++中unsigned long和指针类型的字节数相等
}
template<typename T> listnode_np<T>* next(const listnode_np<T>* left, const listnode_np<T>* curr) //通过curr和curr左边的指针寻找curr右边的指针
{
	return my_xor(curr->np, left);
}
template<typename T> listnode_np<T>* prev(const listnode_np<T>* curr, const listnode_np<T>* right)//通过curr和curr右边的指针寻找curr左边的指针。以后注意这种需要多次使用的表达式可以专门为它们写一个函数，这样便于写程序，也便于修改程序，而不是在每一次使用的时候都写一个特别复杂的表达式。
{
	return my_xor(curr->np, right);
}
/*
运算符也可以直接使用模板类型，可以让模板自己去判断T到底是什么，比较方便

以下语句放在类定义中：
template<typename U> friend U my_xor(const U& left, const U& right);
template<typename U> friend U prev(const U& left, const U& right);
template<typename U> friend U next(const U& left, const U& right);

以下语句放在类定义外面：
template<typename T> T my_xor(const T& left, const T& right)
{
return (T)((unsigned long(left)) ^ (unsigned long(right)));
}
template<typename T> T next(const T& left, const T& curr)
{
return my_xor(curr->np, left);
}
template<typename T> T prev(const T& curr, const T& right)
{
return my_xor(curr->np, right);
}
*/
template<typename T> void list_np<T>::insert_end(T elem) //在队列的最后插入元素
{
	listnode_np<T>* temp = new listnode_np<T>();
	temp->np = my_xor(tail, prev(tail, null)); //先算temp的指针
	temp->element = elem;
	prev(tail, null)->np = my_xor(temp, prev(prev(tail, null), tail)); //如果在之前tail前面已经有元素，则需要修改它前面那个元素的指针。(((tail->np) ^ NULL)->np)代表修改之前tail前面的元素，((((tail->np) ^ NULL)->np) ^ tail)代表修改前tail前倒数第2个元素
	tail->np = my_xor(null, temp);
	size++;
}
template<typename T> void list_np<T>::insert_begin(T elem) //在队列的前端插入元素，原理与在队列后插入一样
{
	listnode_np<T>* temp = new listnode_np<T>();
	temp->np = my_xor(head, next(null, head));
	temp->element = elem;
	next(null, head)->np = my_xor(temp, next(head, next(null, head))); //如果在之前head后面已经有元素，则需要修改它后面那个元素的指针
	head->np = my_xor(null, temp);
	size++;
}
template<typename T> void list_np<T>::insert_before(T elem, int index) //在index指示位置的前端插入元素
{
	listnode_np<T>* temp = new listnode_np<T>();
	temp->element = elem;
	listnode_np<T>* ptr = head;
	listnode_np<T>* tempptr = ptr;
	listnode_np<T>* ptrb = null; //代表ptr的后面一个元素
	for (int i = 0;i <= index;i++)
	{
		tempptr = ptr;
		ptr = next(ptrb, ptr); //将ptr向前移动一位
		ptrb = tempptr; //将ptrb向前移动一位，每次循环结束之后ptrb与tempptr所指的元素相同
	}
	temp->np = my_xor(ptrb, ptr);
	ptrb->np = my_xor(temp, prev(ptrb, ptr));
	ptr->np = my_xor(temp, next(tempptr, ptr));
	size++;
}
template<typename T>T list_np<T>::get(int index) //索引某一位置的元素
{
	listnode_np<T>* ptr = head; //代表表头位置
	listnode_np<T>* ptrb = null; //代表ptr的后面一个元素
	for (size_t i = 0;i <= index;i++)
	{
		auto tempptr = ptr;
		ptr = next(ptrb, ptr); //将ptr向前移动一位
		ptrb = tempptr; //将ptrb向前移动一位
	}
	return ptr->element;
}
template<typename T> T list_np<T>::delete_begin() //删除第一个元素
{
	if (size > 0)
	{
		listnode_np<T>* ptr = next(null, head); //代表第一个元素，即待删除元素
		next(head, ptr)->np = my_xor(head, next(ptr, next(head, ptr)));//((ptr->np)^head)是ptr的向前一个元素，((((ptr->np) ^ head)->np)^ptr)是ptr向前两个元素
		head->np = my_xor(null, next(head, ptr));
		size--;
		return ptr->element;
	}
	return -1;
}
template<typename T> T list_np<T>::delete_end() //删除最后一个元素
{
	if (size > 0)
	{
		listnode_np<T>* ptr = prev(tail, null); //代表最后一个元素，待删除元素
		prev(tail, null)->np = my_xor(tail, prev(prev(ptr, tail), tail));//((ptr->np)^tail)是ptr的向后一个元素，((((ptr->np) ^ tail)->np)^ptr)是ptr向后两个元素
		tail->np = my_xor(null, prev(tail, null));
		size--;
		return ptr->element;
	}
	return -1;
}
template<typename T> T list_np<T>::delete_pos(int index) //删除index指示的元素
{
	if (size > 0)
	{
		listnode_np<T>* ptr = next(null, head); //代表第一个元素，循环之后变为要删除的元素
		listnode_np<T>* tempptr;
		listnode_np<T>* ptrb = head; //代表head，即ptr的后面一个元素
		for (int i = 0;i < index;i++)
		{
			tempptr = ptr;
			ptr = next(ptrb, ptr); //将ptr向前移动一位
			ptrb = tempptr; //将ptrb向前移动一位，每次循环结束之后ptrb与tempptr所指的元素相同
		}
		next(ptrb, ptr)->np = my_xor(ptrb, next(ptr, next(ptrb, ptr)));//((ptr->np) ^ ptrb)是ptr向前一个元素，((((ptr->np) ^ ptrb)->np) ^ ptr)是ptr向前两个元素
		ptrb->np = my_xor(prev(ptrb, ptr), next(ptrb, ptr)); //((ptrb->np) ^ ptr)是ptr向后两个元素
		size--;
		return ptr->element;
	}
	return -1;
}
template<typename T> void list_np<T>::print(std::ostream& os)
{
	listnode_np<T>* ptr = head; //代表表头位置
	listnode_np<T>* ptrb = null; //代表ptr的后面一个元素
	for (size_t i = 0;i < size;i++)
	{
		auto tempptr = ptr;
		ptr = next(ptrb, ptr); //将ptr向前移动一位
		ptrb = tempptr; //将ptrb向前移动一位
		os << ptr->element << '\t';
	}
	os << std::endl;
}
template<typename T> void list_np<T>::reverse() //要将表逆转，只需要将头尾的指针调换即可
{
	listnode_np<T>* temp = head;
	head = tail;
	tail = temp;
}