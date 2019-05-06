#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;

enum Roll {first, second};

struct Turn{
  unsigned turn_number;
  Roll roll;
  
  Turn() : turn_number(1), roll(first){}
};

void bowling(const char*);
unsigned evaluate_turn(Turn&, unsigned);

int main(int argc, char **argv){

  if (argc > 1)
    bowling(argv[1]);
  
  return 0;
}

void bowling(const char* input_path){
  ifstream input(input_path);

  if (input.is_open()){
    size_t listsize;
    input >> listsize;

    for (unsigned i=1; i<=listsize; ++i){
      size_t nrolls;
      input >> nrolls;

      Turn turn;
      unsigned score, totalscore=0;
      cout << "Case #" << i << ":";
      for (unsigned roll=1; roll<=nrolls; ++roll){
	input >> score;
	totalscore += evaluate_turn(turn, score);
	if (turn.roll == first)
	  cout << " " << totalscore;
      }
      cout << endl;
    }
    
    input.close();
  }
}

unsigned evaluate_turn(Turn& turn, unsigned score){
  if (score < 0 || score > 10){
    cout << "debug: mal asunto" << endl;
    return 0;
  }

  if (score == 10){// Strike
    turn.roll = first;
    return (++turn.turn_number) <= 10 ? score : 0;
  } else {// Partial
    if (turn.turn_number > 10)
      return 0;
    
    if (turn.roll == second){
      turn.roll = first;
      ++turn.turn_number;
    } else
      turn.roll = second;

    return score;
  }
}
