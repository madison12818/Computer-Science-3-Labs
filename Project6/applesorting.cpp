// sorting apples
// non STL-algorithm code to be replaced by algorthms
// Madison Dockery
// 2/18/2020


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>

using std::cin; using std::cout; using std::endl;
using std::string;
using std::vector; using std::deque;

struct Apples{
  double weight; // oz
  string color;  // red or green
  void print() const { cout << color << ", " <<  weight << endl; }
  bool operator<(const Apples& rhs) const { return weight < rhs.weight; }
};

bool isLessThan(const Apples& a, const double& maxWeight) { return a.weight < maxWeight; }

int main(){

  srand(time(nullptr));
  const double minWeight = 3.;
  const double maxWeight = 8.;

  cout << "Input crate size: ";
  int size;
  cin >> size;

  vector <Apples> crate(size);

  ///////////////////////////////////////////////////////////////////////////////////

  /*
  // assign random weight and color to apples in the crate
  // replace with generate()
  for(auto it = crate.begin(); it != crate.end(); ++it){
      it->weight = minWeight + 
	static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
      it->color = rand() % 2 == 1 ? "green" : "red";
  }
  */

  //generate(forwardIterator first, forward Iterator last, generator gen(lambda))
  std::generate(crate.begin(), crate.end(), [&](){ //[by reference]()
      Apples a;
      a.weight =  minWeight +
        static_cast<double>(rand())/RAND_MAX*(maxWeight - minWeight);
      a.color = rand() % 2 == 1 ? "green" : "red";
      return a;
    });

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "Enter weight to find: ";
  double toFind;
  cin >> toFind;

  ///////////////////////////////////////////////////////////////////////////////////

  // count_if()
  /*
  int cnt = 0;
  for(auto it = crate.cbegin(); it != crate.cend(); ++it) 
    if(it->weight > toFind) ++cnt;
  */
  
  //count_if(inputIterator first, inputIterator last, unaryPredicate pred(lambda))
  int cnt = count_if(crate.begin(), crate.end(), [=](Apples b){
      return b.weight > toFind;
    });

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "There are " << cnt << " apples heavier than " 
       << toFind << " oz" <<  endl;

  cout << "at positions ";

  ///////////////////////////////////////////////////////////////////////////////////

  /*
  // find_if()
  for(int i=0; i < size; ++i)
    if(crate[i].weight > toFind)
      cout << i << ", ";
  */

  //find_if(inputIterator first, inputIterator last, unaryPredicate pred(lambda)
  for(auto applesIndex = find_if(crate.begin(), crate.end(), 
			    [=](Apples a){return a.weight > toFind; }); 
      applesIndex != crate.end(); 
      applesIndex = find_if(++applesIndex, crate.end(), 
			    [=](Apples a){return a.weight > toFind; })){

    cout << std::distance(crate.begin(), applesIndex) << ", ";
  }

  ///////////////////////////////////////////////////////////////////////////////////

  cout << endl;

  ///////////////////////////////////////////////////////////////////////////////////

  /*
  // max_element()
  double heaviest = crate[0].weight;
  for(int i=1; i < size; ++i)
    if(crate[i].weight > heaviest) heaviest = crate[i].weight; 
  */

  // max_element(forwardIterator first, forwardIterator last)
  auto heaviest = std::max_element(crate.begin(), crate.end());
  

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "Heaviest apple weighs: " << heaviest->weight << " oz" << endl;

  ///////////////////////////////////////////////////////////////////////////////////
  
  /*
  // for_each() or accumulate()
  double sum = 0;
  for(int i=0; i < size; ++i)
    sum += crate[i].weight;
  */
  
  //accumulate(inputIterator first, inputIterator last, integer, function fn)
  double sum = std::accumulate(crate.begin(), crate.end(), 0, [](double num, Apples a){
      return num + a.weight;
    });
  

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "Total apple weight is: " << sum << " oz" << endl;
  cout << "How much should they grow: ";
  double toGrow;
  cin >> toGrow;

  ///////////////////////////////////////////////////////////////////////////////////

  /*
  // transform();
  for(int i=0; i < crate.size(); ++i)
    crate[i].weight += toGrow;
  */
  
  //transform(inputIterator first1, inputIterator last1, 
  //          outputIterator result, unaryOperator op[lambda])  
  std::transform(crate.begin(), crate.end(), crate.begin(), [=](Apples z){ 
      z.weight += toGrow; 
      return z;
    });

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "Input minimum acceptable weight: ";
  double minAccept;
  cin >> minAccept;

  ///////////////////////////////////////////////////////////////////////////////////

  // remove_if()   
  /*
  for(auto it=crate.begin(); it != crate.end(); )
    if(it->weight < minAccept)
      it = crate.erase(it);
    else
      ++it;
  */
  
  //remove_if(forwardIterator first, forwardIterator last, unaryPredicate pred(lambda))
  auto eraseItem = std::remove_if(crate.begin(), crate.end(), [=](Apples g){
      return g.weight < minAccept;
    });
  
  //erase(eraseItem, untilHere)
  crate.erase(eraseItem, crate.end());

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "removed " << size - crate.size() << " elements" << endl;

  ///////////////////////////////////////////////////////////////////////////////////

  /*  
  // bubble sort, replace with sort()
  bool swapped;
  do{
    swapped = false;
    for(int i=0; i < crate.size()-1; ++i)
      if(crate[i].weight > crate[i+1].weight){
	std::swap(crate[i], crate[i+1]);
	swapped = true;
      }
  }while(swapped);
  */

  //sort(randomAccessIterator first, randomAccessIterator last)
  std::sort(crate.begin(), crate.end());  

  ///////////////////////////////////////////////////////////////////////////////////

  // moving all red apples from crate to peck
  // remove_copy_if() with back_inserter()/front_inserter() or equivalents
  
  deque<Apples> peck;

  /*
  for(auto it=crate.begin(); it != crate.end();)
    if(it->color == "red"){
      peck.push_front(std::move(*it));
      it=crate.erase(it);
    }else
      ++it;
  */

  std::remove_copy_if(crate.begin(), crate.end(), std::front_inserter(peck),
		      [=](Apples a){ return a.color != "red"; });
  auto removeIt = std::remove_if(crate.begin(), crate.end(), 
				 [=](Apples a) { return a.color == "red"; });
  crate.erase(removeIt, crate.end());

  ///////////////////////////////////////////////////////////////////////////////////

  cout << "apples in the create"<< endl;
  
  ///////////////////////////////////////////////////////////////////////////////////

  /*
  // for_each() possibly
  for(const auto &e: crate) {
    e.print(); 
  }
  */

  //for_each(inputIterator first, inputIterator last, function fn)
  for_each(crate.begin(), crate.end(), [](Apples p){ p.print(); });

  ///////////////////////////////////////////////////////////////////////////////////

  cout << endl;
  cout << "apples in the peck"<< endl;

  ///////////////////////////////////////////////////////////////////////////////////

  /*
  // for_each() possibly
   for(const auto &e: peck) {
      e.print();
   } 
  */

  //for_each(inputIterator first, inputIterator last, function fn)
  for_each(peck.begin(), peck.end(), [](Apples e){ e.print(); });

  ///////////////////////////////////////////////////////////////////////////////////
  
  // prints every "space" apple in the peck
  // 
  const int space=3; 
  cout << "\nevery " << space << "\'d apple in the peck"<< endl;

   ///////////////////////////////////////////////////////////////////////////////////

   // replace with advance()/next()/distance()
   // no iterator arithmetic
   /*
   auto it2=peck.cbegin(); int i = 0;   
   while(it2 != peck.cend()){
      if(i == space){
        it2->print();
        i=0;
      }
      ++i; 
      ++it2;
   }
  */

  for (auto it = std::next(peck.cbegin(), space); 
       std::distance(it, peck.cend()) > 0; 
       it = std::next(it, space)) 
    it->print();

   ///////////////////////////////////////////////////////////////////////////////////

   // putting all small green apples in a jam
   // use a binder to create a functor with configurable max weight
   // count_if() then remove_if()
   
  const double weightToJam = 10.0;
  double jamWeight = 0; 

  /*
    for(auto it=crate.begin(); it != crate.end();)
    if(it->weight < weightToJam){
    jamWeight += it->weight;
    it=crate.erase(it);
    }else
    ++it;
  */

  // create bind
  auto lessThanBind = std::bind(isLessThan, std::placeholders::_1, weightToJam);
  
  // get total apples with weight less than max
  int total = std::count_if(crate.begin(), crate.end(), lessThanBind);
  
  auto it = crate.cbegin();
  for (int i = 0; i != total; ++it)
    if (lessThanBind(*it))
      {
        jamWeight += it->weight;
        ++i;
      }
  cout << "Weight of jam is: " << jamWeight << endl;
}
