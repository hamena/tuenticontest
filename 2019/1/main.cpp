#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

int how_many_tortillas(int, int);

int main(int argc, char **argv){

    if (argc >= 2) {
	ifstream ifs(argv[1]);

	int lines = 0;
	ifs >> lines;

	int n_case = 1;
	int onion, no_onion;
	while (lines --> 0){
	    ifs >> onion >> no_onion;
	    cout << "Case #" << n_case++ << ": " << how_many_tortillas(onion, no_onion) << endl;
	}
	
	ifs.close();
    }
    else {
	exit(1);
    }
    
    exit(0);
}

int how_many_tortillas(int onion, int no_onion) {
    return (int)ceil((float)onion / 2) + (int)ceil((float)no_onion / 2);
}
