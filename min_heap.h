#pragma once
#include<iostream>
#include<vector>
#include<algorithm>
#include<cmath>
#include<limits>

template<typename T> std::vector<T> min_heapify(std::vector<T>& heap, size_t i)
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

template<typename T> std::vector<T> build_min_heap(std::vector<T>& a) 
{
	for (auto i = a.size() / 2;i >= 1;i--)
	{
		min_heapify(a, i - 1);
	}
	return a;
}

template<typename T> std::vector<T> heap_sort(std::vector<T> a) //∂—≈≈–Ú
{
	build_min_heap(a);
	std::vector<T> temp;
	for (int i = a.size();i > 1;i--)
	{
		std::swap(a[0], a[i - 1]);
		temp.insert(temp.begin(), a[i - 1]);
		a.pop_back();
		min_heapify(a, 0);
	}
	a.insert(a.end(), temp.begin(), temp.end());
	return a;
}

template<typename T> T heap_minimun(const std::vector<T> a)
{
	return a[0];
}

template<typename T> T heap_extract_min(std::vector<T>& a)
{
	if (a.size() < 1)
	{
		std::cout << "Heap underflow." << std::endl;
		return 0;
	}
	T min = a[0];
	a[0] = a[a.size() - 1];
	a.pop_back();
	min_heapify(a, 0);
	return min;
}

template<typename T> std::vector<T> heap_decrease_key(std::vector<T>& a, int i, T key)
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

template<typename T> std::vector<T> min_heap_insert(std::vector<T>& a, T key)
{
	a.push_back(std::numeric_limits<T>::max());
	return heap_decrease_key(a, a.size() - 1, key);
}

template<typename T> std::vector<T> heap_delete(std::vector<T>& a, T key)
{
	a[key] = std::numeric_limits<T>::max();
	if (key != a.size() - 1)
	{
		std::swap(a[key], a[a.size() - 1]);
		min_heapify(a, key);
	}
	a.pop_back();
	return a;
}

