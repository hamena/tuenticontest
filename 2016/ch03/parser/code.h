#ifndef CODE_H_
#define CODE_H_

#include <iostream>
#include <vector>
#include "tape.h"
#include "state.h"

#define ID_END_STATE "end"

typedef std::vector<State*> States;

class Code{
public:
  Code(State* ss, States* s) :
    startState(ss), states(s) {}

  void execute(Tape& tape){
    string IDnextState = startState->execute(tape);

    while (IDnextState != ID_END_STATE){
      int i = find_state(*states,IDnextState);
      if (i > 0)
	IDnextState = (*states)[i]->execute(tape);
      else{
	std::cerr << "Execute fail." << std::endl;
	IDnextState = ID_END_STATE;
      }
    }
  }
private:
  State* startState;
  States* states;

  int find_state(const States& v, const string& e) const{
    unsigned length = v.size();
    for (unsigned i=0; i<length; ++i){
      if (v[i]->get_id() == e)
	return i;
    }

    return -1;
  }
};

#endif // CODE_H_
