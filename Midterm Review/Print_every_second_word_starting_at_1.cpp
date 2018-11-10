#include "pch.h"
#include <iostream>
#include <string>
#include <vector>

void second(std::vector<std::string> &words) {
	auto number = words.size();
	std::vector<std::string> copy;

	for (unsigned int i{}; i <= words.size() / 2-1; i++) {
		copy.push_back(words.at(2*i + 1));
	}
	
	words = copy;
}


int main() {
	std::vector<std::string> words{"hello", "my", "name", "is","amaan", "what", "is","yours"};

	second(words);

	for (auto elem : words) {
		std::cout << elem << std::endl;
	}

	return 0;
}