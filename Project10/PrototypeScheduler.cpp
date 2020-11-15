// main cpp for prototype scheduler
// Madison Dockery
// 4/7/2020

#include "PrototypeScheduler.hpp"
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>

using std::queue; using std::vector;

int main() {

  srand(time(nullptr));

  Process prototype;

  // creating processes from prototype
  const int numOfProcesses = 4;
  vector<Process*> startingProcesses;
  for (int i = 0; i != numOfProcesses; ++i)
    startingProcesses.push_back(prototype.clone());

  queue<Process*> readyQ;
  vector<Process*> waitingList;

  // admit processes and push on the queue
  for (auto process: startingProcesses){

      cout << process->report() << '\n'; // report before transition
      process->admitted();
      readyQ.push(process);
    }
  
  // begin running processes
  int decision;
  while (!readyQ.empty() || !waitingList.empty()){

      if (!readyQ.empty()){

	  // make sure the front process is running
	  readyQ.front()->schedulerDispatch();

	  // decide what to do with running process
	  decision = rand() % 3;
	  switch(decision){

	    case 0: // exit and process is destroyed
	      readyQ.front()->exit();
	      readyQ.pop();
	      break;
	    case 1: // interruption, move to the back of the queue
	      readyQ.front()->interrupt();
	      readyQ.push(readyQ.front());
	      readyQ.pop();
	      break;
	    case 2: // I/O request, process goes to wait list 
	      readyQ.front()->eventWait();
	      waitingList.push_back(readyQ.front());
	      readyQ.pop();
	      break;
	    default:
	      cout << "error\n"; 
	      return 1;

	    }
	}

      // random chance a process from waitingList will become ready
      decision = rand() % 2;
      if (!waitingList.empty() && decision == 0){ // 50% chance of completing a process
       
	  decision = rand() % waitingList.size(); // choose a random process to finish
	  waitingList[decision]->eventCompletion();
	  readyQ.push(waitingList[decision]); // add process back to queue
	  waitingList.erase(waitingList.begin() + decision);
      }
  }
}
