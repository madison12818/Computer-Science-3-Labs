//Comparing two text files and showing where they are different
//Project #1 Procedural Diff
//Madison Dockery
//01/14/2020

#include <iostream>
#include <fstream>
#include <string>

using std::cout;   using std::endl; using std::ifstream; 
using std::string; using std::getline;

int main(int argc, char *argv[]) { 

  //number of arguments needs to be 3
  if(argc != 3) {
    cout << "number of arguments incorrect" << endl;
    return 1;
  }
  
  //for outputting file name
  string fileName1 = argv[1];
  string fileName2 = argv[2];
  
  //opening file1
  ifstream file1;
  file1.open (fileName1);
  if (!file1.is_open()) {
    cout << "error opening file1" << endl;
    return 1; //exit was giving me warnings
  }

  //opening file2
  ifstream file2;
  file2.open (fileName2);
  if (!file2.is_open()) {
    cout << "error opening file2" << endl;
    return 1;
  }
  
  //declaring variables
  int lineCount = 0;
  string nextLineFile1, nextLineFile2;

  while (!file1.eof() || !file2.eof()){
    
    //reading files
    if(!file1.eof()){
      getline(file1, nextLineFile1);
    }
    else {
      nextLineFile1 = ' '; 
      //even if its end of file1, it needs to be compared with file2
    }

    if(!file2.eof()){
      getline(file2, nextLineFile2);
    }
    else {
      nextLineFile2 = ' ';
    }

    //comparing lines
    if (nextLineFile1 != nextLineFile2) {
    
      string caretsPlace(fileName1.length(), ' '); //assumed fileNames are same length
      string setUp;

      setUp = setUp + ": " + std::to_string(lineCount) + ": ";

      for(int i = 0; i < setUp.length(); ++i){
	caretsPlace += ' '; 
      }
      
      int j = 0;
      while (nextLineFile1[j] == nextLineFile2 [j]){
	caretsPlace += ' ';
	++j;
      }
      
      caretsPlace += '^';

      cout << fileName1 <<  setUp <<  nextLineFile1 << endl 
	   << fileName2 << setUp << nextLineFile2 << endl 
	   << caretsPlace << endl;
    }

    ++lineCount;
  }

  //closing files
  file1.close();
  file2.close();

  return 0;
}
