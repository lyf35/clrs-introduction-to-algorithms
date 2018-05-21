#pragma once
#include<iostream>
#include<vector>
#include<memory>
enum status { double_link, circular_link };
template<typename T> struct listnode
{
	std::shared_ptr<listnode<T>> prev;
	std::shared_ptr<listnode<T>> next;
	T element;
	listnode<T>() = default;
	listnode<T>(std::shared_ptr<listnode<T>> before, std::shared_ptr<listnode<T>> after, T elem) : prev(before), next(after), element(elem) {};
};

template<typename T> class List
{
private:
	int size;
	std::shared_ptr<listnode<T>> head;
	std::shared_ptr<listnode<T>> tail;
	status link;
public:
	std::shared_ptr<listnode<T>> get_end()
	{
		return tail->prev;
	}
	std::shared_ptr<listnode<T>> get_beg()
	{
		return head->next;
	}
	std::shared_ptr<listnode<T>> get_tail()
	{
		return tail;
	}
	std::shared_ptr<listnode<T>> get_head()
	{
		return head;
	}
	List(status stat = double_link)
	{
		head = std::make_shared<listnode<T>>();
		if (link == circular_link) //如果是环形列表的话，那么用于指代表尾的tail指针就可以不用，初始化时直接让prev和next两指针都指向自己形成环路
		{
			head->prev = head;
			head->next = head;
		}
		if (link == double_link) //如果不是环形列表，head和tail指针都要使用
		{
			tail = std::make_shared<listnode<T>>();
			head->next = tail;
			tail->prev = head;
			size = 0;
		}
	};
	void copy(List c)
	{
		head(c.head);
		tail(c.tail);
		size(c.size);
		link(c.link);
	}
	void insert_begin(T elem)
	{
		std::shared_ptr<listnode<T>> temp = std::make_shared<listnode<T>>(listnode<T>(head, head->next, elem));
		head->next->prev = temp;
		head->next = temp;
		size++;
	}
	void insert_end(T elem)
	{
		std::shared_ptr<listnode<T>> temp = std::make_shared<listnode<T>>(listnode<T>(tail->prev, tail, elem));
		tail->prev->next = temp;
		tail->prev = temp;
		size++;
	}
	void insert_before(T elem, int pos)
	{
		std::shared_ptr<listnode<T>> temp(head);
		for (int i = 0;i < pos;i++)
		{
			temp = temp->next;
		}
		std::shared_ptr<listnode<T>> newnode = std::make_shared<listnode<T>>(listnode<T>(temp->next, tail->prev, elem));
		temp->next->prev = newnode;
		temp->next = newnode;
		size++;
	}
	listnode<T> operator[](int pos) const
	{
		std::shared_ptr<listnode<T>> temp(head);
		for (int i = 0;i <= pos;i++)
		{
			temp = temp->next;
		}
		return *temp;
	}
	T delete_begin()
	{
		auto temp = head->next;
		temp->next->prev = head;
		head->next = temp->next;
		size--;
		return temp->element;
	}
	T delete_end()
	{
		auto temp = tail->prev;
		tail->prev = temp->prev;
		temp->prev->next = tail;
		size--;
		return temp->element;
	}
	T delete_pos(int pos)
	{
		auto temp(head);
		for (int i = 0;i <= pos;i++)
		{
			temp = temp->next;
		}
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
		size--;
		return temp->element;
	}
	void delete_node(listnode<T> node)
	{
		node.prev = node.next->prev;
		node.next = node.prev->next;
		size--;
	}
	void delete_element(T elem)
	{
		int pos = search(elem);
		if (pos >= 0)
		{
			delete_node((*this)[pos]);
		}
	}
	void print(std::ostream& os) const
	{
		for (int i = 0;i < size;i++)
		{
			os << (*this)[i].element << '\t';
		}
		os << std::endl;
	}
	int get_size() const
	{
		return size;
	}
	int search(T elem) const
	{
		for (int i = 0;i < size;i++)
		{
			if ((*this)[i].element == elem)
			{
				return i;
			}
		}
		return -1;
	}
	void reverse() //将链表的元素顺序翻转
	{
		std::shared_ptr<listnode<T>> temp = head->next;
		for (int i = 0;i < size;i++)
		{
			swap(temp->prev, temp->next);
			temp = temp->prev;
		}
		swap(head->next, tail->prev);
	}
};