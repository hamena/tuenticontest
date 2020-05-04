#include <fstream>
#include <iostream>
#include <map>

using namespace std;

map<char, char> dvorak_to_qwerty =
    { {'{','_'},{'}','-'},{'[','+'},{']','='},{':','Z'},{';','z'},
      {'?','{'},{'+','}'},{'/','['},{'=',']'},{'_','"'},{'-','\''},
      {'"','Q'},{'<','W'},{'>','E'},{'\'','q'},{',','w'},{'.','e'},
      {'p','r'},{'y','t'},{'f','y'},{'g','u'},{'c','i'},{'r','o'},{'l','p'},
      {'a','a'},{'o','s'},{'e','d'},{'u','f'},{'i','g'},{'d','h'},
      {'h','j'},{'t','k'},{'n','l'},{'S',':'},{'s',';'},
      {'q','x'},{'j','c'},{'k','v'},{'x','b'},{'b','n'},{'m','m'},
      {'W','<'},{'V','>'},{'Z','?'},{'w',','},{'v','.'},{'z','/'}
    };
string translate (const string& str) {
    string result("");

    for (char c : str) {
	const auto& it = dvorak_to_qwerty.find(c);
	if (it != dvorak_to_qwerty.end())
	    result += it->second;
	else
	    result += c;
    }
    
    return result;
}

int main(int argc, char **argv) {
    if (argc >= 2) {
	ifstream ifs(argv[1]);
	
	string line;
	size_t n_cases, i_case=1;
	getline(ifs, line);
	n_cases = stoi(line);

	while (n_cases --> 0) {
	    getline(ifs, line);
	    cout << "Case #" << i_case++ << ": " << translate(line) << endl;
	}
	
	ifs.close();
	
    } else {
	return 1;
    }
    return 0;
}
