#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <string>
#include <deque>
#include <map>
#include <algorithm>

using namespace std;

typedef deque<size_t> NumberList;
typedef pair<size_t, size_t> ID;
typedef map<ID, size_t> Memory;
 
size_t sum(const NumberList& list) {
    size_t acum = 0;
    for (auto n : list) {
	acum += n;
    }
    return acum;
}

NumberList create_whitelist(size_t N, const NumberList& blacklist) {
    NumberList whitelist;
    for (size_t i=1; i<N; ++i) {
	if (find(blacklist.begin(), blacklist.end(), i) == blacklist.end())
	    whitelist.push_back(i);
    }
    return whitelist;
}

size_t calculate(size_t N, const NumberList &whitelist);
size_t calculate_rec(size_t N, const NumberList& whitelist, NumberList& candidate,
		     Memory& mem);

int main(int argc, char **argv) {
    if (argc >= 2) {

	ifstream ifs(argv[1]);
	string line;

	size_t n_cases, i_case=1;
	getline(ifs, line);
	n_cases = stoi(line);

	while (n_cases --> 0) {
	    getline(ifs, line);
	    istringstream iss(line);
	    NumberList numbers;
	    copy(istream_iterator<size_t>(iss), istream_iterator<size_t>(),
		 back_inserter(numbers));

	    size_t N = numbers.front();
	    numbers.pop_front();

            NumberList whitelist = create_whitelist(N, numbers);
	    
	    size_t result = calculate(N, whitelist);
	    cout << "Case #" << i_case++ << ": " << result << endl;
	}

	ifs.close();
	
    } else {
	cerr << "Must provide an input file" << endl;
	return 1;
    }
    
    return 0;
}

size_t calculate(size_t N, const NumberList &whitelist) {
    size_t acum = 0;
    NumberList candidate;
    Memory mem;
    for (int i=whitelist.size()-1; i>=0; --i) {
	candidate.push_back(whitelist[i]);
	acum += calculate_rec(N, whitelist, candidate, mem);
	candidate.pop_back();
    }
    return acum;
}

size_t calculate_rec(size_t N, const NumberList& wl, NumberList& candidate,
		     Memory& mem) {
    size_t value = sum(candidate);
    if (value > N) {
	return 0;
    } else if (value == N) {
	return 1;
    } else {
	ID mark = make_pair(N-value, candidate.back());
	Memory::iterator it = mem.find(mark);
	if (it != mem.end()) {
	    return it->second;
	} else {
	    size_t acum = 0;
	    for_each(wl.begin(), wl.end(),
		     [N, &wl, &candidate, &mem, value, &acum] (size_t n) {
			 if (n <= candidate.back() && (value+n) <= N) {
			     candidate.push_back(n);
			     acum += calculate_rec(N, wl, candidate, mem);
			     candidate.pop_back();
			 }
		     });
            /*
            for (auto n : wl) {
                if (n <= candidate.back() && (value+n) <= N) {
                    candidate.push_back(n);
                    acum += calculate_rec(N, wl, candidate, mem);
                    candidate.pop_back();
                }
            }
	    */
	    mem.insert(make_pair(mark, acum));
	    return acum;
	}
    }
}
