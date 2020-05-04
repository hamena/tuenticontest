#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    if (argc >= 2) {

	ifstream ifs(argv[1]);
	
	ifs.close();
	
    } else {
	return 1;
    }
    return 0;
}
