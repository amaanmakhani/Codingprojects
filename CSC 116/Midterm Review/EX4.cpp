#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::string add(std::string word) {
	std::string result;
	char output{};
	for (unsigned int i=0; i<=word.size()-1;i++) {
		output=word.at(i)+1;
		result.push_back(output);
	}
	return result;
}

int main() {
	std::string input{};
	std::cin >> input;
	auto result = input(a);
	std::cout << result;
	return 0;
}