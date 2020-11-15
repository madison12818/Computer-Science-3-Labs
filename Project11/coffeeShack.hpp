// drink class to be used in Coffee Shack lab
// Madison Dockery
// 4/14/20

#ifndef COFFEESHACK_HPP
#define COFFEESHACK_HPP

#include <string>
#include <iostream>
#include <vector>
#include <list>

using std::string; using std::cout; using std::cin; 
using std::vector; using std::list; using std::endl;

enum class DrinkType {small, medium, large};

//
// DECORATOR PATTERN
// component
//
class Drink{
public:
  Drink(DrinkType type = DrinkType::small, double price = 0) 
    : type_(type), price_(price) {}
  virtual double getPrice() const { return price_; }
  virtual string getDrinkName() const;

private: 
  const double price_;
  const DrinkType type_;
  const string drinkName_ = "coffee with ";

};

string Drink::getDrinkName() const {

  string typeName;

  if      (type_ == DrinkType::small)  typeName = "small";
  else if (type_ == DrinkType::medium) typeName = "medium";
  else                                 typeName = "large";

  return typeName + " " + drinkName_;
}

// decorators
//
// sprinkles
class Sprinkles: public Drink {
public:
  Sprinkles(const Drink* wrapped) : wrapped_(wrapped) {}  
  double getPrice() const { return wrapped_->getPrice() + .50; }
  string getDrinkName() const { return wrapped_->getDrinkName() + "sprinkles, "; }

private:
  const Drink *wrapped_; 
};

// caramel
class Caramel: public Drink {
public:
  Caramel(const Drink* wrapped) : wrapped_(wrapped) {}
  double getPrice() const { return wrapped_->getPrice() + .20; }
  string getDrinkName() const { return wrapped_->getDrinkName() + "caramel, "; }

private:
  const Drink *wrapped_;

};

// milk foam
class MilkFoam: public Drink {
public:
  MilkFoam(const Drink* wrapped) : wrapped_(wrapped) {}
  double getPrice() const { return wrapped_->getPrice() + .40; }
  string getDrinkName() const { return wrapped_->getDrinkName() + "milk foam, "; }

private:
  const Drink *wrapped_;

};

// ice
class Ice: public Drink {
public:
  Ice(const Drink* wrapped) : wrapped_(wrapped) {}
  double getPrice() const { return wrapped_->getPrice() + .10; }
  string getDrinkName() const { return wrapped_->getDrinkName() + "ice, "; }

private:
  const Drink *wrapped_;
};

//
// OBSERVER PATTERN
//

class Customer; // concrete observer forward declaration

// concrete subject(observer pattern) and administator (chain pattern)
class Barista {
public:

  // for chain of responsibility
  Barista(Barista* successor = nullptr) : successor_(successor) {}
  virtual Drink* handleRequest(char choice, list<Drink*>& drink) 
  { return successor_->handleRequest(choice, drink); } 

  // for observer
  void registerObserver(Customer *c){ orders_.push_back(c); }
  void deregisterObserver(int num){ orders_.erase(orders_.begin() + num); }
  void notifyObservers() const;

  void finishOrder(); // completes order and erases from vector

  // helpers
  string getCustomerServed(); // returns name of customer served
  bool allCustomersServed() { return orders_.empty(); }
  char askToppings(); // asks for toppings and returns answer

protected:
  static vector<Customer*> orders_; // shared between all baristas
  int customerServed_; // index of customer served

private:
  Barista *successor_; // for chain of responsibility
};

vector<Customer*> Barista::orders_; // initializing static vector

// concrete observer
class Customer {
public:
  Customer(const string& name, Barista* subject, Drink* d)
    : name_(name), subject_(subject), drink_(d) 
  { subject->registerObserver(this); }

  void notify() const;
  string getName() const { return name_; }
  Drink* getDrink() const { return drink_; }

private:
  string name_;
  Drink* drink_;
  Barista *subject_;
};

