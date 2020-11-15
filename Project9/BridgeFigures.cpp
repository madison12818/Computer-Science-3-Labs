// filled/hollow figures demonstrates Bridge Design Pattern,
// square is either hollow or square and is painted with a particular character,
// it is bridged over Figure --> Fill abstract body/handle
// Madison Dockery
// 3/31/2020

#include <iostream>

using std::cout; using std::endl; using std::cin;

// abstract body
class Fill{
public:
  Fill(char border, char internal):borderChar_(border), internalChar_(internal){}
  virtual char getBorder() { return borderChar_; } //=0;
  virtual char getInternal() { return internalChar_; } //=0;
  virtual ~Fill() {}
protected:
  char borderChar_;
  char internalChar_;
};

// concrete body
class Hollow: public Fill{
public:
  Hollow(char border):Fill(border, ' '){}
  //char getBorder() override {return fillChar_;}
  //char getInternal() override {return ' ';}
  ~Hollow(){}
};



// another concrete body
class Filled: public Fill {
public:
  Filled(char fillChar):Fill(fillChar, fillChar){}
  //char getBorder() override {return fillChar_;}
  //char getInternal() override {return fillChar_;}
  ~Filled(){}
};

// last concrete body
class EnhancedFilled: public Fill {
  EnhancedFilled(char border, char internal) : Fill(border, internal) {};
  //char getBorder() override { return borderChar_; }
  //char getInternal() override { return internalChar_; }
  ~EnhancedFilled(){}
};

// abstract handle
class Figure {
public:
  Figure(int size, Fill* fill): size_(size), 
				fill_(fill){}
  void changeFill(const Fill* fill) { fill_ = const_cast<Fill *>(fill); }
  virtual void draw() =0;
  virtual ~Figure(){}
protected:
  int size_;
  Fill *fill_;
};

// concrete handle
class Square: public Figure{
public:
  Square(int size, Fill* fill): Figure(size, fill){}
  void draw() override;
};


void Square::draw(){
  for(int i=0; i < size_; ++i){
    for(int j=0; j < size_; ++j)
      if(i==0 || j==0 || i==size_-1 || j==size_-1 )
	cout << fill_ -> getBorder();
      else
	cout << fill_ -> getInternal();
    cout << endl;
  }
}


int main(){
  
   Fill* hollowPaintY = new Hollow('Y');
   Fill* filledPaintStar = new Filled('*');

   
   Figure *smallBox = new Square(4, filledPaintStar);
   Figure *bigBox = new Square(10, hollowPaintY);

   smallBox->draw();
   cout << endl;
   bigBox -> draw();
   cout << endl;
  
  // ask user for figure parameters
  cout << "Enter fill character: "; 
  char fchar; cin >> fchar;
  cout << "Filled or hollow? [f/h] "; 
  char ifFilled; cin >> ifFilled;
  cout << "Enter size: "; int size; cin >> size;
  
 
  Figure *userBox = new Square(size, ifFilled == 'f'? 
			       static_cast<Fill *>(new Filled(fchar)):
			       static_cast<Fill *>(new Hollow(fchar))
			       ); 

   userBox = ifFilled == 'f' ?
      new Square(size, new Filled(fchar)):
      new Square(size, new Hollow(fchar));

  userBox -> draw();
  cout << endl;

}

