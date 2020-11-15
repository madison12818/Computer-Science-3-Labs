// Singleton Logger
// implements logging into a file using Singleton design pattern
// Madison Dockery
// 3/10/2020

#include <iostream>
#include <fstream>
#include <string>

using std::cout;    using std::endl;
using std::fstream; using std::string;

class Logger {
public:
  static Logger& instance(); 
  void report(const string&);
  ~Logger();
  
  // prohibit these
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  
private:
  Logger();
  fstream fout;  
};

Logger& Logger::instance() { 
  static Logger log; 
  return log; 
}

void Logger::report(const string& log){
  cout << "Logging: " << log << " to file" << endl;
  fout << log << endl;
}

Logger::Logger(){
  fout.open("log.txt", fstream::out | fstream::app);

  if(!fout.is_open())
    cout << "log.txt couldn't be opened." << endl;
  else 
    cout << "log.txt is now open" << endl;
}

Logger::~Logger(){
  fout.close();
  cout << "log.txt is now closed" << endl;
}

int main(){
  Logger::instance().report("add this to the doc");
  Logger::instance().report("diabetes is scarier than corona virus");
}
