// traversing genealogical tree using Composite and Visitors
// Madison Dockery
// 4/21/20

#include <iostream>
#include <string>
#include <vector>

using std::cout; using std::endl; using std::cin;
using std::string; using std::vector;

class Person{ // component

public:
  Person(string firstName, 
	 Person *spouse, 
	 Person *father, 
	 Person *mother):
    firstName_(firstName), spouse_(spouse), 
    father_(father), mother_(mother){}
  string getFirstName(){return firstName_;}
  Person *getSpouse(){return spouse_;}
  void setSpouse(Person *spouse){spouse_=spouse;}
  Person *getFather(){return father_;}

  virtual void accept(class PersonVisitor *)=0;  
  virtual ~Person(){}

private:
  string firstName_;
  Person *spouse_;
  Person *father_;
  Person *mother_;
};

// composites

// man has a last name 
class Man: public Person{

public:
  Man(string lastName, 
      string firstName, 
      Person *spouse, 
      Person *father, Person *mother): 
    lastName_(lastName), 
    Person(firstName, spouse, father, mother){}
  string getLastName(){return lastName_;}
  void accept(class PersonVisitor *visitor) override;

private:
  string lastName_;
};

// woman has a list of children
class Woman: public Person{

public: 
  Woman(vector<Person *> children, 
	string firstName, 
	Person *spouse, 
	Person *father, Person *mother): 
    children_(children),
    Person(firstName, spouse, father, mother){}
  const vector<Person *> & getChildren()
  {return children_;}
  void setChildren(const vector<Person *> &children)
  {children_ = children;}
  void accept(class PersonVisitor *visitor) override;

private:
  vector<Person *> children_;
}; 

// abstract visitor
class PersonVisitor{

public:
  virtual void visit(Man*)=0;
  virtual void visit(Woman*)=0;
  virtual ~PersonVisitor(){}
};

// composite methods that define traversal
void Man::accept(PersonVisitor *visitor)
{visitor->visit(this);}

void Woman::accept(PersonVisitor *visitor){ 
  // children traversal through mother only
  // father's children are not traversed so as not 
  // to traverse them twice: for mother and father

  visitor->visit(this);
  // traversing descendants
  for(auto child : children_) 
    child->accept(visitor);
}

// concrete visitors
// the last name for a man is stored in object
// the last name a woman is determined by her 
// spouse if she is married
// or by her father if she is not
class NamePrinter: public PersonVisitor{
public:

  void visit(Man *m) override {
    cout << m->getFirstName() << " " 
         << m->getLastName() << endl;
  }

  void visit(Woman *w) override {
    cout << w->getFirstName() << " ";
    if (w->getSpouse() != nullptr)
      cout << static_cast<Man *>
	(w->getSpouse())->getLastName();
    else if (w->getFather() != nullptr)
      cout << static_cast<Man *>
	(w->getFather())->getLastName();
    else
      cout << "Doe";
    cout << endl;
  }
};


class ChildrenPrinter: public PersonVisitor{
public:

  void visit(Man *m) override {
    cout << m->getFirstName() << ": ";
    Woman *spouse = static_cast<Woman *>
      (m->getSpouse());
    if(spouse != nullptr)
      printNames(spouse->getChildren());
    cout << endl;
  }

  void visit(Woman *w) override {
    cout << w->getFirstName() << ": ";
    printNames(w->getChildren());
    cout << endl;
  }

private:
  void printNames(const vector<Person *> &children){
    for(const auto c: children)
      cout << c->getFirstName() << ", ";
  }
};

// maiden name visitor
class MaidenPrinter: public PersonVisitor {
public:
  void visit(Man *m) override {
    
    cout << "This is a man" << endl;
    
    Woman *spouse = static_cast<Woman*>(m->getSpouse());
    
    if (spouse != nullptr)
      spouse->accept(this);
  }

