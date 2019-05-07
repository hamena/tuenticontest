#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <vector>
#include <string>
#include "tape.h"
#include "symbol.h"

#define START_STATE "start"
#define NULL_STATE "null_state"

using std::string;

typedef std::vector<Symbol*> Symbols;

class State{
public:
  State(const string& id, Symbols* s) :
    ID(id), symbols(s) {}

  virtual string execute(Tape& tape){
    string IDnextState = NO_STATE;
    unsigned length = symbols->size();

    while (IDnextState == NO_STATE){
      char e = tape.read();
      for (unsigned i=0; i<length; ++i){
	if ((*symbols)[i]->get_symbol() == e){
	  IDnextState = (*symbols)[i]->execute(tape);
	  i = length;
	}
      }
    }

    return IDnextState;
  }

  const string& get_id() const{ return ID; }
private:
  string ID;
  Symbols* symbols;
};

class StartState : public State{
public:
  StartState(char s, Action* a) :
    State(START_STATE, new Symbols(1,new Symbol(s,a))) {}
};

class NullState : public State{
public:
  NullState() : State(NULL_STATE, new Symbols(1, new NullSymbol())) {}
  string execute(Tape& tape) {}  // Null behavior
};

#endif // STATE_H_
