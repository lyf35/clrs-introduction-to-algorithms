#pragma once
#include<iostream>
#include<vector>
#include"merge_sort.h"

void find_elements(std::vector<double> vec, double num)//exercise2.3-7，在vec中寻找两数相加等于num的组合。运行之前先做归并排序，让集合中的元素按照升序排列
{
	int i = 0;
	int cnt = 0;
	while (i < vec.size() - 1 && vec[i] + vec[i + 1] <= num)//注意&&判断条件的计算顺序，右边部分放在左边的时候会优先计算从而可能越界
	{
		double temp = num - vec[i];
		auto x = my_binary_search(vec.begin() + i, vec.end(), temp);
		if (x != vec.end())
		{
			if (*x == temp)
			{
				std::cout << "The elements are " << vec[i] << " and " << *x << std::endl;
				cnt++;
			}
		}
		i++;
	}
	if (cnt == 0)
	{
		std::cout << "No element found." << std::endl;
	}
}