#include "pch.h"
#include <iostream>
#include <vector>
#include <string>

std::string add(std::string a, int b, int c) {
	std::string copy = a;
	std::string result;
	
	for (auto i = b; i <= c; i++) {
		result.push_back(copy.at(i));
	}
	return result;
}

int main() {
	std::string a{};
	int b, c{};

	std::cin >> a >> b >> c;

	auto result = add(a, b, c);
	std::cout << result << std::endl;
	return 0;
}