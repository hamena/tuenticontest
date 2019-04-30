#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>

#define KB_WIDTH 10
#define KB_HEIGHT 4

using namespace std;

typedef pair<int, int> Position;

string decode_message(const string&, int, int);

void calculate_position(Position&, int, int);
char get_kb_char(const Position&);
Position get_kb_position(char);
char to_upper_case(char);

char keyboard[KB_HEIGHT][KB_WIDTH] =
    {
	{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
	{'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
	{'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';'},
	{'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '-'}
    };

Position g_position(2,4);
Position b_position(3,4);

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

    while (n_cases --> 0) {
	// Reads and saves known sender char keyboard position
	getline(ifs, line);
	char sender = to_upper_case(line.at(0));
	Position p_sender = sender == 'G' ? g_position : b_position;

	// Reads coded message and saves the coded sender char keyboard position
	string x_message;
	getline(ifs, x_message);
	char x_sender = to_upper_case(x_message.back());
	Position px_sender = get_kb_position(x_sender);

	// Applies the keyboard distance of known sender and coded sender keys
	// to the coded message
	int yd = p_sender.first - px_sender.first;
	int xd = p_sender.second - px_sender.second;
	string message = decode_message(x_message, yd, xd);
	
	cout << "Case #" << i_case++ << ": " << message << endl;
    }

    ifs.close();
    exit(0);
}

string decode_message(const string& x_message, int yd, int xd){
    string message = "";
    for (int i=0; i<x_message.length(); ++i){
	char c = to_upper_case(x_message.at(i));
	if (c != ' '){
	    Position p = get_kb_position(c);
	    calculate_position(p, yd, xd);
	    message.push_back(get_kb_char(p));
	}
	else {
	    message.push_back(c);
	}
    }
    return message;
}

void calculate_position(Position& p, int yd, int xd){
    p.first += yd;
    p.second += xd;

    if (p.first < 0){
	p.first = KB_HEIGHT - (-p.first);
    }
    else if (p.first >= KB_HEIGHT){
	p.first = p.first % KB_HEIGHT;
    }

    if (p.second < 0){
	p.second = KB_WIDTH - (-p.second);
    }
    else if (p.second >= KB_WIDTH){
	p.second = p.second % KB_WIDTH;
    }
}

// ascii conversion for a-z to A-Z
char to_upper_case(char c){
    if (c >= 97 && 122 >= c){
	return c - 32;
    }
}

char get_kb_char(const Position& p){
    if (p.first >= 0 && p.second >= 0 && p.first < KB_HEIGHT && p.second < KB_WIDTH){
	return keyboard[p.first][p.second];
    } else {
	return ' ';
    }
}

Position get_kb_position(char c){
    for (int i=0; i<KB_HEIGHT; ++i){
	for (int j=0; j<KB_WIDTH; ++j){
	    if (keyboard[i][j] == c)
		return make_pair(i,j);
	}
    }
    return make_pair(-1,-1);
}
