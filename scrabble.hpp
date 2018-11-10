/* scrabble.hpp
   CSC 116 - Fall 2018
   
   Definitions for the ScrabbleSolver class.

   You may modify this file, but ONLY in the places indicated. In particular, you must
   place the implementation of each function in the separate scrabble.cpp file.

   B. Bird - 09/26/2018
*/

#include <string>
#include <set>
#include <map>

/* A "ScoreMap" is a mapping of words to scores */
using ScoreMap = std::map< std::string, int >;

/* A "TileSet" represents a collection of tiles. Notice
   that std::multiset is used to allow a single tile (e.g. "z")
   to appear more than once in the set. 

   Recall that the .count() method of a multiset can be used to
   query the multiplicity (number of occurrences) of a particular
   element.
*/
using TileSet = std::multiset< char >;


class ScrabbleSolver{
public:
    /* Do not modify any of the declarations and code in this section in any way. */
    /* (Put your code for these functions in scrabble.cpp, not directly in this file) */

    /* Constructor */
    ScrabbleSolver();

    /* add_word(word)
       Add the provided word to the internal dictionary of this solver.
       If the word is already present in the dictionary, throw an
       exception of type std::runtime_error.
    */
    void add_word( std::string const& word );

    /* get_score_map()
       Return a map containing all words in the dictionary, mapped to their
       scores (as computed by compute_score).
    */
    ScoreMap get_score_map();

    /* find_words(tiles)
       Given a set of tiles, return a map containing all possible
       words that can be created from the provided tiles, with each word
       word is mapped to its score.

       Remember that it is not necessary for a word to use all of the provided
       tiles (some tiles may be left unused).

       Hint: Try using get_score_map first
    */
    ScoreMap find_words( TileSet const& tiles );

    /* find_best_words(tiles)
       Given a set of tiles, return a map containing only the highest-scoring
       words that can be created from the provided tiles, with each word
       mapped to its score.

       Remember that it is not necessary for a word to use all of the provided
       tiles (some tiles may be left unused).

       Hint: Use find_words first.
    */
    ScoreMap find_best_words( TileSet const& tiles );


    /* Static methods */
    /* These methods are not associated with a specific solver instance, and
      do not use the internal dictionary */

    /* compute_score(word)
       Compute and return the score of the provided word.
    */
    static int compute_score( std::string const& word );

    /* can_make_word(word, tiles)
       Return true if the provided word can be constructed using the provided
       tiles and false otherwise. 
    */
    static bool can_make_word( std::string const& word, TileSet const& tiles );

    /* string_to_tileset(s)
       Creates a TileSet object out of the characters in the provided string
       (as a convenience). Notice that this is an "inline" method defined
       directly in the header.
    */
    static inline TileSet string_to_tileset( std::string const& s ){
        TileSet T;
        for(auto c: s)
            T.insert(c);
        return T;
    }

   /* Do not modify any of the code above this line */

private:
    /* Your code here */
    /* (Add any private data or methods you need)*/
    /* (You may also add new #include directives at the top of the file) */
	std::set<std::string> oxford;
};