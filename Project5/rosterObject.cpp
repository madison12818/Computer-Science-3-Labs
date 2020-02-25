// vector and list algorithms with objects in containers
// Madison Dockery
// 2/11/2020

#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstdlib>
#include <map>

using std::ifstream;
using std::string; using std::getline;
using std::list; using std::vector;
using std::cout; using std::endl;
using std::move; using std::map;


class Student{
public:
   Student(string firstName, string lastName): 
      firstName_(firstName), lastName_(lastName) {}
 
   // move constructor, not really needed, generated automatically
   Student(Student && org):
      firstName_(move(org.firstName_)),
      lastName_(move(org.lastName_))
   {}
  
   // force generation of default copy constructor
   Student(const Student & org) = default;
   
   string print() const {return firstName_ + ' ' + lastName_;}

    // needed for unique() and for remove()
   friend bool operator== (Student left, Student right){
      return left.lastName_ == right.lastName_ &&
	     left.firstName_ == right.firstName_;
   }

   // needed for sort()
   friend bool operator< (Student left, Student right){
      return left.lastName_ < right.lastName_ ||
	     (left.lastName_ == right.lastName_ && 
	      left.firstName_ < right.firstName_);
   }
private:
   string firstName_;
   string lastName_;
};



// reading a list from a fileName
void readRoster(list<Student>& roster, string fileName);  
// printing a list out
void printRoster(const list<Student>& roster); 

int main(int argc, char* argv[]){

   if (argc <= 1){ cout << "usage: " << argv[0] 
      << " list of courses, dropouts last" << endl; exit(1);}

   // vector of courses of students
   vector <list<Student>> courseStudents; 

   for(int i=1; i < argc-1; ++i){
      list<Student> roster;
      readRoster(roster, argv[i]);  
      cout << "\n\n" << argv[i] << "\n";  
      printRoster(roster);
      courseStudents.push_back(move(roster)); 
   }


   // reading in dropouts
   list<Student> dropouts; 
   readRoster(dropouts, argv[argc-1]); 
   cout << "\n\n dropouts \n"; printRoster(dropouts);

   list<Student> allStudents;  // master list of students
 
   for(auto lst : courseStudents) 
     allStudents.splice(allStudents.end(),lst);

   cout << "\n\n all students unsorted \n"; 
           printRoster(allStudents);

   allStudents.sort(); // sorting master list
   cout << "\n\n all students sorted \n"; printRoster(allStudents);

   allStudents.unique(); // eliminating duplicates
   cout << "\n\n all students, duplicates removed \n"; printRoster(allStudents);
   
   for (const auto& str : dropouts)  // removing individual dropouts
      allStudents.remove(str);
   cout << "\n\n all students, dropouts removed \n"; printRoster(allStudents);


   //creating student entries
   map<Student, list<string>> studentEntries;

   // creating student keys to add names to it later
   for(const auto& student: allStudents){
     studentEntries[student] = list<string>();
   }
   
   // go through all rosters given
   for(int i = 0; i != courseStudents.size(); ++i){
     
     // get name of class with .txt at the end and delete .txt
     string className(argv[i+1]);
     className.erase(className.end()-4, className.end());
     
     // need to start the search at the very beginning
     auto it = courseStudents[i].begin();
     
     // search the roster for student names
     while(it != courseStudents[i].end()){
       
       auto studentLog = studentEntries.find(*it);
       
       // see if student is already on roster
       if(studentLog != studentEntries.end()){
	 // if the name is already in roster, add class name to their list
	 studentLog->second.push_back(className);  
       }
       
       // increment through while loop
       ++it;
     }
     
   }
   
   // intro that is given is to be printed first
   cout << "All Students" << endl;
   cout << "last name, first name: courses enrolled" << endl;
   
   // print out the student entries
   auto student = studentEntries.begin();
   while(student != studentEntries.end()){
     cout << student->first.print() << ": ";
     
     // print out the classes for the student
     for(auto it = student->second.cbegin();
	 it != student->second.cend(); ++it){
       cout << *it << " ";
     }
     
     cout << endl;
     
     // increment through while loop for student name
     ++student;
   }
}

void readRoster(list<Student>& roster, string fileName){
  ifstream course(fileName);
  string first, last;
  while(course >> first >> last)
    roster.push_back(Student(first, last));
  course.close();
}

// printing a list out
void printRoster(const list<Student>& roster){
  for(const auto& student : roster)
    cout << student.print() << endl;
}
