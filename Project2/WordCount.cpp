//Program that reads texts and outputs the amount each word is repeated
//Project #2 Word Count
//Madison Dockery
//01/21/2020

#include "WordList.h"


int main(int argc, char *argv[]) {
  
  //number of arguments needs to be 2
  if(argc != 2) {
    cout << "number of arguments incorrect" << endl;
    return 1;
  }
  
  //for outputting file name
  string fileName = argv[1];
  
  //opening file
  ifstream incomingfile;
  incomingfile.open (fileName);
  if (!incomingfile.is_open()) {
    cout << "error opening file1" << endl;
    return 1;
  }
  
  WordList newList;
  string newWord;
  
  while(!incomingfile.eof()){
    incomingfile >> newWord;
    
    //check if is alphanumeric character or not
    if(!isalnum(newWord.back())){
      newWord.pop_back();
    }    

    if(!isalnum(newWord.front())){
      newWord.erase(0);
    }

    //now it needs to be checked to see if any word is left
    if(newWord != ""){
      newList.addWord(newWord);
    }

  }

  newList.print();
  
  return 0;
}