  void visit(Woman *w) override {
    
    Man* father = static_cast<Man*>(w->getFather());
    
    if (father != nullptr)
      cout << "Maiden name for " << w->getFirstName()
	   << " is " << father->getLastName() << endl;
    else
      cout << w->getFirstName() 
	   << " a first generation woman" << endl;
  }
};

// marriage advisor
class MarriageAdvisor: public PersonVisitor {
public:

  MarriageAdvisor(string oneFirst, string oneLast,
		  string twoFirst, string twoLast):
    oneFirst_(oneFirst), oneLast_(oneLast),
    twoFirst_(twoFirst), twoLast_(twoLast) {}

  void visit(Man *m) override {

    if (m->getFirstName() == oneFirst_ && m->getLastName() == oneLast_) {
      personOneFound_ = true;
      checkIfCanMarry(m);
    }

    else if (m->getFirstName() == twoFirst_ && m->getLastName() == twoLast_) {
      personTwoFound_ = true;
      checkIfCanMarry(m);
    }    
  }

  void visit(Woman *w) override {
    
    Man *lastNameHolder = static_cast<Man*>(w->getSpouse());
    
    if (lastNameHolder == nullptr)
      lastNameHolder = static_cast<Man*>(w->getFather());

    if (w->getFirstName() == oneFirst_ && lastNameHolder->getLastName() == oneLast_) {
      personOneFound_ = true;
      checkIfCanMarry(w);
    }

    else if (w->getFirstName() == twoFirst_ && lastNameHolder->getLastName() == twoLast_) {
      personTwoFound_ = true;
      checkIfCanMarry(w);
    }
  }

  void checkIfCanMarry(Woman *w) {
    
    // check spouse
    if (w->getSpouse() != nullptr) {
      canMarry_ = false;
      return;
    }
    
    // if marrying themselves
    if (oneFirst_ == twoFirst_ && twoLast_ == oneLast_) {
      canMarry_ = false;
      return;
    }

    bool oneFound = personOneFound_;

    // check father
    Man *father = static_cast<Man*>(w->getFather());
    
    if (father != nullptr) {
      
      if (oneFound && 
	  father->getFirstName() == twoFirst_ &&
	  father->getLastName() == twoLast_) {
	canMarry_ = false;
	return;
      }
      
      if (!oneFound && 
	  father->getFirstName() == oneFirst_ &&
	  father->getLastName() == oneLast_) {
	canMarry_ = false;
	return;
      }
      
      // check for parents spouses siblings
      Man *fatherOfFather = static_cast<Man*> (father->getFather());

      if (fatherOfFather != nullptr) {
	Woman *motherOfFather = static_cast<Woman*> (father->getFather()->getSpouse());
	
	if (motherOfFather != nullptr) {
	  
	  for (auto e: motherOfFather->getChildren()) {
	    
	    if (e->getSpouse() == nullptr && fatherOfFather != nullptr) {
	      
	      if (oneFound && 
		  fatherOfFather->getLastName() == twoLast_ &&
		  e->getFirstName() == twoFirst_) {
		canMarry_ = false;
		return;
	      }
	      
	      if (!oneFound && 
		  fatherOfFather->getLastName() == oneLast_ &&
		  e->getFirstName() == oneFirst_) {
		canMarry_ = false;
		return;
	      }
	    }
	  }
	}
      }
    }

    // check mother
    Woman *mother = static_cast<Woman*>(father->getSpouse());
    
    if (mother != nullptr) {
      
      if (oneFound && 
	  mother->getFirstName() == twoFirst_ &&
	  father->getLastName() == twoLast_) {
	canMarry_ = false;
	return;
      }
      
      if (!oneFound && 
	  mother->getFirstName() == oneFirst_ &&
	  father->getLastName() == oneLast_) {
	canMarry_ = false;
	return;
      }

      // check for siblings
      for (auto e: mother->getChildren()) {
	
	if (e->getSpouse() == nullptr && father != nullptr) {
	  
	  if (oneFound && 
	      father->getLastName() == twoLast_ &&
	      e->getFirstName() == twoFirst_) {
	    canMarry_ = false;
	    return;
	  }
	  
	  if (!oneFound && 
	      father->getLastName() == oneLast_ &&
              e->getFirstName() == oneFirst_) {
            canMarry_ = false;
            return;
          }
	}
      }

      // check for parents spouses siblings
      Man *fatherOfMother = static_cast<Man*> (mother->getFather());
      
      if (fatherOfMother != nullptr) {
	Woman *motherOfMother = static_cast<Woman*> (mother->getFather()->getSpouse());
	
	if (motherOfMother != nullptr) {
	  
	  for (auto e: motherOfMother->getChildren()) {
	    
	    if (e->getSpouse() == nullptr && fatherOfMother != nullptr) {
	      
	      if (oneFound && 
		  fatherOfMother->getLastName() == twoLast_ &&
		  e->getFirstName() == twoFirst_) {
		canMarry_ = false;
		return;
	      }
	      
	      if (!oneFound && 
		  fatherOfMother->getLastName() == oneLast_ &&
		  e->getFirstName() == oneFirst_) {
		canMarry_ = false;
		return;
	      }
	    }
	  }
	}
      }
    }
  }

