#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

bool repition(std::vector<std::string> const &input) {
	
	for (unsigned int i = 0; i <= input.size() - 1; i++) {

		std::string word{ input.at(i)};
		for (unsigned int k = 0; k <= input.size() - 1; k++) {
			if (k == i) {
				continue;
			}
			if (word == input.at(k)) {
				return true;
			}
		}
	}
	return false;
}



int main() {
	std::vector<std::string>input{"hello","there", "bud","hello"};
	
	auto result = repition(input);
	
	std::cout << result << std::endl;
	
	return 0;
}
