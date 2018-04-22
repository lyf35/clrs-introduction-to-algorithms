#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<tuple>
#include<limits>

std::vector<double> max_heapify(std::vector<double>& heap, size_t i)
{
	size_t left = 2 * i + 1;
	size_t right = 2 * i + 2;
	size_t largest;
	if (left < heap.size() && heap[left] > heap[i])
	{
		largest = left;
	}
	else
	{
		largest = i;
	}
	if (right < heap.size() && heap[right] > heap[largest])
	{
		largest = right;
	}
	if (largest != i)
	{
		std::swap(heap[largest],heap[i]);
		max_heapify(heap, largest);
	}
	return heap;
}

std::vector<double> min_heapify(std::vector<double>& heap, size_t i)
{
	size_t left = 2 * i + 1;
	size_t right = 2 * i + 2;
	size_t smallest;
	if (left < heap.size() && heap[left] < heap[i])
	{
		smallest = left;
	}
	else
	{
		smallest = i;
	}
	if (right < heap.size() && heap[right] < heap[smallest])
	{
		smallest = right;
	}
	if (smallest != i)
	{
		std::swap(heap[smallest], heap[i]);
		min_heapify(heap, smallest);
	}
	return heap;
}

std::vector<double> max_heapify_modified(std::vector<double>& heap, size_t i)
{
	size_t largest = i;
	size_t temp;
	size_t left = 2 * largest + 1;
	size_t right = 2 * largest + 2;
	while (largest < heap.size() && left <= heap.size() && right <= heap.size())
	{
		left = 2 * largest + 1;
		right = 2 * largest + 2;
		temp = largest;
		if (left <= heap.size() && heap[left] > heap[largest])
		{
			largest = left;
		}
		if (right < heap.size() && heap[right] > heap[largest])
		{
			largest = right;
		}
		if (largest == temp)
		{
			break;
		}
		else
		{
			std::swap(heap[largest], heap[temp]);
		}
	}
	return heap;
}

std::vector<double> build_max_heap(std::vector<double>& a)
{
	for (auto i = a.size() / 2;i >= 1;i--)
	{
		max_heapify(a, i - 1);
	}
	return a;
}

std::vector<double> heap_sort(std::vector<double>& a)
{
	build_max_heap(a);
	std::vector<double> temp;
	for (int i = a.size();i > 1;i--)
	{
		std::swap(a[0], a[i - 1]);
		temp.insert(temp.begin(), a[i - 1]);
		a.pop_back();
		max_heapify(a, 0);
	}
	a.insert(a.end(), temp.begin(), temp.end());
	return a;
}

double heap_maximum(const std::vector<double> a)
{
	return a[0];
}

double heap_minimun(const std::vector<double> a)
{
	return a[0];
}

double heap_extract_max(std::vector<double>& a)
{
	if (a.size() < 1)
	{
		std::cout << "Heap underflow." << std::endl;
		return 0;
	}
	double max = a[0];
	a[0] = a[a.size() - 1];
	a.pop_back();
	max_heapify(a, 0);
	return max;
}

double heap_extract_min(std::vector<double>& a)
{
	if (a.size() < 1)
	{
		std::cout << "Heap underflow." << std::endl;
		return 0;
	}
	double min = a[0];
	a[0] = a[a.size() - 1];
	a.pop_back();
	min_heapify(a, 0);
	return min;
}

std::vector<double> heap_increase_key(std::vector<double>& a, size_t i, double key)
{
	if (key < a[i])
	{
		std::cout << "new key is smaller than current key." << std::endl;
		return a;
	}
	a[i] = key;
	while (i >= 0 && a[(i - 1) / 2] < a[i])
	{
		std::swap(a[(i - 1) / 2], a[i]);
		i = (i - 1) / 2;
	}
	return a;
}

std::vector<double> heap_decrease_key(std::vector<double>& a, size_t i, double key)
{
	if (key > a[i])
	{
		std::cout << "new key is bigger than current key." << std::endl;
		return a;
	}
	a[i] = key;
	while (i >= 0 && a[(i - 1) / 2] > a[i])
	{
		std::swap(a[(i - 1) / 2], a[i]);
		i = (i - 1) / 2;
	}
	return a;
}

std::vector<double> max_heap_insert(std::vector<double>& a, double key)
{
	a.push_back(std::numeric_limits<double>::min());
	return heap_increase_key(a, a.size() - 1, key);
}

std::vector<double> min_heap_insert(std::vector<double>& a, double key)
{
	a.push_back(std::numeric_limits<double>::max());
	return heap_decrease_key(a, a.size() - 1, key);
}

std::vector<double> heap_delete(std::vector<double>& a, double key)
{
	a[key] = std::numeric_limits<double>::min();
	if (key != a.size() - 1)
	{
		std::swap(a[key], a[a.size() - 1]);
		max_heapify(a, key);
	}
	a.pop_back();
	return a;
}


//