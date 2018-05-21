#pragma once
#include<iostream>
#include<vector>
#include<memory>

template<typename T> class queue
{
private:
	int max_number;
	std::unique_ptr<T[]> element;
	int head;
	int tail;
	bool full;
public:
	queue(int n) :max_number(n), element(new T[n]())
	{
		head = tail = 0;
	};
	void push(const T a)
	{
		if (full)
		{
			std::cout << "The queue is already full." << std::endl;
		}
		else
		{
			element[tail] = a;
			tail++;
			if (tail >= max_number)
			{
				tail = tail%max_number;
			}
			if (head == tail)
			{
				full = 1;
			}
		}
	}
	T pop()
	{
		if (check_empty())
		{
			std::cerr << "The queue is empty." << std::endl;
			return 0;
		}
		else
		{
			T temp = element[head];
			element[head] = 0;
			head--;
			if (head < 0)
			{
				head = head + max_number;
			}
			if (full == 1)
			{
				full = 0;
			}
			return temp;
		}
	}
	bool check_empty()
	{
		if (head == tail&&full == 0)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	void output_elements(std::ostream& os)
	{
		for (int i = 0;i < max_number;i++)
		{
			os << element[i] << '\t';
		}
		os << std::endl;
	}
};
