#pragma once
#include<iostream>
#include<vector>
#include<memory>
template<typename T> class stack
{
private:
	int max_number;
	std::unique_ptr<T[]> element;
	int current;
public:
	stack(int n) :max_number(n), element(new T[n]())
	{
		current = -1;
	};
	void push(const T a)
	{
		if (check_full())
		{
			std::cout << "The stack is already full." << std::endl;
		}
		else
		{
			current++;
			element[current] = a;
		}
	}
	bool check_full()
	{
		if (current == max_number - 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	T pop()
	{
		if (check_empty())
		{
			std::cerr << "The stack is empty." << std::endl;
			return 0;
		}
		else
		{
			T temp = element[current];
			element[current] = 0;
			current--;
			return temp;
		}
	}
	bool check_empty()
	{
		if (current == -1)
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
		for (int i = 0;i <= current;i++)
		{
			os << element[i] << '\t';
		}
		os << std::endl;
	}
};
