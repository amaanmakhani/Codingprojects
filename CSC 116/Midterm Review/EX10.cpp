#include "pch.h"
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> caps(std::vector<std::string> words) {
	std::vector<std::string> result;
	std::string line;
	bool wasSpace=true;
	for (auto lines : words) {
		line.clear();
		wasSpace = true;
		for (auto word : lines) {
			if (word == ' ') {
				line.push_back(word);
				wasSpace = true;
			}
			if (word != ' '&& !wasSpace) {
				line.push_back(word);
				wasSpace = false;
			}
			if (word != ' '&& wasSpace) {
				word=toupper(word);
				line.push_back(word);
				wasSpace = false;
			}
		}
		result.push_back(line);
	}
	return result;
}


int main() {
	std::vector<std::string> words{"hello    there friend", "I love  C++"};
	auto result = caps(words);
	for (auto elem : result) {
		std::cout << elem << std::endl;
	}
	return 0;
}
