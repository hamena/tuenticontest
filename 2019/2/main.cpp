#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <utility>

using namespace std;

typedef string planet;
typedef map<planet, vector<planet> > nearbyplanets;

int how_many_possible_ways(const nearbyplanets&, const planet&);

void readPaths(nearbyplanets&, ifstream&, int);
void split(vector<string>&, const string&, char);

int main(int argc, char **argv){

    if (argc >= 2) {
	ifstream ifs(argv[1]);
	string line;

	getline(ifs, line);
	int n_cases = stoi(line);
	int i_case = 1;
	
	while (n_cases --> 0){
	    getline(ifs, line);
	    int n_planets = stoi(line);
	    nearbyplanets paths;
	    readPaths(paths, ifs, n_planets);

	    int n_ways = how_many_possible_ways(paths, planet("Galactica"));
	    cout << "Case #" << i_case++ << ": " << n_ways << endl;
	}
    }
    else {
	cerr << "You must provide an input file as argument." << endl;
	exit(1);
    }

    exit(0);
 
}

int how_many_possible_ways(const nearbyplanets& paths, const planet& current_planet){
    if (current_planet == "New Earth"){
	return 1;
    }
    else {
	vector<planet> nearby = paths.find(current_planet)->second;
	int temp = 0;
	for (int i=0; i<nearby.size(); ++i){
	    temp += how_many_possible_ways(paths, nearby[i]);
	}
	return temp;
    }
}

void readPaths(nearbyplanets& paths, ifstream& ifs, int size){
    string line;
    while (size --> 0){ // for each planet
	getline(ifs, line);
	    
	int i_colon = line.find(':');
	planet origin = line.substr(0, i_colon);
	vector<planet> planet_list;
	split(planet_list, line.substr(i_colon+1, line.length()), ',');
	
	paths.insert(make_pair(origin, planet_list));
    }
}

void split(vector<string>& tokens, const string& str, char delimiter){
    string token;
    istringstream iss(str);

    while (getline(iss, token, delimiter)){
	tokens.push_back(token);
    }
}
