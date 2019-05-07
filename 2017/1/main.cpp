#include <iostream>
#include <fstream>
#include <cmath>

using std::cout;
using std::endl;
using std::ifstream;

void pizza(const char*);

int main(int argc, char** argv){

  if (argc > 1)
    pizza(argv[1]);
  
  return 0;
}

void pizza(const char* input_path){
  ifstream input(input_path);
  if (input.is_open()){
    size_t listsize;
    input >> listsize;

    for (unsigned i=1; i<=listsize; ++i){
      size_t nguests;
      input >> nguests;

      unsigned totalslices = 0;
      for (unsigned j=0; j<nguests; ++j){
	unsigned temp;
	input >> temp;
	totalslices += temp;
      }

      cout << "Case #" << i << ": " << std::ceil((float)totalslices/8) << endl;
    }
    
    input.close();
  }
}
