// Games, Template Method example
// Madison Dockery
// 3/10/2020

#include <ctime>
#include <cstdlib>
#include <iostream>

using std::cout; using std::endl; using std::cin;

// template for any game where players take 
// turns to make moves
// and there is a winner
class Game{
public:
  Game():playersCount_(0), movesCount_(0), 
	 playerWon_(noWinner){}

  // template method
  void playGame(const int playersCount = 0) {
    playersCount_ = playersCount;
    movesCount_=0;

    initializeGame();

    for(int i=0; !endOfGame(); i = (i+1) % playersCount_ ){
      makeMove(i);
      if (i==playersCount_-1) 
	++movesCount_; 
    }

    printWinner();
  }

  virtual ~Game(){}

protected:
  // primitive operations
  virtual void initializeGame() = 0;
  virtual void makeMove(int player) = 0;
  virtual void printWinner() = 0;
  virtual bool endOfGame() { return playerWon_ != noWinner;} // this is a hook
                           // returns true if winner is decided
  static const int noWinner=-1;

  int playersCount_;
  int movesCount_;
  int playerWon_;
};

// Monopoly - a concrete game implementing primitive 
// operations for the template method
class Monopoly: public Game {
public:  
  // implementing concrete methods
  void initializeGame(){
    playersCount_ = rand() % numPlayers_ + 1 ; // initialize players
  }

  void makeMove(int player) {
    if (movesCount_ > minMoves_){ 
      const int chance = minMoves_ + rand() % (maxMoves_ - minMoves_);
      if (chance < movesCount_) playerWon_= player;
    }
  }

  void printWinner(){
    cout << "Monopoly, player "<< playerWon_<< " won in "
         << movesCount_<< " moves." << endl;
  }

private:
  static const int numPlayers_ = 8; // max number of players
  static const int minMoves_  = 20; // nobody wins before minMoves_
  static const int maxMoves_ = 200; // somebody wins before maxMoves_
};

// Chess - another game implementing
// primitive operations
class Chess: public Game {
public:
  void initializeGame(){
    playersCount_ = numPlayers_; // initalize players
    for(int i=0; i < numPlayers_; ++i) 
      experience_[i] = rand() % maxExperience_ + 1 ; 
  }

  void makeMove(int player){
    if (movesCount_ > minMoves_){
      const int chance = (rand() % maxMoves_) / experience_[player];
      if (chance < movesCount_) playerWon_= player;
    }
  }

  void printWinner(){
    cout << "Chess, player " << playerWon_ 
         << " with experience " << experience_[playerWon_]
         << " won in "<< movesCount_ << " moves over"
         << " player with experience " << experience_[playerWon_== 0 ? 1:0] 
	 << endl;
  }

private:
  static const int numPlayers_ = 2;
  static const int minMoves_ = 2;      // nobody wins before minMoves_
  static const int maxMoves_ = 100;    // somebody wins before maxMoves_
  static const int maxExperience_ = 3; // player's experience
                     // the higher, the greater probability of winning
  int experience_[numPlayers_]; 
};

// Dice - another game implementing
// rolling dice and adding them up, high score wins
class Dice: public Game {
public:
  void initializeGame() override final{
    playersCount_ = numPlayers_;
    for (bool e: pass_) e = false;
  }
  
  void makeMove(int) override final;
  
  void printWinner() override final{
    if(playerWon_ == 0) 
      cout << "You lost Dice" << endl;
    else
      cout << "You won Dice" << endl;
  }

private:
  static const int numPlayers_ = 2; // let com = 0 and user = 1
  static const int maxRounds_ = 3;  // up to 3 rounds 
  static const int numRolls_ = 5;   // can roll up to 5 times per turn
  
  bool pass_[numPlayers_];     // stores if player passed or not
  int highScore_[numPlayers_]; // stores the high scores
};

void Dice::makeMove(int player){
  // check total moves
  if(movesCount_ == maxRounds_ || (pass_[0] && pass_[1])){
    playerWon_ = highScore_[0] >= highScore_[1] ? 0 : 1;
    return;
  }

  // print who's turn it is
  if(player == 0) cout << "computer's turn" << endl;
  if(player == 1) cout << "user's turn" << endl;
  
  if(!pass_[player]){ // if player hasn't passed then its their turn
    cout << "rolled: ";
    
    // roll the dice and add up the total
    int rolls[numRolls_];
    int total = 0;
    for(int e: rolls){
      e = rand() % 6 + 1;
      total += e;
      cout << e << ' ';
    }
    cout << " = " << total << ", " << endl;
    
    // check if it is a high score
    if(highScore_[player] < total)
      highScore_[player] = total;
  }
  else // this means player has passed
    cout << "the player has passed, " << endl;

  // print the highest score
  if(player == 0)
    cout << "computer's ";
  else 
    cout << "user's ";
  
  cout << "highest score is " << highScore_[player] << endl;
  
  // check if current player want to roll again or if it's the last round
  if(!pass_[player] && player == 1 && movesCount_ < maxRounds_ - 1){
    cout << "Roll again? y or n" << endl;
    char answer;
    cin >> answer;
    if(answer == 'n' || answer == 'N') 
      pass_[player] = true;    
  }

  // check if computer wants to roll again
  else if(!pass_[player] && player == 0){
    int choice = rand() % 2; 
    
    // 0 means the computer passes
    if(choice == 0)
      pass_[player] = true;
  }
 
}

int main() {
  srand(time(nullptr));

  Game* gp = nullptr;

  // play chess 10 times
  for (int i = 0; i < 10; ++i){ 
    gp = new Chess;
    gp->playGame(); 
    delete gp;
  }
      

  // play monopoly 5 times
  for (int i = 0; i < 5; ++i){
    gp = new Monopoly;
    gp->playGame(); 
    delete gp;
  }

  // play Dice 1 time
  gp = new Dice;
  gp->playGame();
  delete gp;

}
