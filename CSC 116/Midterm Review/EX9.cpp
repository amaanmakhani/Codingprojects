#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

bool exists(std::vector<std::string> const& result, std::string const &w) {
	for (auto elem : result) {
		if (w == elem) {
			return true;
		}
	}
	return false;
}


std::vector<std::string> repition(std::vector<std::string> const &input) {
	std::vector<std::string> result;
	for (unsigned int i = 0; i <= input.size() - 1; i++) {

		std::string word{ input.at(i) };
		for (unsigned int k = 0; k <= input.size() - 1; k++) {
			if (k == i) {
				continue;
			}
			if (word == input.at(k)) {
				if (!exists(result, word)) {
					result.push_back(word);
				}
			}
		}
	}
	return result;
}



int main() {
	std::vector<std::string>input{"hello","there", "bud","hello"};
	
	auto result = repition(input);
	
	for (auto elem : result) {
		std::cout << elem << std::endl;
	}
	return 0;
}
