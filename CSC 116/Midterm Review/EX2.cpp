#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <cctype>

std::string reverse(std::string input) {	
	for (auto & elem : input) {
		if (std::islower(elem)) {
			elem=toupper(elem);
		}
		if (std::isupper(elem)) {
			elem=tolower(elem);
		}
	}
	return input;
}

int main() {
	std::string input{};
	std::cin >> input;
	input = reverse(input);
	std::cout <<input <<std::endl;
	return 0;
}