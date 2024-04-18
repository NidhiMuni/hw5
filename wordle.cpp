#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
  //Initialize a set of all letters
  std::queue<char> allLetters(std::deque<char>{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'});

  //Get a list of emptyIndeces
  vector<int> emptyIndeces;
  for (int i = 0; i < (int)in.length(); i++){
    if (in[i] == '-'){
      emptyIndeces.push_back(i);
    }
  }

  //Get a list of floating letters
  std::queue<char> floats;
  for(int i = 0; i < (int)floating.size(); i++){
    floats.push(floating[i]);
  }

  std::set<std::string> combos = wordleHelper(in, floats, allLetters, emptyIndeces);

  std::set<std::string> real_words;
  for(std::set<std::string>::iterator it = combos.begin(); it != combos.end(); ++it) {
    std::string possibleWord = *it;
    
    std::set<string>::iterator finder;
    finder=dict.find(possibleWord);

    if (finder != dict.end()){
      real_words.insert(*it);
    }
  }

  return real_words;

}

// Define any helper functions here
std::set<std::string> wordleHelper(
    std::string word,
    std::queue<char> floats,
    std::queue<char> allLetters,
    std::vector<int> emptyIndeces) 
{

  std::set<std::string> results;

  //Select the letter to place
  char letter;
  bool isFloating = false;

  //If there is nothing left to place or nowhere left to place it, done
  if ((floats.empty() && allLetters.empty()) || emptyIndeces.empty()){ 
    //cout << "empty id " <<emptyIndeces.empty() << endl;
    results.insert(word);
    return results;

  //If there is a floating letter, place it
  } else if (!floats.empty()){
    letter = floats.front();
    floats.pop();
    isFloating = true;
  } 

  for (int i = 0; i < (int)emptyIndeces.size(); i++){
    int emptyIndex = emptyIndeces[i];

    //in the new string, this index is no longer empty
    std::vector<int> emptyIndecesToSend = emptyIndeces;
    emptyIndecesToSend.erase(emptyIndecesToSend.begin() + i);

    std::string temp = word;
    //cout << "empty index is " << emptyIndex << " and letter is  "<< letter << endl;

    //if it is a float:
    if (isFloating == true){
      temp[emptyIndex] = letter;

      std::set<std::string> recursedWords = wordleHelper(temp, floats, allLetters, emptyIndecesToSend);
      results.insert(recursedWords.begin(), recursedWords.end());
    } else {
      for (char alphabet = 'a'; alphabet <= 'z'; alphabet++){
        temp[emptyIndex] = alphabet;

        std::set<std::string> recursedWords = wordleHelper(temp, floats, allLetters, emptyIndecesToSend);
        results.insert(recursedWords.begin(), recursedWords.end());
      }
    }
  }

  return results;

}
