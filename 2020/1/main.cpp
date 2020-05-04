#include <fstream>
#include <iostream>

using namespace std;

int parse(char c);

int main(int argc, char **argv) {
    if (argc >= 2) {

	ifstream ifs(argv[1]);
	string line;

	getline(ifs, line);

	int n_cases = stoi(line);
	int i_case = 1;
	char first, second;

	char wintable[3][3] = {{'-', 'P', 'R'}, {'P', '-', 'S'}, {'R', 'S', '-'}};

	while (n_cases --> 0) {
	    ifs >> first;
	    ifs >> second;

	    cout << "Case #" << i_case++ << ": " 
		 << wintable[parse(first)][parse(second)] << endl;
	}
	
	ifs.close();
	
    } else {
	return 1;
    }
    return 0;
}

int parse(char c) {
    switch (c) {
    case 'R':
	return 0;
    case 'P':
	return 1;
    case 'S':
	return 2;
    default:
	return -1;
    }
}
