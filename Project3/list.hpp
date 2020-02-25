// node class tempate used to contruct lists
// 1/28/20
// Madison Dockery

#ifndef LIST_HPP_
#define LIST_HPP_

#include <iostream>

using std::cout; using std::endl;

/////////////////////////////////////////////////////////////////////////////

// the pointer part of every list is the same
// lists differ by data part
template <typename T>
class node{
public:

  node(): next_(nullptr) {}

  // functions can be inlined
  T getData()const{return data_;}
  void setData(const T& data){data_=data;}

  // or can be defined outside
  node<T>* getNext() const;
  void setNext(node<T> *const);

private:
  T data_;
  node<T> *next_;
};

//
// member functions for node
//

/////////////////////////////////////////////////////////////////////////////
template <typename T>
node<T>* node<T>::getNext()const{
  return next_;
}

/////////////////////////////////////////////////////////////////////////////

template<typename T>
void node<T>::setNext(node<T> *const next){
  next_=next;
}

///////////////////////////////////////////////////////////////////////////////

template <typename T>
class Collection;

template <typename T> //need this or will get error inside public for equal
bool equal(const Collection<T>&, const Collection<T>&);

template <typename T>
class Collection {
public:
  Collection() : original(nullptr) {}

  void add(T);
  void remove(T);
  void print();
  T last();

  friend bool equal<T>(const Collection&, const Collection&);

private:

  node<T> *original;

};

/////////////////////////////////////////////////////////////////////////////
//add(): adds to the collection, it does not check for duplicates.

template <typename T>
void Collection<T>::add(T item){
  node<T> *currentNode;
  currentNode = new node<T>;
  
  //is it empty? if yes then set to currentNode and return
  if (original == nullptr){
    currentNode->setData(item);
    original = currentNode;
    return;
  }
  
  //not empty? set node to original
  currentNode = original;

  //move to last space of node
  while (currentNode->getNext() != nullptr){
    currentNode = currentNode->getNext();
  }
  
  //make new node and add it to the end 
  node<T> *addThisNode;
  addThisNode = new node<T>;
  addThisNode->setData(item);
  currentNode->setNext(addThisNode);
  
}

/////////////////////////////////////////////////////////////////////////////
//remove(): removes all instances of item from the collection.

template <typename T>
void Collection<T>::remove(T item){

  node<T> *currentNode, *oldNode, *itemNode;

  //is it empty? then return
  if(original == nullptr){
    return;
  }
  currentNode = original;
  
  //not empty? find where first occurrence of item is in node and delete
  while(currentNode->getData() == item){
    itemNode = currentNode;
    currentNode = currentNode->getNext();
    delete itemNode;

    //if empty once item is removed, set to nullptr and return
    if(currentNode == nullptr){
      original = nullptr;
      return;
    }
  }  

  //move pointer to beginning of new node
  original = currentNode;
  
  //check rest of node until find where another occurrence of item is and delete
  while(currentNode->getNext() != nullptr){
    oldNode = currentNode;
    currentNode = currentNode->getNext();

    if(currentNode->getData() == item){
      itemNode = new node<T>;
      itemNode = currentNode;
      
      currentNode = currentNode->getNext();
      delete itemNode;
      oldNode->setNext(currentNode);
    }
    
  }

  //check the end of node for item and delete if found
  if(currentNode->getData() == item){
    delete currentNode;
    oldNode->setNext(nullptr);
  }
  
}

/////////////////////////////////////////////////////////////////////////////
//last(): returns the last item added to the collection.

template <typename T>
T Collection<T>::last(){
  node<T> *currentNode;
  currentNode = new node<T>;
  currentNode = original;
  
  //move to last space of node
  while (currentNode->getNext() != nullptr){
    currentNode = currentNode->getNext();
  }
  
  return currentNode->getData();
  
}

/////////////////////////////////////////////////////////////////////////////
//print(): prints all items in the collection. The printout does not have to be in order.

template <typename T>
void Collection<T>::print(){
  node<T> *currentNode;
  currentNode = new node<T>;
  currentNode = original;
  
  //need to stop printing when empty
  while(currentNode != nullptr){
    cout << currentNode->getData() << endl;
    currentNode = currentNode->getNext();    
  }

}

/////////////////////////////////////////////////////////////////////////////
//bool equal(const Collection&, const Collection&) : compares two collections for equality.

template <typename T>
bool equal(const Collection<T>& lhs, const Collection<T>& rhs){
  
  node<T> *lhsNode, *rhsNode;
  lhsNode = new node<T>;
  rhsNode = new node<T>;
  
  //checking all elements - starting with first and ending with nullptr
  for (lhsNode = lhs.original, rhsNode = rhs.original; 
       (lhsNode != nullptr) && (rhsNode != nullptr); 
       lhsNode = lhsNode->getNext(), rhsNode = rhsNode->getNext()){
  
    if(lhsNode->getData() != rhsNode->getData()){
      return false;
    }
  }

  //need to check both ways or it may return false if nodes are same sizes
  if((lhsNode == nullptr) && (rhsNode != nullptr)){
    return false;
  }
  else if((lhsNode != nullptr) && (rhsNode == nullptr)){
    return false;
  }
  
  //everything has been checked so now can return true
  return true;
}

/////////////////////////////////////////////////////////////////////////////

#endif // LIST_HPP_
