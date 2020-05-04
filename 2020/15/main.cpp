#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc >= 2) {

	ifstream ifs(argv[1]);
	string animal = "none";
	while () {
	    size_t nadditions;
	    ifs >> animal;
	    ifs >> nadditions;
	    cout << animal << ": " << endl;
	    while (nadditions --> 0) {
		size_t pos, bval;
		ifs >> pos >> bval;
		cout << "\t" << pos << " " << bval << endl;
	    }
	}
	
	ifs.close();
	
    } else {
	return 1;
    }
    return 0;
}
