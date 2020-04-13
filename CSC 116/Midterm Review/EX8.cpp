#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

bool repition(std::vector<std::string> const &input) {
	for (unsigned int index = 0; index <= input.size() - 1; index++) {
		std::string word{input.at(index)};
		for (unsigned int i = 0; i <= input.size() - 1; i++) {
			if (i == index) {
				continue;
			}
			if (word == input.at(i)) {
				return true;
			}
		}
	}
	return false;
}

int main() {
	std::vector<std::string>input{"hello","there", "friend","hello"};
	auto result = repition(input);
	std::cout << result << std::endl;
	return 0;
}