#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::vector<int> positive(std::vector<int> input, int a) {
	std::vector<int> result;
	
	for (auto elem : input) {
		if (elem > a) {
			result.push_back(elem);
		}
	}
	return result;
}

int main() {
	std::vector<int>input{};
	int number{};
	int a{};

	std::cin >> a;

	while (std::cin >> number) {
		input.push_back(number);
	}
	auto result = positive(input,a);
	
	for (auto elem : result) {
	std::cout << elem<<std::endl;
	}
	
	return 0;
}