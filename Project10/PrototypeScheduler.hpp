// header file for prototype scheduler sing state and prototype patterns
// Madison Dockery
// 4/7/2020

#ifndef PROTOTYPESCHEDULER_HPP
#define PROTOTYPESCHEDULER_HPP

#include <iostream>
#include <string>

using std::string; using std::cout;

class State; // forward declaration

// Process class

class Process {
public:
  Process();
  Process(State* s) : state_(s), id_(processNum_) 
  { ++processNum_; }

  // behaviors
  void admitted();
  void schedulerDispatch();
  void interrupt();
  void eventCompletion();
  void eventWait();
  void exit();

  string report();
  void changeState(State* s) { state_ = s; }

  // for prototype pattern
  Process* clone() { return new Process(state_); }

private:
  State* state_;
  int id_;
  static int processNum_;
};

int Process::processNum_ = 0;

// Abstract State class

class State {
public:
  // everything defaults to doing nothing
  virtual void admitted(Process*) {}
  virtual void schedulerDispatch(Process*) {}
  virtual void interrupt(Process*) {}
  virtual void eventCompletion(Process*) {}
  virtual void eventWait(Process*) {}
  virtual void exit(Process*) {}

  virtual string report() = 0;
  void changeState(Process* p, State* s) { p->changeState(s); }
};

// States
//
// New

class New: public State {
public:
  // for singleton
  static State* instance(){

    static State* s = new New;
    return s;
  }
  New(const New&) = delete;
  New& operator=(const New&) = delete;
  
  // transitions
  void admitted(Process*) override;

  string report() override { return "new "; }

private:
  New() {}
};

// Ready

class Ready: public State {
public:
  // for singleton
  static State* instance(){

    static State* s = new Ready;
    return s;
  }
  Ready(const Ready&) = delete;
  Ready& operator=(const Ready&) = delete;

  // transitions
  void schedulerDispatch(Process*) override;

  string report() override { return "ready "; }

private:
  Ready() {}
};

// Running

class Running: public State {
public:
  // for singleton
  static State* instance(){

    static State* s = new Running;
    return s;
  }
  Running(const Running&) = delete;
  Running& operator=(const Running&) = delete;

  // transitions
  void interrupt(Process*) override;
  void eventWait(Process*) override;
  void exit(Process*) override;

  string report() override { return "running "; }

private:
  Running() {}
};

// Waiting

class Waiting: public State {
public:
  // for singleton
  static State* instance(){

    static State* s = new Waiting;
    return s;
  }
  Waiting(const Waiting&) = delete;
  Waiting& operator=(const Waiting&) = delete;

  // transitions
  void eventCompletion(Process*) override;

  string report() override { return "waiting "; }

private:
  Waiting() {}
};

// Terminated

class Terminated: public State {
public:
  // for singleton
  static State* instance(){

    static State* s = new Terminated;
    return s;
  }
  Terminated(const Terminated&) = delete;
  Terminated& operator=(const Terminated&) = delete;

  string report() override { return "terminated "; }

private:
  Terminated() {}
};

// state transitions and member functions

void New::admitted(Process* p){

  this->changeState(p, Ready::instance());
  cout << p->report() << '\n';
}

void Ready::schedulerDispatch(Process* p){

  this->changeState(p, Running::instance());
  cout << p->report() << '\n';
}

void Running::interrupt(Process* p){

  this->changeState(p, Ready::instance());
  cout << p->report() << '\n';
}

void Running::eventWait(Process* p) {

  this->changeState(p, Waiting::instance());
  cout << p->report() << '\n';
}

void Running::exit(Process* p) {

  this->changeState(p, Terminated::instance());
  cout << p->report() << '\n';
}

void Waiting::eventCompletion(Process* p){

  this->changeState(p, Ready::instance());
  cout << p->report() << '\n';
}

// Process behaviors

void Process::admitted()          { state_->admitted(this); }
void Process::schedulerDispatch() { state_->schedulerDispatch(this); }
void Process::interrupt()         { state_->interrupt(this); }
void Process::eventCompletion()   { state_->eventCompletion(this); }
void Process::eventWait()         { state_->eventWait(this); }
void Process::exit()              { state_->exit(this); }

string Process::report() {
 
  string str("Process ");
  str += std::to_string(id_);
  str += " is ";
  str += state_->report();
  return str; 
}

Process::Process() : id_(processNum_) 
{ state_ = New::instance(); ++processNum_; }

#endif // PROTOTYPESCHEDULER_HPP
