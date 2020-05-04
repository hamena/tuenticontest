#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

using namespace std;

string read(const string filepath) {
    ifstream ifs(filepath);
    string text; 
    string line;
    while (!ifs.eof()) {
	getline(ifs, line); 
	text += line;
    }
    ifs.close();
    return text;
}

int main(int argc, char **argv) {

    string p1 = read("plaintexts/test1.txt");
    string p2 = read("plaintexts/test2.txt");
    string c1 = read("ciphered/test1.txt");
    string c2 = read("ciphered/test2.txt");

    unsigned e = 3;
    
    cout << p1 << endl;
    cout << p2 << endl;
    cout << c1 << endl;
    cout << c2 << endl;
    
    return 0;
}

			       
