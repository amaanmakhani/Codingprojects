#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::vector <int> range(std::vector<int> numbers,int min,int max) {
	std::vector<int> result;
	for (auto elem : numbers) {
		if (elem < min || elem > max) {
			continue;
		}
		if (min <= elem && elem <= max) {
			result.push_back(elem);
		}
	}
	return result;
}

int main() {
	std::vector<int> numbers{1,2,3,4,5};
	int min, max{};
	std::cin >> min >> max;
	auto result = range(numbers, min, max);
	for (auto elem : result) {
		std::cout << elem << std::endl;
	}
	return 0;
}