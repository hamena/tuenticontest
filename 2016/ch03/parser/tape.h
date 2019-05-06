#ifndef TAPE_H_
#define TAPE_H_

#include <string>
#include <cstring>

#define NULL_TAPE "NULL_TAPE"

using std::string;

class Tape{
public:
  Tape(string id, string t) :
    ID(id), tape(new char[strlen(t.c_str())]), pointer(1)
  {
    strcpy(tape,t.c_str());
  }

  const string& get_id() const{ return ID; }
  const char* get_tape() const{ return tape; }

  virtual char read() const{ return tape[pointer]; }
  virtual void write(char c){ tape[pointer] = c; }
  virtual void increment_pointer(){ ++pointer; }
  virtual void decrement_pointer(){ --pointer; }
  
private:
  string ID;
  char* tape;
  int pointer;
};

class NullTape : public Tape{
public:
  NullTape() : Tape(NULL_TAPE, NULL_TAPE) {}

  char read() const{ return 'N'; }
  void write(char c) {}
  void increment_pointer(){}
  void decrement_pointer(){}
};

#endif // TAPE_H_
