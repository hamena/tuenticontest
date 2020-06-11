#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "socket.h"

#define IP "52.49.91.111"
#define PORT 2003
#define MAP_Y 110
#define MAP_X 110
#define VISION_Y 5
#define VISION_X 5

using namespace std;

struct Position {
    int y;
    int x;
    Position(int y, int x) : y(y), x(x) {}
    Position() : y(0), x(0) {}
};

struct Knight {
    Position pos = {2,2};
    char vision[VISION_Y][VISION_X];
};

struct Map {
    char m[MAP_Y][MAP_X];
    size_t sy, sx; // size y, size x
    Map() : sy(VISION_Y), sx(VISION_X) {
	for (size_t i=0; i<MAP_Y; ++i) {
	    for (size_t j=0; j<MAP_X; ++j) {
		m[i][j] = '~';
	    }
	}
    }
};

ostream& operator <<(ostream& os, const Map& map) {
    os << map.sy << " " << map.sx << endl;
    for (size_t i=0; i<map.sy; ++i) {
	for (size_t j=0; j<map.sx; ++j) {
	    os << map.m[i][j];
	}
	os << endl;
    }
    return os;
}

void shift_right(Map& map, Knight& k) {
    for (int i=0; i<MAP_Y; ++i) {
	for (int j=MAP_X-2; j>=0; --j) {
	    map.m[i][j+1] = map.m[i][j];
	}
    }
    for (int i=0; i<MAP_Y; ++i) {
	map.m[i][0] = '~';
    }
    ++k.pos.x;
    ++map.sx;
}

void shift_down(Map& map, Knight& k) {
    for (int i=MAP_Y-2; i>=0; --i){
	for (int j=0; j<MAP_X; ++j) {
	    map.m[i+1][j] = map.m[i][j];
	}
    }
    for (int j=0; j<MAP_X; ++j) {
	map.m[0][j] = '~';
    }
    ++k.pos.y;
    ++map.sy;
}

void shift(Map& map, Knight& k) {
    while (k.pos.y < 2) {
	shift_down(map, k);
    }
    while (k.pos.x < 2) {
	shift_right(map, k);
    }
    while (k.pos.y >= (map.sy-2)) {
	++map.sy;
    }
    while (k.pos.x >= (map.sx-2)) {
	++map.sx;
    }
}

void paste_vision(Map& map, const Knight& k) {
    Position vision00 = k.pos;
    vision00.y -= 2;
    vision00.x -= 2;
    for (size_t i=0; i<VISION_Y; ++i) {
	for (size_t j=0; j<VISION_X; ++j) {
	    size_t y = vision00.y + i;
	    size_t x = vision00.x + j;
	    if (map.m[y][x] == '~' || k.vision[i][j] == 'K') {
		map.m[y][x] = k.vision[i][j];
	    }/* else if (map.m[y][x] != k.vision[i][j]) {
                cout << "Conflicting mapping [" << y << "," << x << "] "
                     << map.m[y][x] << "!= " << k.vision[i][j] << endl;
		     }*/
	}
    }
}

void read_knight_vision(Knight &knight, const string& response) {
    stringstream sstream(response);
    for (size_t i=0; i<VISION_Y; ++i) {
	for (size_t j=0; j<VISION_X; ++j) {
	    sstream >> knight.vision[i][j];
	}
    }
}

void print_knight_vision(const Knight &knight) {
    cout << endl;
    for (size_t i=0; i<VISION_Y; ++i) {
	for (size_t j=0; j<VISION_X; ++j) {
	    cout << knight.vision[i][j];
	}
	cout << endl;
    }
}

string backwards(const string& move) {
    string back = move;
    back[1] = back[1]=='d' ? 'u' : 'd';
    back[3] = back[3]=='l' ? 'r' : 'l';
    return back;
}

struct Move {
    int oy, ox;
    string command;
    Move(int oy, int ox, const string& command) :
	oy(oy), ox(ox), command(command) {}
};

vector<Move> moves =
    {
     Move(2, -1, "2d1l"),
     Move(1, -2, "1d2l"),
     Move(-1, -2, "1u2l"),
     Move(-2, -1, "2u1l"),
     Move(-2, 1, "2u1r"),
     Move(-1, 2, "1u2r"),
     Move(1, 2, "1d2r"),
     Move(2, 1, "2d1r")
    };

void explore(Socket& laberinth, Map& map, Knight& knight) {

    for (const auto& move : moves) {
	Position pos(knight.pos.y+move.oy, knight.pos.x+move.ox);
	char v = map.m[pos.y][pos.x];
	
	if (v == 'P') {
	    laberinth.send_msg(move.command.c_str());

	    ofstream ofs("pass.txt");
	    ofs << laberinth.read_msg() << endl;
	    ofs.close();
	} else if (v == '.') {
	    laberinth.send_msg(move.command.c_str());

	    map.m[knight.pos.y][knight.pos.x] = ' ';
	    knight.pos.y += move.oy;
	    knight.pos.x += move.ox;
	    shift(map, knight);

            read_knight_vision(knight, laberinth.read_msg());
	    paste_vision(map, knight);
	    cout << map << endl;

	    explore(laberinth, map, knight);

	    // backtrack
	    laberinth.send_msg(backwards(move.command).c_str());
	    map.m[knight.pos.y][knight.pos.x] = ' ';
	    knight.pos.y += -move.oy;
	    knight.pos.x += -move.ox;
            read_knight_vision(knight, laberinth.read_msg());
	    paste_vision(map, knight);
	}
    }
}

int main() {
    Socket laberinth("52.49.91.111", 2003);
    //Socket laberinth("127.0.0.1", 4444);
    Map map;
    Knight knight;

    read_knight_vision(knight, laberinth.read_msg());
    paste_vision(map, knight);
    cout << map << endl;
    
    explore(laberinth, map, knight);
    
    laberinth.dismiss();

    return 0;
}
