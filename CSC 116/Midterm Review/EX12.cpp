#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::vector<int> positive(std::vector<int> input, int min) {
	std::vector<int> result;
	for (auto elem : input) {
		if (elem > min) {
			result.push_back(elem);
		}
	}
	return result;
}

int main() {
	std::vector<int>input{};
	int number{};
	int min{};
	std::cin >> min;
	while (std::cin >> number) {
		input.push_back(number);
	}
	auto result = positive(input,min);
	for (auto elem : result) {
		std::cout << elem<<std::endl;
	}
	return 0;
}