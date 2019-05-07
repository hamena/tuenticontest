#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <string>
#include "tape.h"
#include "action.h"

#define NO_STATE "NO_STATE"
#define NULL_SYMBOL ' '

using std::string;

class Symbol{
public:
  Symbol(char s, Action* a) :
    symbol(s), action(a) {}

  string execute(Tape& tape){
    if (action)
      return action->execute(tape);
    else{
      std::cerr << "Action doesn't exist." << std::endl;
      return NO_STATE;
    }
  }

  char get_symbol() const{ return symbol; }
private:
  char symbol;
  Action* action;
};

class NullSymbol : public Symbol{
public:
  NullSymbol() : Symbol(NULL_SYMBOL, new NoAction()) {}
  string execute(Tape& tape) {}
};

#endif // SYMBOL_H_
