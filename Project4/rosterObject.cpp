// vector and list algorithms with objects in containers
// Madison Dockery
// 02/04/2020

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

  // needed to add course to list
  void addCourse(const string& name){
    courses.push_back(name);
  }

  // used to print out courses
  void printCourses() const {
    auto it = courses.cbegin();
    while(it != courses.cend()){
      cout << *it << " ";
      ++it;
    }
    cout << endl;
  }

private:
  
  string firstName_;
  string lastName_;
  list<string> courses;
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



  // need to iterate through all classes
  for(int i = 0; i != courseStudents.size(); ++i){
    
    // sort roster and get name of the class
    courseStudents[i].sort();
    // courseStudents[0] holds the roster for class in argv[1]
    string className(argv[i+1]); // has .txt at the end
    className.erase(className.end() - 4, className.end()); // removing .txt
    
    // creating iterators
    std::list<Student>::iterator studentsIndex = allStudents.begin();
    std::list<Student>::iterator rosterIndex = courseStudents[i].begin();

    // search for the students name in the class roster
    while(rosterIndex != courseStudents[i].end() && studentsIndex != allStudents.end()){
      
      //if name is in roster then add the class and increase both indexes
      if(*rosterIndex == *studentsIndex){
        studentsIndex->addCourse(className);
        ++studentsIndex; ++rosterIndex;
      }
      else if(*studentsIndex < *rosterIndex){
        ++studentsIndex;
      }
      else{
        ++rosterIndex;
      }
      
    }
  }

  // now print out the intro given for the lab
  cout << "All Students" << endl;
  cout << "last name, first name: courses enrolled" << endl;

  // printing out names and courses
  for(const auto &student: allStudents){
    cout << student.print() << ": "; 
    student.printCourses();
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
