/*
Description: Determining if inputted words are really a word ladder
Requirements and details:
	- Array can contains numbers or punctuation
	- Not case sensitive
	- Words are of the same length
	- Must be different than words before
	- Can only differ by one letter from the previous word
*/

#include <iostream>												
#include <string>                                                        
#include <vector>                                                        
#include <string>                                                        
#include <cctype>                                                        

int compare(std::vector<std::string>userinput) {																				
	for (unsigned int a = 0; a < userinput.size() - 1; a++) {																	
		std::string v1 = userinput[a];							
		std::string v2 = userinput[a + 1];						
		int count=0;																											
		for (unsigned int x = 0; x < v1.size(); x++) {			
			v1[x] = tolower(v1[x]);								
		}                                                                															
		for (unsigned int y = 0; y < v2.size(); y++) {			
			v2[y] = tolower(v2[y]);								
		}                                                                															
		if (v1.size() != v2.size()) {							
			return a;                                                    
		}                                                                															
		for (unsigned int b = 0; b < v1.size()+1; b++) {		
			if (v1[b] != v2[b]) {								
				count++;                                                 
			}                                                            
		}                                                                																
		if (count > 1||count==0) {								
			return a;                                                
		}                                                                
	}                                                                    
	return -1;													
}                                                                        

int main() {                                                             																
	std::vector<std::string> userinput{};						
	std::string input{};										
	int index{};																												
	while (std::cin >> input) {									
		userinput.push_back(input);								
	}                                                                    															
	index = compare(userinput);																								
	if (index == -1) {											
		std::cout << "Word ladder is valid." << std::endl;																	
		for (auto words : userinput) {																							
			for (unsigned int xy = 0; xy < words.size(); xy++) {
				words[xy] = tolower(words[xy]);					
			}   																
		std::cout << words << std::endl;						
		}                                                                
	} else  {														
		std::cout << "Error: Invalid transition [" << userinput[index] << "]" << " -> [" << userinput[index + 1] << "]" << std::endl;
	}
	return 0;
}