//Description: Creates a pipeline of action given a string. The program was designed using inheritance and smart pointers.

#include <string>
#include <cctype>
#include "pipeline.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

/* You may add as many (or as few) classes or other functions to this file as you want. 
   However, no global variables are permitted (except for const variables if needed). */

//conditions
class all:public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override{
		return true;
	}
};

class none :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		return false;
	}
};

class first_chracter :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		if (idx == 0){
			return true;
		}
		return false;
	}
};

class last_character :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		auto last = input.size() - 1;
		if ((int)last ==(int)idx) {
			return true;
		}
		return false;
	}
};

class all_lowercase :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		auto a = input.at(idx);
		if (std::islower(a)) {
			return true;
		}
		return false;
	}
};

class all_uppercase :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		auto a = input.at(idx);
		if (std::isupper(a)) {
			return true;
		}
		return false;
	}
};

class index :public Condition {
public:
	index(std::string a) {
		auto num = stoi(a);
		mindex=num;
	}
	bool check_condition(const std::string& input, unsigned int idx) override {
		if ((int)idx == mindex) {
			return true;
		}
		return false;
	}
private:
	int mindex{};
};

class letter :public Condition {
public:
	letter(std::string a) {
		mletter=a;
	}
	bool check_condition(const std::string& input, unsigned int idx) override {
		auto user = input.at(idx);
		for (auto elem : mletter) {
			if (user == elem) {
				return true;
			}
			return false;
		}
		return false;
	}
private:
	std::string mletter{};
};

class start_of_word :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		if (idx == 0) {
			return true;
		}
		if (input.at(idx - 1) == ' '){
			return true;
		}
		return false;
	}
};

class end_of_word :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		auto max = input.size() - 1;
		if (idx == max){
			if (input.at(idx) != ' ') {
				return true;
			} else { 
				return false; 
			}
		}
		if (input.at(idx + 1) == ' '&&input.at(idx)!=' ') {
			return true;
		}
		return false;
	}
};

class first_word :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		auto max = words(input);
		if ((int)idx < max) {
			return true;
		}
		return false;
	}
	int words(std::string const& input) {
		std::string copy{};
		unsigned int max; 
		for (auto elem : input) {
			if (elem != ' ') {
				copy.push_back(elem);
			}
			if (elem == ' ') {
				break;
			}
		}
		max=copy.size();
		return (int)max;
	}
};

class last_word :public Condition {
public:
	bool check_condition(const std::string& input, unsigned int idx) override {
		for (unsigned int i = idx; i <= input.size() - 1; i++) {
			if (input.at(i) == ' ') {
				return false;
			}
		}
		return true;
	}
};

//make condition
ConditionPtr make_condition(const std::string& name, const std::string& argument) {
	if (name == "all") {
		ConditionPtr ptr = std::make_shared<all>();
		return ptr;
	}
	if (name == "none") {
		ConditionPtr ptr = std::make_shared<none>();
		return ptr;
	}
	if (name == "first_character") {
		ConditionPtr ptr = std::make_shared<first_chracter>();
		return ptr;
	}
	if (name == "last_character") {
		ConditionPtr ptr = std::make_shared<last_character>();
		return ptr;
	}
	if (name == "first_word") {
		ConditionPtr ptr = std::make_shared<first_word>();
		return ptr;
	}
	if (name == "last_word") {
		ConditionPtr ptr = std::make_shared<last_word>();
		return ptr;
	}
	if (name == "start_of_word") {
		ConditionPtr ptr = std::make_shared<start_of_word>();
		return ptr;
	}
	if (name == "end_of_word") {
		ConditionPtr ptr = std::make_shared<end_of_word>();
		return ptr;
	}
	if (name == "all_lowercase") {
		ConditionPtr ptr = std::make_shared<all_lowercase>();
		return ptr;
	}
	if (name == "all_uppercase") {
		ConditionPtr ptr = std::make_shared<all_uppercase>();
		return ptr;
	}
	if (name == "index") {
		ConditionPtr ptr = std::make_shared<index>(argument);
		return ptr;
	}
	if (name == "letter") {
		ConditionPtr ptr = std::make_shared<letter>(argument);
		return ptr;
	} else {
		throw std::invalid_argument("invalid");
	}
}

class invert_case:public Action {
public:
	invert_case(ConditionPtr ptr) {
		usercondition = ptr;
	}
	std::string transform(const std::string& input) {
		std::string result{};
		if (input.empty()) {
			return input;
		}
		for (unsigned int i = 0; i <= input.size() - 1; i++) {
			if (usercondition->check_condition(input, i)) {
				if (std::isupper(input.at(i))) {
					result.push_back(tolower(input.at(i)));
				}
				if (std::islower(input.at(i))) {
					result.push_back(toupper(input.at(i)));
				}
			} else {
				result.push_back(input.at(i));
			}

		}
		return result;
	}
private:
	ConditionPtr usercondition;
};
		
	
class Remove :public Action {
public:
	Remove(ConditionPtr ptr) {
		usercondition = ptr;
	}
	std::string transform(const std::string& input) {
		std::string result{};
		if (input.empty()) {
			return input;
		}
		for (unsigned int i = 0; i <= input.size()- 1; i++) {
		if(!usercondition->check_condition(input,i))
			result.push_back(input.at(i));
		}
		return result;
	}
private:
ConditionPtr usercondition;
};
	
class lowercase :public Action {
public:
	lowercase(ConditionPtr ptr) {
		usercondition = ptr;
	}
	std::string transform(const std::string& input) {
		std::string result{};
		if (input.empty()) {
			return input;
		}
		for (unsigned int i = 0; i <= input.size()- 1; i++) {
			if (usercondition->check_condition(input, i)) {
				result.push_back(tolower(input.at(i)));
			} else {
				result.push_back(input.at(i));
			}
		}
		return result;
	}
private:
ConditionPtr usercondition;
};
	
class uppercase :public Action {
public:
	uppercase(ConditionPtr ptr) {
		usercondition = ptr;
	}
	std::string transform(const std::string& input) {
		std::string result{};
		if (input.empty()) {
			return input;
		}
		for (unsigned int i = 0; i <= input.size() - 1; i++) {
			if (usercondition->check_condition(input, i)) {
				 result.push_back(toupper(input.at(i)));
			} else {
				result.push_back(input.at(i));
			}
		}
		return result;
	}
private:
ConditionPtr usercondition;
};

ActionPtr make_action(const std::string& name, ConditionPtr condition) {
	if (name == "remove") {
		ActionPtr ptr = std::make_shared<Remove>(condition);
		return ptr;
	}
	if (name == "uppercase") {
		ActionPtr ptr = std::make_shared<uppercase>(condition);
		return ptr;
	}
	if (name == "lowercase") {
		ActionPtr ptr = std::make_shared<lowercase>(condition);
		return ptr;
	}
	if (name == "invert_case") {
		ActionPtr ptr = std::make_shared<invert_case>(condition);
		return ptr;
	} else {
		throw std::invalid_argument("invalid");
	}
}