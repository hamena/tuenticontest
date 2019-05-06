#ifndef ACTION_H_
#define ACTION_H_

#include <iostream>
#include <string>
#include "tape.h"

using std::string;

#define RIGHT_DIRECTION "right"
#define LEFT_DIRECTION "left"

class Action{
public:
  virtual string execute(Tape& tape) = 0;
};

class WriteAction : public Action{
public:
  WriteAction(char s, Action* a) : Action(),
    symbol(s), action(a) {}

  string execute(Tape& tape){
    std::cout << "WRITE " << symbol << std::endl;
    tape.write(symbol);
    return action->execute(tape);
  }
private:
  char symbol;
  Action* action;
};

class MoveAction : public Action{
public:
  MoveAction(string d, Action* a) : Action(),
    direction(d), action(a) {}

  string execute(Tape& tape){
    std::cout << "MOVE " << direction << std::endl;
    if (direction == RIGHT_DIRECTION)
      tape.increment_pointer();
    else
      tape.decrement_pointer();
    
    return action->execute(tape);
  }
  
private:
  string direction;
  Action* action;
};

class StateAction : public Action{
public:
  StateAction(const string& id) :
    Action(), idState(id) {}

  string execute(Tape& tape){
    std::cout << "STATE " << idState << std::endl;
    return idState;
  }
private:
  string idState;
};

class NoAction : public Action{
public:
  NoAction() : Action() {}
  string execute(Tape& tape){} // Null behavior
};

#endif // ACTION_H_
