#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::vector <int> range(std::vector<int> numbers,int a,int b) {
	std::vector<int> result;
	for (auto elem : numbers) {
		if (elem<a || elem>b) {
			continue;
		}
		if (a <= elem || elem <= b) {
			result.push_back(elem);
		}
	}
	return result;
}

int main() {
	std::vector<int> numbers{1,2,3,4,5};
	int a, b{};
	std::cin >> a >> b;
	auto result = range(numbers, a, b);
	for (auto elem : result) {
		std::cout << elem << std::endl;
	}
	return 0;
}