  void checkIfCanMarry(Man *m) {
    
    // check spouse
    if (m->getSpouse() != nullptr) {
      canMarry_ = false;
      return;
    }
    
    // if marrying themselves
    if (oneFirst_ == twoFirst_ && twoLast_ == oneLast_) {
      canMarry_ = false;
      return;
    }

    bool oneFound = personOneFound_;

    // check father
    Man *father = static_cast<Man*>(m->getFather());
    
    if (father != nullptr) {
      
      if (oneFound && 
	  father->getFirstName() == twoFirst_ &&
          father->getLastName() == twoLast_) {
        canMarry_ = false;
        return;
      }

      if (!oneFound && 
	  father->getFirstName() == oneFirst_ &&
          father->getLastName() == oneLast_) {
        canMarry_ = false;
        return;
      }

      // check for parents spouses siblings
      Man *fatherOfFather = static_cast<Man*> (father->getFather());
      
      if (fatherOfFather != nullptr) {
	Woman *motherOfFather = static_cast<Woman*> (father->getFather()->getSpouse());
	
	if (motherOfFather != nullptr) {
	  
	  for (auto e: motherOfFather->getChildren()) {
	    
	    if (e->getSpouse() == nullptr && fatherOfFather != nullptr) {
	      
	      if (oneFound && 
		  fatherOfFather->getLastName() == twoLast_ &&
		  e->getFirstName() == twoFirst_) {
		canMarry_ = false;
		return;
	      }
	      
	      if (!oneFound && 
		  fatherOfFather->getLastName() == oneLast_ &&
		  e->getFirstName() == oneFirst_) {
		canMarry_ = false;
		return;
	      }
	    }
	  }
	}
      }
    }
    // check mother
    Woman *mother = static_cast<Woman*>(father->getSpouse());
    
    if (mother != nullptr) {
      
      if (oneFound && 
	  mother->getFirstName() == twoFirst_ &&
          father->getLastName() == twoLast_) {
        canMarry_ = false;
        return;
      }
      
      if (!oneFound && 
	  mother->getFirstName() == oneFirst_ &&
          father->getLastName() == oneLast_) {
        canMarry_ = false;
        return;
      }

      // check for parents spouses siblings
      Man *fatherOfMother = static_cast<Man*> (mother->getFather());
      
      if (fatherOfMother != nullptr) {
	Woman *motherOfMother = static_cast<Woman*> (mother->getFather()->getSpouse());

	if (motherOfMother != nullptr) {
	  
	  for (auto e: motherOfMother->getChildren()) {
	    
	    if (e->getSpouse() == nullptr && fatherOfMother != nullptr) {
	      
	      if (oneFound && 
		  fatherOfMother->getLastName() == twoLast_ &&
		  e->getFirstName() == twoFirst_) {
		canMarry_ = false;
		return;
	      }

	      if (!oneFound && 
		  fatherOfMother->getLastName() == oneLast_ &&
		  e->getFirstName() == oneFirst_) {
		canMarry_ = false;
		return;
	      }
	    }
	  }
	}
      }
    }
  }
  
