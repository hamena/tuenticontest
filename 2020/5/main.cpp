#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc >= 2) {

	ifstream ifs(argv[1]);

	size_t n_cases, i_case=1;
        ifs >> n_cases;

	while (n_cases --> 0) {
	    long unsigned n;
	    ifs >> n;
	    long unsigned tuentistics = n/20;
	    size_t mod = n%20;
	    cout << "Case #" << i_case++ << ": ";
	    if (tuentistics*9 >= mod) {
		cout << tuentistics << endl;
	    } else {
		cout << "IMPOSSIBLE" << endl;
	    }
	}
	
    } else {
	return 1;
    }
    return 0;
}
