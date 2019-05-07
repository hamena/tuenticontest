#include <iostream>
#include <fstream>
#include <string>

#define FILE_INPUT "testInput.txt"
#define FILE_OUTPUT "testOutput.txt"

#define OPEN "---"
#define CLOSE "..."

using std::ifstream;
using std::ofstream;
using std::string;
using std::cerr;
using std::endl;

int main(){
  ifstream fileInput(FILE_INPUT);
  ofstream fileOutput(FILE_OUTPUT);

  string token;
  fileInput >> token;

  if (token == OPEN){
    Test test;
    token = test.init(fileInput);

    if (token == CLOSE)
      test.execute();
    else
      cerr << "Close '...' sentence expected." << endl;
  }
    
  fileInput.close();
  fileOutput.close();
  
  return 0;
}