  bool getCanMarry() const {return canMarry_;}

private:
  string oneFirst_;
  string oneLast_;
  string twoFirst_;
  string twoLast_;
  bool personTwoFound_ = false;
  bool personOneFound_ = false;
  bool canMarry_ = true;
};


// demonstrating the operation
int main(){

  // setting up the genealogical tree      
  // the tree is as follows
  //    
  //
  //      James Smith  <--spouse-->   Mary 
  //                                  |
  //                                 children -------------------------
  //                                  |              |                |
  //                                  |              |                |
  //   William Johnson <--spouse-> Patricia      Robert Smith       Linda
  //                                  |
  //                                 children------------
  //                                  |                 |
  //                                  |                 |
  //   Jennifer  <--spouse-->  Michael Johnson      Barbara
  //       |
  //     children
  //       |
  //       |
  //     Susan


  // first generation
  Man   *js = new Man("Smith", "James", nullptr, nullptr, nullptr);
  Woman *ms = new Woman({}, "Mary", nullptr, nullptr, nullptr);
  ms->setSpouse(js); 
  js->setSpouse(ms);

  // second generation
  Woman *ps = new Woman({}, "Patricia", nullptr, js, ms);
  Man *wj = new Man("Johnson", "William", nullptr, nullptr, nullptr);
  ps->setSpouse(wj); 
  wj->setSpouse(ps);

  vector<Person *> marysKids  = {ps, new Man("Smith", "Robert", nullptr, js, ms),
				     new Woman({}, "Linda", nullptr, js, ms)};
  ms->setChildren(marysKids);

  // third generation
  Man *mj = new Man("Johnson", "Michael", nullptr, wj, ps);
  vector<Person *> patsKids   = {mj, new Woman({}, "Barbara", nullptr, wj, ps)}; 
  ps->setChildren(patsKids);

  Woman *jj = new Woman({}, "Jennifer", nullptr, nullptr, nullptr);
  vector<Person *> jensKids = {new Woman({}, "Susan", nullptr, mj ,jj)};

  jj->setSpouse(mj); mj->setSpouse(jj);
  jj->setChildren(jensKids);

  /*
  // defining two visitors
  ChildrenPrinter *cp = new ChildrenPrinter;
  NamePrinter *np = new NamePrinter;
  cout << "\nNAME LIST\n";
  ms->accept(np);
  cout << endl << endl;
  // executing the traversal with the composite
  // and the specific visitor
  cout << "CHILDREN LIST\n";
  ms->accept(cp);
  */

  cout << "MAIDEN NAME LIST" << endl;
  MaidenPrinter *mPrinter = new MaidenPrinter;
  ms->accept(mPrinter);

  cout << "Marriage Advice" << endl;
  cout << "Enter first candidates first name: ";
  string oneFirst; 
  cin >> oneFirst;
  cout << "Enter first candidates last name: ";
  string oneLast; 
  cin >> oneLast;

  cout << "Enter second candidates first name: ";
  string twoFirst; 
  cin >> twoFirst;
  cout << "Enter second candidates last name: ";
  string twoLast; 
  cin >> twoLast;

  MarriageAdvisor *maVisitor = new MarriageAdvisor
    (oneFirst, oneLast, twoFirst, twoLast);
    
  ms->accept(maVisitor);
  
  if (maVisitor->getCanMarry())
    cout << endl << "They can marry!" << endl;
  else
    cout << endl << "They cannot marry." << endl;
}
