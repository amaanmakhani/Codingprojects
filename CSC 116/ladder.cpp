/*Amaan Makhani
V00883520
September 23, 2018
Determining if inputted words are really a word ladder
*/

/*
Requirements
Array can contains numbers or puncutaion
Not case sensitive
Same length words
Must be different than words before
Can only differ once from the previous word
*/

#include <iostream>															//includes all libraries 
#include <string>
#include <vector>
#include <string>
#include <cctype>

int compare(std::vector<std::string>userinput) {							//function to compare input words inputted by user

	for (unsigned int a = 0; a < userinput.size() - 1; a++) {						//runs a loop for every vector size to compare them
		
		std::string v1 = userinput[a];										//declares an element of a vector as word v1 so it can be transversed 
		std::string v2 = userinput[a + 1];									//declares an element of a vector as word v2 so it can be transversed
		int count=0;														// declares a varaible to store a count value

		for (unsigned int x = 0; x < v1.size(); x++) {								//takes v1 and changes it to lowercase for comparison
			v1[x] = tolower(v1[x]);											// changes each chracter to lower case
		}
		
		for (unsigned int y = 0; y < v2.size(); y++) {								//takes v2 and changes it to lowercase for comparison
			v2[y] = tolower(v2[y]);											// changes each chracter to lower case
		}

		if (v1.size() != v2.size()) {										//compares the size of v1 and v2 and returns 'a' the index of the failed transition
			return a;
		}

		for (unsigned int b = 0; b < v1.size()+1; b++) {							// takes the size of both v1 and v2 runs it until every charcater has been run 
			if (v1[b] != v2[b]) {											//if a character isn't the same add one to count
				count++;
			}
		}
		
		if (count > 1||count==0) {											//if count is more than one then return position of word which is 'a'
				return a;
		}
	}
	return 500;																//if no parts of the function fail return 1 which will indicate validity 
}

int main() {

	std::vector<std::string> userinput{};									//declares vector userinput
	std::string input{};													//declares string to store data into before inserting in vector								
	int index{};															// declares a variable to store invalid words for printing

	while (std::cin >> input) {												//takes input and places it in input 
		userinput.push_back(input);											//input is then put into user input and continues until user is done
	}
	
	index = compare(userinput);												//inputs userinput into a compare function
	
	if (index == 500) {														//outputs result of index then prints return based on function
		std::cout << "Word ladder is valid." << std::endl;					// if return of 500 prints valid
		
		for (auto words : userinput) {										
			
			for (unsigned int xy = 0; xy < words.size(); xy++) {					//turns all vector elements to lowercase
				words[xy] = tolower(words[xy]);											
			}

		std::cout << words << std::endl;									//Prints elements of valid string
		}
	}
	else  {																	//if not 500 prints the invalid transition using index
		std::cout << "Error: Invalid transition [" << userinput[index] << "]" << " -> [" << userinput[index + 1] << "]" << std::endl;
	}
	return 0;
}