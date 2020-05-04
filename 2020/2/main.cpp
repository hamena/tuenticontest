#include <cstddef>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>
#include <utility>

using namespace std;

int main(int argc, char **argv) {
    if (argc >= 2) {

	ifstream ifs(argv[1]);

	size_t n_cases, i_case = 1;
	ifs >> n_cases;

	while (n_cases --> 0) {
	    size_t n_matches;
	    ifs >> n_matches;

	    map<size_t, int> best;
	    while (n_matches --> 0) {
		size_t p1, p2, strongest;
		ifs >> p1 >> p2 >> strongest;
		if (best.find(p1) == best.end()) {
		    best.insert(make_pair(p1, 0));
		}
		if (best.find(p2) == best.end()) {
		    best.insert(make_pair(p2, 0));
		}

		if (strongest > 0) {
		    ++best[p1];
		    --best[p2];
		} else {
		    ++best[p2];
		    --best[p1];
		}
	    }

	    auto bestPlayer = 
	    max_element(best.begin(),
			best.end(),
			[] (const pair<size_t, int>& p1, const pair<size_t, int>& p2){
			    return p1.second < p2.second;
			});
	    
	    
	    cout << "Case #" << i_case++ << ": " << bestPlayer->first << endl;
	}
    } else {
	return 1;
    }
    return 0;
}