//
// CHAIN OF RESPONSIBILITY PATTERN
//

// junior barista
class JuniorBarista: public Barista {
public:
  JuniorBarista(Barista* successor = nullptr) : Barista(successor) {}
  Drink* handleRequest(char choice, list<Drink*>& drink) override;
};

// senior barista
class SeniorBarista: public Barista {
public:
  SeniorBarista(Barista* successor = nullptr) : Barista(successor) {}
  Drink* handleRequest(char choice, list<Drink*>& drink) override;
};

// manager
class Manager: public Barista {
public:
  Manager(Barista* successor = nullptr) : Barista(successor) {}
  Drink* handleRequest(char choice, list<Drink*>& drink) override;
};

// member functions
//
// parent barista member functions

void Barista::notifyObservers() const { for(auto e: orders_) e->notify(); }

string Barista::getCustomerServed() 
{ return orders_[customerServed_]->getName(); }

char Barista::askToppings() {

  // ask for topping
  cout << "Would you like to add [s]prinkles, "
       << "[c]aramel, milk [f]oam, [i]ce or [n]ot? ";
  char selection; 
  cin >> selection;

  return selection;
}

void Barista::finishOrder()
{
  customerServed_ =  rand() % orders_.size();

  cout << orders_[customerServed_]->getName() << ", your " 
       << orders_[customerServed_]->getDrink()->getDrinkName() << "is ready. " 
       << "It will be $" << orders_[customerServed_]->getDrink()->getPrice() 
       << ", please." << endl;
  
  this->notifyObservers();
  this->deregisterObserver(customerServed_);
}

// children of barista member functions
////////////////////////////////////////
Drink* JuniorBarista::handleRequest(char choice, list<Drink*>& drink){

  // ask for size
  cout << "Welcome to Coffee Shack, can I get you "
       << "[l]arge, [m]edium, or [s]mall coffee? ";
  cin >> choice;
  
  DrinkType type;
  double price;

  if      
     (choice == 'l') { type = DrinkType::large; price = 3; }
  else if 
     (choice == 'm') { type = DrinkType::medium; price = 2; }
  else                    
                     { type = DrinkType::small; price = 1; }
  
  // begin making the drink
  drink.push_back(new Drink(type, price));
  
  choice = this->askToppings();
  
  if (choice == 'n'){

      cout << "Junior Barista handles request\n";
      return drink.back();
    }

  return Barista::handleRequest(choice, drink);
}

Drink* SeniorBarista::handleRequest(char choice, list<Drink*>& drink){

  while (choice != 'f'){

      cout << "Senior Barista handles request\n";

      // wrap the previous drink with the new topping
      if      
         (choice == 's') drink.push_back(new Sprinkles(drink.back()));
      else if 
         (choice == 'c') drink.push_back(new Caramel(drink.back()));
      else if 
         (choice == 'i') drink.push_back(new Ice(drink.back()));
      else if 
         (choice == 'n') return drink.back();

      choice = this->askToppings();
    }

  return Barista::handleRequest(choice, drink);
}

Drink* Manager::handleRequest(char choice, list<Drink*>& drink){

  while (choice != 'n'){

      cout << "Manager handles request\n";

      // wrap the previous drink with the new topping
      if      
        (choice == 's') drink.push_back(new Sprinkles(drink.back()));
      else if 
        (choice == 'c') drink.push_back(new Caramel(drink.back()));
      else if 
        (choice == 'f') drink.push_back(new MilkFoam(drink.back()));
      else if 
        (choice == 'i') drink.push_back(new Ice(drink.back()));

      choice = this->askToppings();
    } 

  return drink.back();
}

// customer member functions

void Customer::notify() const {

  cout << "Customer " << name_ << " reports order is finished for " 
       << subject_->getCustomerServed() << endl;
}

#endif // COFFEESHACK_HPP
