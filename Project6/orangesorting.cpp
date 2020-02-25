// sorting oranges
// converting vectors to multimaps
// Madison Dockery
// 2/18/2020


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <map>
#include <utility>

using std::cin; using std::cout; using std::endl;
using std::string; using std::multimap; using std::map;
using std::vector;
using std::make_pair;

enum class Variety {orange, pear, apple};
vector<string> colors = {"red", "green", "yellow"};

struct Fruit{
  Variety v;
  string color; // red, green or orange
};


int main(){
  srand(time(nullptr));

  multimap<Variety, string> tree;
  int numOfFruit = rand() % 100 + 1;

  for(int i = 0; i < numOfFruit; ++i)
    tree.insert(make_pair(static_cast<Variety>(rand() % 3), 
			  colors[rand() % 3]));
 
  cout << "Colors of the oranges: ";
  for(auto f = tree.lower_bound(Variety::orange); 
      f != tree.upper_bound(Variety::orange); 
      ++f) //only print if orange
    cout << f->second << ", ";
  cout << endl;

}
