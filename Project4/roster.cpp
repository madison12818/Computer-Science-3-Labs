// vector and list algorithms
// Madison Dockery
// 02/04/20

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move;

// reading a list from a fileName
void readRoster(list<string>& roster, string fileName);  

// printing a list out
void printRoster(const list<string>& roster); 

int main(int argc, char* argv[]){

  if (argc <= 1){ 
    cout << "usage: " << argv[0] 
    << " list of courses, dropouts last" 
    << endl; exit(1);
  }

  // vector of courses of students
  vector <list<string>> courseStudents; 

  for(int i=1; i < argc-1; ++i){
    list<string> roster;
    readRoster(roster, argv[i]);  
    cout << "\n\n" << argv[i] << "\n";  
    printRoster(roster);
    courseStudents.push_back(move(roster)); 
  }

  // reading in dropouts
  list<string> dropouts; 
  readRoster(dropouts, argv[argc-1]); 
  cout << "\n\n dropouts \n"; printRoster(dropouts);

  // master list of students
  list<string> allStudents;  

  for(auto lst : courseStudents) 
    allStudents.splice(allStudents.end(), lst);

  cout << "\n\n all students unsorted \n"; 
          printRoster(allStudents);

  // sorting master list
  allStudents.sort();
  cout << "\n\n all students sorted \n"; 
  printRoster(allStudents);

  // eliminating duplicates
  allStudents.unique(); 
  cout << "\n\n all students, duplicates removed \n"; 
  printRoster(allStudents);

  // removing individual dropouts
  for (const auto& str : dropouts)  
    allStudents.remove(str);

  cout << "\n\n all students, dropouts removed \n"; 
  printRoster(allStudents);


  // creating student entries
  vector<list<string>> studentEntries;

  // adding names to roster
  std::list<string>::iterator it = allStudents.begin();

  while(it != allStudents.end()) {
    
    // make a list for each student and add each list to student entries
    list<string> student;
    student.push_back(*it);
    studentEntries.push_back(student);
    ++it;
  }


  // iterating through the classes
  for(int i = 0; i != courseStudents.size(); ++i){
    
    // sort roster and get name of class
    courseStudents[i].sort();
    // courseStudents[0] holds the roster for class in argv[1]
    string className(argv[i+1]); // has .txt at the end
    className.erase(className.end()-4, className.end()); // removing .txt
    
    // creating iterators
    int studentsIndex = 0;
    std::list<string>::iterator rosterIndex = courseStudents[i].begin();

    // searching for student name in roster
    while(rosterIndex != courseStudents[i].end() && studentsIndex != studentEntries.size()){

      // if name is in roster then add class and iteratate index's
      if(*rosterIndex == studentEntries[studentsIndex].front()){
        studentEntries[studentsIndex].push_back(className);
        ++studentsIndex; ++rosterIndex;
      }
      else if(*rosterIndex > studentEntries[studentsIndex].front()){
        ++studentsIndex;
      }
      else{
        ++rosterIndex;
      }
    }
  }
    
  // print out students and classes intro given for lab
  cout << "All Students" << endl;
  cout << "last name, first name: courses enrolled" << endl;
  
  // print out student entries
  for(auto &student: studentEntries){
    // print name then delete
    cout << student.front() << ": ";
    student.pop_front();

    // print classes
    auto it = student.begin(); 
    while(it != student.end()){
      cout << *it << " ";
      ++it;
    }

    cout << endl;
  }
}

// reading in a file of names into a list of strings
void readRoster(list<string>& roster, string fileName){
   ifstream course(fileName);    
   string first, last;
   while(course >> first >> last)
      roster.push_back(move(first + ' ' + last));
   course.close();
}

// printing a list out
void printRoster(const list<string>& roster){
   for(const auto& str : roster)
      cout << str << endl;
}
