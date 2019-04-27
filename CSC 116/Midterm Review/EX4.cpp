#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::string add(std::string word) {
	std::string result;
	char a{};

	for (unsigned int i=0; i<=word.size()-1;i++) {
		a=word.at(i)+1;
		result.push_back(a);
	}
	return result;
}

int main() {
	std::string a{};
	std::cin >> a;
	auto result = add(a);
	std::cout << result;
	return 0;
}