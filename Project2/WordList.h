//contains classes for WordCount.cpp
//Project #2 WordList.h
//Madison Dockery
//01/21/2020

#ifndef WORD_LIST_H
#define WORD_LIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> //allows for sort
#include <cctype> //allows for isalnum

using std::cout;   using std::endl; using std::ifstream;
using std::string; using std::getline; 
using std::isalnum; //checks if character is alphanumeric 
//isalnum is used to check for white spaces when comparing
using std::sort; //sort elements in range

class WordOccurrence {

 public:

  WordOccurrence(const string& word="", int num=0);
  bool matchWord(const string &); // returns true if word matches stored
  void increment(); // increments number of occurrences
  string getWord() const; 
  int getNum() const;

  //needed for sort when printing
  bool operator<(const WordOccurrence& rhs) const { return num < rhs.num; }

 private:
  string word;
  int num;

};

WordOccurrence::WordOccurrence(const string& newWord, int newNum){
  word = newWord;
  num = newNum;
}

bool WordOccurrence::matchWord(const string& otherWord){
  return otherWord == word;
}

void WordOccurrence::increment(){
  ++num;
}

string WordOccurrence::getWord()const{
  return word;
}

int WordOccurrence::getNum()const{
  return num;
}

class WordList{

 public:

  // add copy constructor, destructor, overloaded assignment
  WordList();
  WordList(const WordList&);
  ~WordList();
  WordList& operator=(const WordList&);

  // implement comparison as friend
  friend bool equal(const WordList&, const WordList&);
  void addWord(const string &);
  void print();

 private:

  WordOccurrence *wordArray; // a dynamically allocated array of WordOccurrences
  // may or may not be sorted
  int size;

};

WordList::WordList(){
  wordArray = nullptr;
  size = 0;
}

WordList::WordList(const WordList& wordList){

  size = wordList.size;
  wordArray = new WordOccurrence[size];
  
  for(int i = 0; i < size; ++i){
    wordArray[i] = wordList.wordArray[i];
  }
}

WordList::~WordList(){
  delete[] wordArray;
}

WordList& WordList::operator=(const WordList& rhs){
  
  if(this == &rhs){
    return *this;
  }
  
  delete[] wordArray; //deleting old to make room for new
  
  size = rhs.size;
  wordArray = new WordOccurrence[size];
  
  for(int i = 0; i < size; ++i){
    wordArray[i] = rhs.wordArray[i];
  }
  
  return *this;
}

bool equal(const WordList& lhs, const WordList& rhs){
  
  int size = lhs.size;

  //checking to see if they are same size words
  if(size != rhs.size){
    return false;
  }
  
  //checking each piece of word
  for(int i = 0; i < size; ++i){
    if(lhs.wordArray[i].getWord() != rhs.wordArray[i].getWord()){
       return false;
    }
  }

  return true;
}

void WordList::addWord(const string& word){
  
  //need to check if adding to amount a word appears
  //or if new count needs to be started
  for(int i = 0; i < size; ++i){
    if(wordArray[i].matchWord(word)){
      wordArray[i].increment();
      return;
    }
  }
  
  //need to copy old array to temp
  WordOccurrence *temp = new WordOccurrence[size];
  for(int i = 0; i < size; ++i){
    temp[i] = wordArray[i];
  }

  //deleting old array and making a bigger new array
  ++size;
  delete[] wordArray;
  wordArray = new WordOccurrence[size];

  //putting old information back into array
  for(int i = 0; i < size - 1; ++i){
    wordArray[i] = temp[i];
  }

  //add the new word to the array
  WordOccurrence addWord = word;
  addWord.increment();
  wordArray[size - 1] = addWord;
}

void WordList::print(){
  
  sort(wordArray, wordArray + size); //sorts into ascending order
  
  for(int i = 0; i < size; ++i){
    cout << wordArray[i].getWord() << endl 
	 << "count: " << wordArray[i].getNum() << endl;
  }

}

#endif //WORD_LIST_H
