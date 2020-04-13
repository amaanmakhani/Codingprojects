// Description: Scrabble Solver will find the best word and score for a given set of tiles. A dictionary of valid words is given.

#include <iostream>
#include <set> 
#include "scrabble.hpp"
#include <vector>
#include <string>

void ScrabbleSolver::addWord( std::string const& word ){		
	auto result = oxford.find(word);							
	if (result == oxford.end()){								
		oxford.insert(word);                                        
	} else {														
		throw std::runtime_error("Duplicate");
	}
}

ScoreMap ScrabbleSolver::getScoreMap(){
	std::map<std::string, int> empty;
	return empty;
}

ScoreMap ScrabbleSolver::findWords( TileSet const& tiles ){
	std::map<std::string,int> wordsandscore;					
	for (auto elem : oxford) {									
		if (canMakeWord(elem, tiles)) {						
			wordsandscore.insert({ elem, computeScore(elem) });
		}														
	}
	return wordsandscore;
}

ScoreMap ScrabbleSolver::findBestWords( TileSet const& tiles ){
	auto result = find_words(tiles);
	std::map<std::string,int> bestwordsandscore;
	int max{ 0 };
	for (auto elem : result) {
		if (max <= elem.second) {
			max = elem.second;
		}
	}
	for (auto elem : result) {
		if (elem.second == max) {
			bestwordsandscore.insert({ elem.first,elem.second });
		}
	}
	return bestwordsandscore;
}

/* Static methods */
int ScrabbleSolver::computeScore( std::string const& word ){	
	int sum{0};													
	for (auto character : word) {
		if (character == 'a' || character == 'e' || character == 'i' || character == 'l' || character == 'n' || character == 'o' || character == 'r' || character == 's' || character == 't' || character == 'u') {
			sum += 1;
		}
		if (character == 'd' || character == 'g' ) {
			sum += 2;
		}
		if (character == 'b' || character == 'c' || character == 'm' || character == 'p' ) {
			sum += 3;
		}
		if (character == 'f' || character == 'h' || character == 'v' || character == 'w'|| character =='y') {
			sum += 4;
		}
		if (character == 'k') {
			sum += 5;
		}
		if (character == 'j' || character == 'x') {
			sum += 8;
		}
		if (character == 'q' || character == 'z') {
			sum += 10;
		}
	}
	return sum;
}

bool ScrabbleSolver::canMakeWord( std::string const& word, TileSet const& tiles ){
	std::vector<char> TILES;	
	bool found{};
	for (auto elem : tiles) {									
		TILES.push_back(elem);                                 
	}                                                          															   
	for (auto elem : word) {									
		found = false;											
		for (auto i = TILES.begin(); i <= TILES.end()-1; i++) {
			if (elem == *i) {									
				TILES.erase(i);                                 
				found = true;									
				break;                                          
			}                                                   
		}                                                       
		if (!found) {											
			return false;                                       
		}                                                       
	}                                                           																
	return true;												
}