#include "pch.h"
#include <iostream>
#include <string>
#include <vector>

void getSecond(std::vector<std::string> &words) {
	std::vector<std::string> copy;
	for (unsigned int i{}; i <= words.size() / 2-1; i++) {
		copy.push_back(words.at(2*i + 1));
	}
	words = copy;
}

int main() {
	std::vector<std::string> words{"one", "two", "three", "four"};
	getSecond(words);
	for (auto elem : words) {
		std::cout << elem << std::endl;
	}
	return 0;
}