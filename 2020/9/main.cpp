#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <string>

using namespace std;

string execute_encrypt(int counter, const string& key, const string& msg, int i) {
    stringstream sstream;
    sstream << "./encrypt.sh ";
    if (counter > 0) sstream << counter;
    sstream << key << " \"" << msg << "\" "
	    << i << " > output" << endl;
    cout << "Executing '" << sstream.str() << "'";
    system(sstream.str().c_str());

    string encrypted;
    ifstream fout("output");
    fout >> encrypted;
    fout.close();
    return encrypted;
}

string execute_encrypt(const string& key, const string& msg) {
    return execute_encrypt(-1, key, msg, -1);
}

int main(int argc, char **argv) {
    size_t f=0,l=2;
    string currentcoded="";
    string key= argc>=2 ? argv[1] : "";
    if (key.empty()) {
	string firstcoded="3633363A33353B393038383C363236333635313A353336";
	string firstmsg="514;248;980;347;145;332";
	for (size_t i=0; i<firstmsg.size(); ++i) {
	    string hexbyte = firstcoded.substr(f,l);
	    int counter=0;
	    bool match = false;
	    while (!match && counter < 10) {
		currentcoded = execute_encrypt(counter, key, firstcoded, i);
		cout << "currentcoded: " << currentcoded << endl;
		match = hexbyte==currentcoded.substr(f,l);
		++counter;
	    }

	    if (match) {
		cout << "match! " << hexbyte << " == " << currentcoded.substr(f,l) << endl;
		key = to_string(counter-1) + key;
		cout << "current key: " << key << endl;
	    }
	    
	    f += 2;
	}

	cout << firstcoded << endl << currentcoded << endl;
    }
    
    string scndcoded="3A3A333A333137393D39313C3C3634333431353A37363D";
    string scndmsg="000;000;000;000;000;000";
    f=0;
    l=2;
    
    currentcoded = "";
    for (size_t i=0; i<scndmsg.length(); ++i) {
	if (scndmsg[i] != ';') {
	    string hexbyte = scndcoded.substr(f,l);
	    int counter=0;
	    bool match = false;
	    while (!match && counter < 10) {
		scndmsg.replace(i,1,to_string(counter));
		currentcoded = execute_encrypt(key, scndmsg);
		match = hexbyte == currentcoded.substr(f,l);
		++counter;
	    }
	    cout << "current message: " << scndmsg << endl;
	}
	f += 2;
    }
    
    return 0;
}
