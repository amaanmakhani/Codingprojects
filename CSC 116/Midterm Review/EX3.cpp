#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

bool compare(std::string first, std::string second) {
	if (first.size()!= second.size()) {
		return false;
	}
	for (auto & elem : first) {
		elem = tolower(elem);
	}
	for (auto & elem : second) {
		elem = tolower(elem);
	}
	for (unsigned int j = 0; j <= first.size() - 1; j++) {
		if (first.at(j) != second.at(j)) {
			return false;
		}
	}
	return true;
}

int main() {
	std::string first{};
	std::string second{};
	std::cin >> first >> second;
	auto result = compare(first,second);
	std::cout << result <<std::endl;
	return 0;
}