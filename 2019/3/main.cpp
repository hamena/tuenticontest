#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

typedef char Fold;
typedef pair<int, int> Punch;

void predict_punches(int, int, vector<Fold>&, vector<Punch>&);

void readFolds(ifstream&, vector<Fold>&);
void readPunches(ifstream&, vector<Punch>&);

int main(int argc, char **argv){
    if (argc < 2){
	cerr << "You must provide an input file as argument." << endl;
	exit(1);
    }

    ifstream ifs(argv[1]);
    int n_cases;
    ifs >> n_cases;
    int i_case = 1;

    while (n_cases --> 0){
	int width, height, n_folds, n_punches;
	ifs >> width >> height >> n_folds >> n_punches;
	
	vector<Fold> folds(n_folds);
	vector<Punch> punches(n_punches);
	readFolds(ifs, folds);
	readPunches(ifs, punches);

	predict_punches(width, height, folds, punches);

	sort(punches.begin(), punches.end());
	
	cout << "Case #" << i_case++ << ":" << endl;
	for (int i=0; i<punches.size(); ++i){
	    cout << punches[i].first << " " << punches[i].second << endl;
	}
    }

    ifs.close();
    exit(0);
}

void predict_punches(int width, int height, vector<Fold>& folds, vector<Punch>& punches){
    while (folds.size() > 0){
	vector<Punch> new_punches(0);
	switch (folds.back()){
	case 'T':
	    for (int i=0; i<punches.size(); ++i){
		Punch punch = punches[i];
		punch.second = height - (punch.second+1);
		new_punches.push_back(punch);
		punches[i].second += height;
	    }
	    height = height << 1;
	    break;
	case 'B':
	    for (int i=0; i<punches.size(); ++i){
		Punch punch = punches[i];
		punch.second = height*2 - (punch.second+1);
		new_punches.push_back(punch);
	    }
	    height = height << 1;
	    break;
	case 'R':
	    for (int i=0; i<punches.size(); ++i){
		Punch punch = punches[i];
		punch.first = width*2 - (punch.first+1);
		new_punches.push_back(punch);
	    }
	    width = width << 1;
	    break;
	case 'L':
	    for (int i=0; i<punches.size(); ++i){
		Punch punch = punches[i];
		punch.first = width - (punch.first+1);
		new_punches.push_back(punch);
		punches[i].first += width;
	    }
	    width = width << 1;
	    break;
	}

	punches.insert(punches.end(), new_punches.begin(), new_punches.end());
	
	folds.pop_back();
    }
}

void readFolds(ifstream& ifs, vector<Fold>& folds){
    // Inserting backwards for consuming vector later as a stack (pop_back)
    for (int i=folds.size()-1; i>=0; --i){
	ifs >> folds[i];
    }
}

void readPunches(ifstream& ifs, vector<Punch>& punches){
    for (int i=0; i<punches.size(); ++i){
	ifs >> punches[i].first;
	ifs >> punches[i].second;
    }
}
