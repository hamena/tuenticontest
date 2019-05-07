#include <iostream>
#include <fstream>
#include <cmath>

using std::cout;
using std::endl;
using std::ifstream;
using std::ceil;
using std::sqrt;

void card_counter(const char*);
unsigned nesim_fibonacci(unsigned);

int main(int argc, char** argv){

  if (argc > 1)
    card_counter(argv[1]);
  
  return 0;
}

void card_counter(const char* inputpath){
  ifstream input(inputpath);
  if (input.is_open()){
    size_t listsize;
    input >> listsize;
    
    for (unsigned icase=1; icase<=listsize; ++icase){
      unsigned maxscore;
      input >> maxscore;
      cout << "Case #" << maxscore << ": " << nesim_fibonacci(maxscore) << endl;
    }
  }
}

unsigned nesim_fibonacci(unsigned target){
  unsigned a=0,b=1,counter=0,acum=0,temp;
  while (acum < target){
    ++counter;
    temp = b;
    b += a;
    a = temp;
    acum += b;
    cout << b << " " << acum << endl;
  }
  //  cout << "-> " << acum << endl;
  return counter;
}
