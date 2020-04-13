#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::string slice(std::string word, int begin, int end) {
	std::string result;
	for (auto index = begin; i <= end; i++) {
		result.push_back(word.at(index));
	}
	return result;
}

int main() {
	std::string word{};
	int begin, end{};
	std::cin >> word >> begin >> end;
	auto result = slice(word, begin, end);
	std::cout << result << std::endl;
	return 0;
}