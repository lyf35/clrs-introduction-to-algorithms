#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<limits>

template<typename T> std::vector<T> max_heapify(std::vector<T>& heap, size_t i) //递归算法
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

template<typename T> std::vector<T> max_heapify_modified(std::vector<T>& heap, size_t i) //迭代算法
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

template<typename T> std::vector<T> build_max_heap(std::vector<T>& a) //建一个最大堆
{
	for (auto i = a.size() / 2;i >= 1;i--)
	{
		max_heapify(a, i - 1);
	}
	return a;
}

template<typename T> std::vector<T> heap_sort(std::vector<T>& a) //堆排序
{
	build_max_heap(a);
	std::vector<T> temp;
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

template<typename T> T heap_maximum(const std::vector<T> a)
{
	return a[0];
}

template<typename T> T heap_extract_max(std::vector<T>& a)
{
	if (a.size() < 1)
	{
		std::cout << "Heap underflow." << std::endl;
		return 0;
	}
	T max = a[0];
	a[0] = a[a.size() - 1];
	a.pop_back();
	max_heapify(a, 0);
	return max;
}

template<typename T> std::vector<T> heap_increase_key(std::vector<T>& a, size_t i, T key)
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

template<typename T> std::vector<T> max_heap_insert(std::vector<T>& a, T key)
{
	a.push_back(std::numeric_limits<T>::min());
	return heap_increase_key(a, a.size() - 1, key);
}

template<typename T> std::vector<T> heap_delete(std::vector<T>& a, T key)
{
	a[key] = std::numeric_limits<T>::min();
	if (key != a.size() - 1)
	{
		std::swap(a[key], a[a.size() - 1]);
		max_heapify(a, key);
	}
	a.pop_back();
	return a;
}