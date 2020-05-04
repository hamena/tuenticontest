#include <cctype>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

#include "socket.hpp"

#define MAP_X 110
#define MAP_Y 110
#define VISION_X 5
#define VISION_Y 5

using namespace std;

struct Position {
    size_t x;
    size_t y;
};

struct Map {
    char m[MAP_Y][MAP_X];
    size_t sy, sx; // size y, size x
    Position kinit; // Initial position of the knight
};

struct Knight {
    Position pos;
    char vision[VISION_Y][VISION_X];
};

istream& operator >>(istream& is, Map& map) {
    is >> map.kinit.y >> map.kinit.x;
    is >> map.sy >> map.sx;
    for (size_t i=0; i<MAP_Y; ++i) {
	for (size_t j=0; j<MAP_X; ++j) {
	    is >> map.m[i][j];
	}
    }
    return is;
}

ostream& operator <<(ostream& os, const Position& p) {
    os << p.y << " " << p.x;
    return os;
}

ostream& operator <<(ostream& os, const Map& map) {
    os << map.kinit << endl;
    os << map.sy << " " << map.sx << endl;
    for (size_t i=0; i<MAP_Y; ++i) {
	for (size_t j=0; j<MAP_X; ++j) {
	    os << map.m[i][j];
	}
	os << endl;
    }
    return os;
}

void print(const Map& map) {
    for (size_t i=0; i<map.sy; ++i) {
	for (size_t j=0; j<map.sx; ++j) {
	    cout << map.m[i][j];
	}
	cout << endl;
    }
}

void read_last_move(char vision[VISION_Y][VISION_X]) {
    ifstream fVision("last_move.txt");
    for (size_t i=0; i<VISION_Y; ++i) {
	for (size_t j=0; j<VISION_X; ++j) {
	    fVision >> vision[i][j];
	}
    }
    fVision.close();
}

bool parse_move(const string& move, Position& kpos) {
    if (move=="2u1l") {
	kpos.y -= 2;
	kpos.x -= 1;
    } else if (move=="2u1r") {
	kpos.y -= 2;
	kpos.x += 1;
    } else if (move=="2d1l") {
	kpos.y += 2;
	kpos.x -= 1;
    } else if (move=="2d1r") {
	kpos.y += 2;
	kpos.x += 1;
    } else if (move=="1u2l") {
	kpos.y -= 1;
	kpos.x -= 2;
    } else if (move=="1u2r") {
	kpos.y -= 1;
	kpos.x += 2;
    } else if (move=="1d2l") {
	kpos.y += 1;
	kpos.x -= 2;
    } else if (move=="1d2r") {
	kpos.y += 1;
	kpos.x += 2;
    } else {
	return false;
    }
    return true;
}

void shift_right(Map& map, Position& kpos) {
    for (int i=0; i<MAP_Y; ++i) {
	for (int j=MAP_X-2; j>=0; --j) {
	    map.m[i][j+1] = map.m[i][j];
	}
    }
    for (int i=0; i<MAP_Y; ++i) {
	map.m[i][0] = '=';
    }
    ++kpos.x;
    ++map.kinit.x;
    ++map.sx;
}

void shift_down(Map& map, Position& kpos) {
    for (int i=MAP_Y-2; i>=0; --i){
	for (int j=0; j<MAP_X; ++j) {
	    map.m[i+1][j] = map.m[i][j];
	}
    }
    for (int j=0; j<MAP_X; ++j) {
	map.m[0][j] = '=';
    }
    ++kpos.y;
    ++map.kinit.y;
    ++map.sy;
}

void shift(Map& map, Position& kpos) {
    if (kpos.y < 2){
	int n = 2-kpos.y;
	for (size_t i=0; i<n; ++i) {
	    shift_down(map, kpos);
	}
    }
    if (kpos.y >= (map.sy-2)) {
	int n = map.sy-kpos.y;
        for (size_t i=0; i<n; ++i) {
	    ++map.sy;
        }
    }
    if (kpos.x < 2 || kpos.x > (map.sx-2)){
	int n = 2-kpos.x;
	for (size_t i=0; i<n; ++i) {
	    shift_right(map, kpos);
	}
    }
    if (kpos.x >= map.sx-2) {
	int n = map.sx-kpos.x;
        for (size_t i=0; i<n; ++i) {
	    ++map.sx;
        }
    }
}

void paste(Map& map, const Knight& k) {
    Position vision00 = k.pos;
    vision00.y -= 2;
    vision00.x -= 2;
    for (size_t i=0; i<VISION_Y; ++i) {
	for (size_t j=0; j<VISION_X; ++j) {
	    size_t y = vision00.y + i;
	    size_t x = vision00.x + j;
	    if (map.m[y][x] == '=' || map.m[y][x] == 'K'){
		map.m[y][x] = k.vision[i][j];
	    }
            /*
            if (map.m[y][x] == '=' || map.m[y][x] == 'K' || k.vision[i][j] ==
'K') { map.m[y][x] = k.vision[i][j]; } else if (map.m[y][x] != k.vision[i][j]) {
                cout << "Conflicting mapping ["<<y<<","<<x<<"] "
                     << map.m[y][x] << "!= " << k.vision[i][j] << endl;
            }
	    */
	}
    }
    map.m[k.pos.y][k.pos.x] = 'K';
}

void move_knight(Map& map, Knight& k, const string& move) {
    if (parse_move(move, k.pos)) {
	system(string("./new_move.sh "+move).c_str());
	system("./crop_last_move.sh > last_move.txt");
	read_last_move(k.vision);
	shift(map, k.pos);
	paste(map, k);
    }
}

int main(int argc, char **argv) {

    ifstream fmap("map.txt");
    Map map;
    if (fmap.good()) {
	cout << "read" << endl;
	fmap >> map;
    }
    fmap.close();
    
    Knight k;
    k.pos = map.kinit;

    ifstream fMemMoves("11.txt");
    string move;
    while (!fMemMoves.eof()) {
	fMemMoves >> move;
	cout << "Movimiento memorizado: " << move << endl;
	move_knight(map, k, move);
	move = "";
	print(map);
    }
    fMemMoves.close();

    ofstream fSession("session.txt");
    bool exit = false;
    while (!exit) {
	print(map);
        cout << "Nuevo movimiento: ";
        cin >> move;
	move_knight(map, k, move);
	exit = move == "exit";
	if (!exit) fSession << move << endl;
	move = "";
    }
    fSession.close();

    ofstream fMapResult("map-session.txt");
    fMapResult << map << endl;
    fMapResult.close();

    return 0;
}
