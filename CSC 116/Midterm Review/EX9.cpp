#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

bool exists(std::vector<std::string> const& result, std::string const &word) {
	for (auto elem : result) {
		if (word == elem) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> repition(std::vector<std::string> const &input) {
	std::vector<std::string> result;
	for (unsigned int index = 0; index <= input.size() - 1; index++) {
		std::string word{ input.at(index) };
		for (unsigned int i = 0; i <= input.size() - 1; i++) {
			if (i == index) {
				continue;
			}
			if (word == input.at(i)) {
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
