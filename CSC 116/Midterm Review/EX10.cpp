#include "pch.h"
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> caps(std::vector<std::string> words) {
	std::vector<std::string> result;
	std::string line;
	bool wasspace=true;
	for (auto lines : words) {
		line.clear();
		wasspace = true;
		for (auto word : lines) {
			
			if (word == ' ') {
				line.push_back(word);
				wasspace = true;
			}
			if (word != ' '&& !wasspace) {
				line.push_back(word);
				wasspace = false;
			}
			if (word != ' '&& wasspace) {
				word=toupper(word);
				line.push_back(word);
				wasspace = false;
			}
		}
		result.push_back(line);
	}
	return result;
}


int main() {
	std::vector<std::string> words{"hello    there buddy", "my name  is  amaan"};

	auto result = caps(words);

	for (auto elem : result) {
		std::cout << elem << std::endl;
	}

	return 0;
}
