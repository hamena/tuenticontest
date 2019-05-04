#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

typedef pair<char, char> CharPair;
typedef pair<string, string> StringPair;

typedef char Node;
typedef map<Node, set<Node> > Graph;

void build_graph(Graph&, const vector<string>&);
void solve(vector<Node>&, const set<char>&, const Graph&);
vector<Node> solve(const vector<Node>&, const Graph&, Node, Node);
Node get_initial_node(set<char>, const Graph&);
Node get_final_node(set<char>, const Graph&);

void get_all_chars(set<char>&, const vector<string>&);
void read_strings(ifstream&, vector<string>&);

void format_solution(stringstream&, vector<Node>);

int min(int a, int b){
    return a<b ? a : b;
}

int main(int argc, char **argv){

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
        vector<string> strings;
	read_strings(ifs, strings);

	set<char> chars;
	get_all_chars(chars, strings);

	stringstream result;

	// Need to solve graph
	if (chars.size() > 1){
	    Graph graph;
	    build_graph(graph, strings);

	    vector<Node> path;
	    solve(path, chars, graph);

	    format_solution(result, path);
	}
	// Direct solution, only 1 character
	else {
	    result << *(chars.begin());
	}
	
	cout << "Case #" << i_case++ << ": ";
	cout << result.str() << endl;
    }
    
    ifs.close();
    exit (0);
}

void format_solution(stringstream& ss, vector<Node> path){
    if (path.size() == 0){
	ss << "AMBIGUOUS";
    }
    else{
	ss << path.back();
	path.pop_back();
	while (path.size() > 0){
	    ss << " " << path.back();
	    path.pop_back();
	}
    }
}

// Gets the longest path from initial to final graph node.
void solve(vector<Node>& path, const set<char>& chars, const Graph& graph){
    Node initial = get_initial_node(chars, graph);
    Node final = get_final_node(chars, graph);

    // if initial or final node are not unique then result is AMBIGUOUS
    if (initial==0 || final==0){
	path = vector<Node>();
    }
    else{
        path = solve(vector<Node>(0), graph, final, initial);

	// if the path length is not equal to the total of characters then result is ambiguous
	if (path.size() != chars.size()){
	    path = vector<Node>();
	}
    }
}

// Solves recursively the longest path from a node to the final node
vector<Node> solve(const vector<Node>& path, const Graph& graph, Node final, Node current){
    if (current == final){
	vector<Node> best_path(path);
	best_path.push_back(current);
	return best_path;
    }
    else {
	const set<Node>& nodes = graph.find(current)->second;
	set<Node>::const_iterator it = nodes.begin();
	vector<Node> best_path;
	while (it != nodes.end()){
	    vector<Node> candidate = solve(path, graph, final, *it);
	    if (candidate.size() > best_path.size()){
		best_path = candidate;
	    }
	    ++it;
	}
	best_path.push_back(current);
	return best_path;
    }
}

// Determines which is the initial node: lesser character (char not existant in any set of nodes)
// Initial node must be unique. If not returns value 0 as 'error'.
Node get_initial_node(set<char> chars, const Graph& graph){
    Graph::const_iterator git = graph.begin();
    while (git != graph.end()){
	set<Node>::const_iterator sit = git->second.begin();
	while (sit != git->second.end()){
	    chars.erase(*sit);
	    ++sit;
	}
	++git;
    }
    
    if (chars.size() == 1){
	return *(chars.begin());
    }
    else{
	return 0;
    }
}

// Determines which is the final node: greater character (char not existant in map keyset)
// Final node must be unique. If not returns value 0 as 'error'.
Node get_final_node(set<char> chars, const Graph& graph){
    if ((chars.size() - graph.size()) != 1){
	return 0;
    }
    
    Graph::const_iterator it = graph.begin();
    while (it != graph.end()){
	chars.erase(it->first);
	++it;
    }
    
    return *(chars.begin());
}

// Strings represents a graph so far: map<char, set<char>>
//
//    p.e: dad < bad < cab < cda
//      'd' => {'b', 'c'}
//      'b' => {'c'}
//      'a' => {'d'}
//
void build_graph(Graph& graph, const vector<string>& strings){
    for (int i=1; i<strings.size(); ++i){
	const string& str1 = strings[i-1];
	const string& str2 = strings[i];

	bool stop = false;
	int ichar=0, max=min(str1.length(), str2.length());
	while (!stop && ichar<max){
	    char a = str1.at(ichar);
	    char b = str2.at(ichar);
	    if (a != b){
		Graph::iterator it = graph.find(a);
		if (it != graph.end()){
		    it->second.insert(b);
		}
		else {
		    graph.insert(make_pair(a, set<Node>())).first->second.insert(b);
		}
		stop = true;
	    }
	    ++ichar;
	}
    }
}

// Collects all available characters in strings with no duplicity
void get_all_chars(set<char>& chars, const vector<string>& strings){
    for (int i=0; i<strings.size(); ++i){
	const string& str = strings[i];
	for (int j=0; j<str.length(); ++j){
	    chars.insert(str.at(j));
	}
    }
}

void read_strings(ifstream& ifs, vector<string>& strings){
    string line;
    getline(ifs, line);
    int n_strings = stoi(line);

    while (n_strings --> 0){
	getline(ifs, line);
	strings.push_back(line);
    }
}
