#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "tape.h"
#include "code.h"

#define FILE_OUTPUT "testOutput.txt"

using std::endl;
using std::ofstream;
using std::string;
using std::vector;

typedef vector<Tape*> Tapes;

class Test{
public:
  Test(Code* c, Tapes* t) :
    code(c), tapes(t) {}
  void execute(){
    ofstream fileOutput(FILE_OUTPUT);
    for (auto& it : *tapes){
      code->execute(*it);
      fileOutput << "Tape #" << it->get_id() << ": '" << it->get_tape() << "'" << endl;
    }
    fileOutput.close();
  }
private:
  Code* code;
  Tapes* tapes;
};

#endif // TEST_H_
