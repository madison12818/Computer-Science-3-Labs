// figures: class adapter pattern
// Madison Dockery
// 4/7/2020

#include <iostream>

using std::cout; using std::endl;

// base interface
class Figure {
public:
  virtual void draw()=0;
  virtual ~Figure(){}
};

// adaptee/implementer
class LegacyRectangle{
public:
  LegacyRectangle(int topLeftX, 
		  int topLeftY, 
		  int bottomRightX, 
		  int bottomRightY): 
    topLeftX_(topLeftX), 
    topLeftY_(topLeftY), 
    bottomRightX_(bottomRightX), 
    bottomRightY_(bottomRightY){}

  void oldDraw(){
    for(int i=0; i < bottomRightY_; ++i){
      for(int j=0; j < bottomRightX_; ++j)
	if(i >= topLeftY_  && j >= topLeftX_ )
	  cout << '*';
	else
	  cout << ' ';
      cout << endl;
    }
  }

  void move(int topLeftX, 
	    int topLeftY, 
	    int bottomRightX, 
	    int bottomRightY){
    topLeftX_ = topLeftX;
    topLeftY_ = topLeftY;
    bottomRightX_ = bottomRightX;
    bottomRightY_ = bottomRightY;
  }

  int getBottomRightX() const { return bottomRightX_; }

private: // defining top/left and bottom/right coordinates 
  int topLeftX_;
  int topLeftY_;
  int bottomRightX_;
  int bottomRightY_;
};

// adapter uses multiple inheritance to inherit
// interface and implementation
class SquareAdapter: public Figure, 
		     private LegacyRectangle {
public:
  SquareAdapter(int size): LegacyRectangle(0,0,size,size){};
  
  void draw() override {
    oldDraw();
  }

  void resize(int newSize) { 
    this->move(0, 0, newSize, newSize); 
  }
  
  int size() const { 
    return getBottomRightX(); 
  }

};


int main(){

  // get input from user
  cout << "What will the size of square be? ";
  int sizeOfSquare;
  std::cin >> sizeOfSquare;

  cout << endl;

  // check with output that square is right size
  SquareAdapter square(sizeOfSquare);
  cout << "Size is " << square.size() << endl;
  // draw the square
  square.draw();

  // resize square -- get input from user
  cout << "New size of square? ";
  int newSize;
  std::cin >> newSize;
  
  cout << endl;

  // resizing square
  square.resize(newSize);
  cout << "Size is " << square.size() << endl;
  // print resized square
  square.draw();

}
