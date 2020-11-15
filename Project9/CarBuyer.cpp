// car buying - ford or toyota
// Madison Dockery
// 3/31/2020

#include <iostream>
#include <vector>
#include <cstdlib>
#include <array>
#include "CarFactory.hpp"

using std::vector;
using std::cout; using std::endl;

class CarLot{
public:
  CarLot();
  Car* nextCar();
  int lotSize() { return maxCars4Sale_; }
  Car* testDriveCar(){ return car4sale_; }

  // if a car is bought, requests a new one
  Car *buyCar(){ 
    Car *bought = cars4sale_[carNum_];
    cars4sale_.erase(cars4sale_.begin() + carNum_); // sold car
    cars4sale_.push_back(factories_[rand()%factories_.size()]->requestCar());
    return bought;
  }
       
private:
    vector<Car *> cars4sale_; // cars for sale at the lot
    vector<CarFactory *> factories_;
    static const int maxCars4Sale_ = 10;
    static int carNum_; // current car being looked at

};

  int CarLot::carNum_ = -1;

CarLot::CarLot(){
  // creates 2 Ford factories and 2 Toyota factories 
  factories_.push_back(new FordFactory());   
  factories_.push_back(new ToyotaFactory());
  factories_.push_back(new FordFactory());
  factories_.push_back(new ToyotaFactory());

  // filling the lot with cars
  for(int i = 0; i != maxCars4Sale_; ++i){
    cars4sale_.push_back(factories_[rand() % factories_.size()] -> requestCar());
  }
}

Car* CarLot::nextCar(){
  carNum_ = (++carNum_) % maxCars4Sale_;
  return cars4sale_[carNum_];
}


CarLot *carLotPtr = nullptr; // global pointer instantiation

void carBuyer(int id){
  if(carLotPtr == nullptr)
    carLotPtr = new CarLot();

  std::string requestedMake = rand() %2 == 0 ? "Ford" : "Toyota";

  // making arrays of possible models
  static const std::array<std::string, 4>
    fordModels = {"Focus", "Mustang", "Explorer", "F-150"};
  static const std::array<std::string, 5>
    toyotaModels = {"Corolla", "Camry", "Prius", "4Runner", "Yaris"};

  // selecting random model
  std::string requestedModel = requestedMake == "Ford" ? 
    fordModels[rand() % fordModels.size()] : toyotaModels[rand() % toyotaModels.size()];

  cout << "Buyer " << id << " wants a " << requestedMake << " " << requestedModel << endl;

  Car *toBuy;
  bool foundCarTheyLike = false;
  
  // finding one they like 
  for(int i = 0; i != carLotPtr->lotSize() && !foundCarTheyLike; __i){
    
    // test drive car
    toBuy = carLotPtr->nextCar();
    cout << "Car being looked at is a " << toBuy->getMake() << " "
	 << toBuy->getModel() << endl;
    
    // check to see if they like it
    if(toBuy->getMake() == requestedMake && toBuy->getModel() == requestedModel)
      foundCarTheyLike = true;
  }

  if(foundCarTheyLike){
    cout << "Found what they want to buy!" << endl << endl;
    carLotPtr->buyCar();
  }
  else
    cout << "Cound't find what car they wanted." << endl << endl;
}

int main() {
  srand(time(nullptr));

  const int numBuyers=10;
  for(int i=0; i < numBuyers; ++i)
    carBuyer(i);
  
}
