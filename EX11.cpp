#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::vector<int> positive(std::vector<int> input) {
	std::vector<int> result;
	
	for (auto elem : input) {
		if (elem > 0) {
			result.push_back(elem);
		}
	}
	return result;
}

int main() {
	std::vector<int>input{};
	int number{};

	while (std::cin >> number) {
		input.push_back(number);
	}
	auto result = positive(input);
	
	for (auto elem : result) {
	std::cout << elem<<std::endl;
	}
	
	return 0;
}