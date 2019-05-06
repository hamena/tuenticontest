#include <iostream>
#include <fstream>
#include <cmath>

int main(){
  std::ifstream filetest("submitInput.txt");
  std::ofstream fileoutput("submitOutput.txt");

  int nCases = 0;
  filetest >> nCases;

  int diners = 0, solution = 0;
  for (int i=0; i<nCases; ++i){
    filetest >> diners;

    if (diners <= 4){
      if (diners > 0)
	solution = 1;
      else
	solution = 0;
    }
    else{
      solution = diners / 2;
      if (diners % 2 == 0)
	--solution;
    }
    
    fileoutput << "Case #" << i+1 << ": " << solution << std::endl;
  }

  filetest.close();
  fileoutput.close();
}
