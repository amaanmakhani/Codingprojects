/* scrabble.cpp
   CSC 116 - Fall 2018
   Amaan Makhani
   V00883520
   This is a scrabble solver for given tiles
   October 28, 2018
*/

#include <iostream>
#include <set> 
#include "scrabble.hpp"
#include <vector>
#include <string>



ScrabbleSolver::ScrabbleSolver() {

}																//set constructor to default

void ScrabbleSolver::add_word( std::string const& word ){		//add word if not in dictionary
	auto result = oxford.find(word);							//get the iterator of the word 
	if (result == oxford.end()){								// if it doesnt exist add the word
	oxford.insert(word);
	}else {														//if it does throw an exception
	throw std::runtime_error("Duplicate");
	}
}

ScoreMap ScrabbleSolver::get_score_map(){						// return empty map to satisfy the compiler 
	std::map<std::string, int> empty;
	return empty;
}

ScoreMap ScrabbleSolver::find_words( TileSet const& tiles ){
	std::map<std::string,int> wordsandscore;					//create map that uses words as a key to scores
	for (auto elem : oxford) {									
		if (can_make_word(elem, tiles)) {						//check if words in dictionary can be made by tiles 
			wordsandscore.insert({ elem, compute_score(elem) });
		}														//if it can be made using tiles add the word and its score
	}
	return wordsandscore;										//return all possible words and values map
}

ScoreMap ScrabbleSolver::find_best_words( TileSet const& tiles ){
	auto result = find_words(tiles);							//use find word to create a map called result
	std::map<std::string,int> bestwordsandscore;				//make a map to contain the best words and their score 
	int max{ 0 };												//create variable max equal to zero 
	for (auto elem : result) {									//compare scores to max to find the best score
		if (max <= elem.second) {
			max = elem.second;
		}
	}
	for (auto elem : result) {									//inserta all the words with a max score into the best map
		if (elem.second == max) {
			bestwordsandscore.insert({ elem.first,elem.second });
		}
	}
	return bestwordsandscore;									//return map of best words and score
}

/* Static methods */

	
int ScrabbleSolver::compute_score( std::string const& word ){	//add up score of the word	
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

bool ScrabbleSolver::can_make_word( std::string const& word, TileSet const& tiles ){
	
std::vector<char> TILES;	
bool found{};

	for (auto elem : tiles) {									//put all tiles into a vector
		TILES.push_back(elem);
	}

	for (auto elem : word) {									// compare words to the tiles given
		found = false;											//set found to false 
		for (auto i = TILES.begin(); i <= TILES.end()-1; i++) {//use iterator loop to go throgh all tiles
			if (elem == *i) {									//if characters are the same erase it frim tile vector
				TILES.erase(i);
				found = true;									//change found to true and stop going through tiles continue to the next word
				break;
			}
		}
		if (!found) {											//if a letter is not found cant make word and return false 
			return false;
		}
	}

	return true;												//if all tiles found return true because you can make the word
}