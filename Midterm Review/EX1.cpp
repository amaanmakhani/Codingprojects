#include "pch.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int maximum(std::vector <int> const &numbers) {
	int maximum{};
	for (auto elem : numbers) {
		if (elem >= maximum) {
			maximum = elem;
		}
	}
	return maximum;
}

int minimum(std::vector <int> const &numbers) {
	int min=numbers.at(0);
	for (auto elem : numbers) {
		if (min >= elem) {
			min = elem;
		}
	}
	return min;
}


double median(std::vector <int> &numbers) {
	std::sort(numbers.begin(), numbers.end());
	if (numbers.size() % 2 == 0) {
		float left = numbers.at(numbers.size() / 2 - 1);
		float right = numbers.at(numbers.size() / 2);
		return ((left + right) / 2);
	}
	else {
		return numbers.at(numbers.size() / 2);
	}
}


int average(std::vector <int> const &numbers) {
	int sum{};

	for (auto elem : numbers) {
		sum += elem;
	}

	auto average = sum / numbers.size();
	return average;

}

int main()
{
	std::vector <int> numbers{ 8, 5, 7, 10, 15, 21 };
	auto min = minimum(numbers);
	auto max = maximum(numbers);
	auto med = median(numbers);
	auto avg = average(numbers);

	std::cout << min << std::endl;
	std::cout << max << std::endl;
	std::cout << med << std::endl;
	std::cout << avg << std::endl;
	return 0;
}
