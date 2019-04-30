#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <math.h>

using namespace std;

template <typename T>
bool exists(const vector<T>&, const T&);
vector<int> read_row(ifstream&);

long lcm(long, long);
long gcd(long, long);
void reduce_fraction(long&, long&);

vector<int> select_lcm_candidates(const vector<int>&);
void calculate_fraction(const vector<int>&, long&, long&);

int main(int argc, char **argv){

    if (argc < 2){
	cerr << "You must provide an input file as argument." << endl;
	exit(1);
    }

    ifstream ifs(argv[1]);
    int n_cases;
    ifs >> n_cases;
    int i_case = 1;

    while (n_cases --> 0) {	
	vector<int> row = read_row(ifs);

	long n=0, d=0;
        calculate_fraction(row, n, d);
	reduce_fraction(n, d);

	cout << "Case #" << i_case++ << ": " << n << "/" << d << endl;
    }

    ifs.close();
    exit(0);
}

void calculate_fraction(const vector<int>& row, long& n, long& d){
    // X is the least common multiple of every number in row
    vector<int> lcm_row = select_lcm_candidates(row);
    long x = accumulate(lcm_row.begin(), lcm_row.end(), 1, lcm);

    // pieces of candy
    n = x*row.size();

    // attendees
    for (int i=0; i<row.size(); ++i){
	d += x / row[i];
    }
}

// Reduce the lcm candidates: greater than 1 and not repeated values
vector<int> select_lcm_candidates(const vector<int>& row){
    vector<int> lcm_row(0);
    for (int i=0; i<row.size(); ++i){
	int value = row[i];
	if (value > 1 && !exists(lcm_row, value)){
	    lcm_row.push_back(value);
	}
    }
    if (!lcm_row.size()){
	lcm_row = row; // copy
    }
    return lcm_row;
}

void reduce_fraction (long &n, long &d){
    long t = gcd(n, d);
    n /= t;
    d /= t;
}

long lcm(long a, long b) {
    long lcm = 0;
    long ta = max(a, b);
    long tb = min(a, b);
    lcm = (ta/gcd(ta,tb))*tb;
    return lcm;
}

long gcd(long a, long b) {
    long gcd = 0;
    long ta = max(a, b);
    long tb = min(a, b);
    do{
        gcd = tb;
        tb = ta%tb;
        ta = gcd;
    }while (tb!=0);
    return gcd;
}

template <typename T>
bool exists(const vector<T>& v, const T& value){
    return find(v.begin(), v.end(), value) != v.end();
}

vector<int> read_row(ifstream& ifs){
    int row_length;
    ifs >> row_length;
    vector<int> row(row_length);
    for (int i=0; i<row_length; ++i){
	ifs >> row[i];
    }
    return row;
}
