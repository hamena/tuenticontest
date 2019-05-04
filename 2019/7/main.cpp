#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <cmath>

#define MAX 16

using namespace std;

template <typename T>
void initialize(T[], int, T);
string build_string(ifstream&);
void no_so_complex_hash(char[MAX], const string&);
void to_int(int[], char[]);
int total_changes(const int *);

void determine_n_changes_and_gaps(int[], int[], int[], const string&, const string&, int);
string insert_zeros(int, const string&, const string&);
bool is_viable(int[], const int*, int[], int[], int); 
bool fits(int&, int, int, int, int);

void determine_characters(vector<vector<char> >&, const int*, const int*);
void to_single_array(char[], vector<vector<char> >&, int);

int main (int argc, char **argv){

    if (argc < 2){
	cerr << "You must provide an input file as argument." << endl;
	exit(1);
    }
    
    ifstream ifs(argv[1]);
    string line;
    getline(ifs, line);
    int n_cases = stoi(line);
    int i_case = 1;

    while (n_cases --> 0){
	string str_original = build_string(ifs);
	string str_modified = build_string(ifs);

	size_t i_insert = (str_modified.find("---")+3);
	string modified_preamble = str_modified.substr(0, i_insert);
	string modified_body = str_modified.substr(i_insert, str_modified.length());
    
	char hash_original[MAX];
	int ihash_original[MAX];
	no_so_complex_hash(hash_original, str_original);
	to_int(ihash_original, hash_original);

	int changes[MAX];
	int gaps[MAX];
	initialize(changes, MAX, 0);
	initialize(gaps, MAX, 0);

	// First determines how many characters is needed to be inserted in print zone
	// the original hash byte and payload hash byte gap are collected too.
	determine_n_changes_and_gaps(changes, gaps, ihash_original, modified_preamble, modified_body,
				     i_insert % MAX);

	// Minimizes alphabetically the characters to be inserted for hash collision
	vector<vector<char> > characters(MAX);
	determine_characters(characters, changes, gaps);

	// Simplifies the data struct to a simple array to be printed easily
	int n_changes = total_changes(changes);
	char to_insert[n_changes+1];
	to_single_array(to_insert, characters, n_changes);

	cout << "Case #" << i_case++ << ": " << to_insert << endl;
    }
	
    ifs.close();
    exit(0);
}

void determine_n_changes_and_gaps(int changes[], int gaps[], int ihash_original[], const string& preamble, const string& body, int i_hash){
    char hash_payload[MAX];
    int ihash_payload[MAX];
    int i_changes = 0;
    bool stop = false;
    while (!stop){
	string str_payload = insert_zeros(total_changes(changes), preamble, body);
	no_so_complex_hash(hash_payload, str_payload);
	to_int(ihash_payload, hash_payload);

	stop = is_viable(gaps, changes, ihash_original, ihash_payload, i_hash);

	if (!stop){
	    ++changes[i_changes++ % MAX];
	}
    }
}

string insert_zeros(int n_zeros, const string& preamble, const string& body){
    stringstream ss;
    ss << preamble;
    for (int i=0; i<n_zeros; ++i){
	ss << (char)0;
    }
    ss << body;
    return ss.str();
}

bool is_viable(int gaps[], const int *changes, int hash_a[], int hash_b[], int i_hash){
    for (int i=0; i<MAX; ++i){
	if (!fits(gaps[i], 48*changes[i], 122*changes[i], hash_a[i_hash], hash_b[i_hash])){
	    return false;
	}
	i_hash = ++i_hash % MAX;
    }
    return true;
}

bool fits(int& gap, int min, int max, int hash_a, int hash_b){
    int diff = hash_a < hash_b ? 256 - hash_b + hash_a : hash_a - hash_b;
    if (min==0 && diff==0){
	return true;
    }
    else {
	while (diff <= max){
	    if (diff>=min && diff<=max){
		gap = diff;
		return true;
	    }
	    else{
		diff += 256;
	    }
	}
    }
    // not reachable
    return false;
}

void determine_characters(vector<vector<char> >& characters, const int *changes, const int *gaps){
    for (int i=0; i<MAX; ++i){
	int gap = gaps[i];
	int n_changes = changes[i];

	for (int i_change = n_changes-1; i_change >= 0; --i_change){
	    int t = gap - 48*i_change;
	    if (t > 122){
		characters[i].push_back(122);
		gap -= 122;
	    } else {
		characters[i].push_back(t);
		gap -= t;
	    }
	}
    }
}

void to_single_array(char to_insert[], vector<vector<char> >& characters, int n_changes){
    for (int i=0; i<n_changes; ++i){
	to_insert[i] = characters[i%16].back();
	characters[i%16].pop_back();
    }
    to_insert[n_changes] = '\0';
}

void no_so_complex_hash(char hash[MAX], const string& input_text) {
    for (int i=0; i<MAX; ++i){
	hash[i] = 0;
    }
    for (int i=0; i<input_text.length(); i++) {
        hash[i%MAX] = hash[i%MAX] + input_text.at(i);
    }
}

void to_int(int hash_int[], char hash[]){
    for (int i=0; i<MAX; ++i){
	hash_int[i] = 128 + hash[i];
    }
}

template <typename T>
void initialize(T array[], int max, T value){
    for (int i=0; i<max; ++i){
	array[i] = value;
    }
}
		
int total_changes(const int *changes){
    int count = 0;
    for (int i=0; i<MAX; ++i){
	count += changes[i];
    }
    return count;
}

string build_string(ifstream& ifs){
    string line;
    getline(ifs, line);
    int n_lines = stoi(line);
    stringstream ss;
    while (n_lines --> 0){
	getline(ifs, line);
	ss << line;
    }
    return ss.str();
}
