#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <set>
#include <tuple>
#include <cstring>

using namespace std;

/*
 * 一二三四五六七八九十百千万
 *
 * 一 二 三  四 五 六  七 八 九  十  百    千     万
 * 1  2  3  4  5  6  7  8  9  10  100  1000   10000
 */
class Kanji{
public:
    Kanji(char a, char b, char c, const char* str, int value) :
	_a(a), _b(b), _c(c), _str(str), _value(value) {}
    const char* str() const{ return _str; }
    int value() const{ return _value; }
private:
    char _a,_b,_c;
    const char* _str;
    int _value;
};
class K1 : public Kanji{ public: K1() : Kanji(-28, -72, -128, "一", 1){} };
class K2 : public Kanji{ public: K2() : Kanji(-28, -70, -116, "二", 2){} };
class K3 : public Kanji{ public: K3() : Kanji(-28, -72, -119, "三", 3){} };
class K4 : public Kanji{ public: K4() : Kanji(-27, -101, -101, "四", 4){} };
class K5 : public Kanji{ public: K5() : Kanji(-28, -70, -108, "五", 5){} };
class K6 : public Kanji{ public: K6() : Kanji(-27, -123, -83, "六", 6){} };
class K7 : public Kanji{ public: K7() : Kanji(-28, -72, -125, "七", 7){} };
class K8 : public Kanji{ public: K8() : Kanji(-27, -123, -85, "八", 8){} };
class K9 : public Kanji{ public: K9() : Kanji(-28, -71, -99, "九", 9){} };
class K10 : public Kanji{ public: K10() : Kanji(-27, -115, -127, "十", 10){} };
class K100 : public Kanji{ public: K100() : Kanji(-25, -103, -66, "百", 100){} };
class K1000 : public Kanji{ public: K1000() : Kanji(-27, -115, -125, "千", 1000){} };
class K10000 : public Kanji{ public: K10000() : Kanji(-28, -72, -121, "万", 10000){} };

K1 k1; K2 k2; K3 k3; K4 k4; K5 k5; K6 k6; K7 k7; K8 k8; K9 k9;
K10 k10; K100 k100; K1000 k1000; K10000 k10000;

typedef vector<Kanji> KanjiWord;
typedef set<Kanji> KanjiGroup;

bool operator <(const Kanji& k1, const Kanji& k2){
    return k1.value() < k2.value();
}

bool operator ==(const Kanji& k1, const Kanji& k2){
    return k1.value() == k2.value();
}

bool operator !=(const Kanji& k1, const Kanji& k2){
    return !(k1.value() == k2.value());
}

bool operator <=(const Kanji& k1, const Kanji& k2){
    return k1.value() < k2.value() || k1.value() == k2.value();
}

void get_operations(vector<tuple<int, int, int> >&,const set<int>&, const set<int>&, const set<int>&);
void get_combinations(set<int>&, const KanjiWord&);
void build_combinations(set<int>&, KanjiWord, const vector<Kanji>&, const vector<Kanji>&);
int to_int(const KanjiWord&);
vector<Kanji> erase(const vector<Kanji>&, const Kanji&);
Kanji get_max(const vector<Kanji>&);
void clasify_kanjis(vector<Kanji>&, vector<Kanji>&, const KanjiWord&);
bool is_factor(const Kanji&);
void build_kanji_word(KanjiWord&, const char *);
Kanji parse_kanji(int, int, int);

int main(int argc, char **argv){

    if (argc < 2){
	cerr << "You must provide an input file as argument." << endl;
	return 1;
    }
    
    ifstream ifs(argv[1]);
    int n_cases;
    ifs >> n_cases;
    
    int i_case = 1;
    while (n_cases --> 0){
	string first, op, second, eq, result;
	ifs >> first >> op >> second >> eq >> result;

        KanjiWord w_first, w_second, w_result;
	build_kanji_word(w_first, first.c_str());
	build_kanji_word(w_second, second.c_str());
	build_kanji_word(w_result, result.c_str());

	set<int> c_first, c_second, c_result;
	get_combinations(c_first, w_first);
	get_combinations(c_second, w_second);
	get_combinations(c_result, w_result);

        vector<tuple<int, int, int> > operations;
	get_operations(operations, c_first, c_second, c_result);
	
	cout << "Case #" << i_case++ << ": ";
	for (int i=0; i<operations.size(); ++i){
	    int a = get<0>(operations[i]);
	    int b = get<1>(operations[i]);
	    int c = get<2>(operations[i]);
	    if ((a + b) == c)
		cout << a << " + " << b << " = " << c << endl;
	    else if ((a - b) == c)
		cout << a << " - " << b << " = " << c << endl;
	    else if ((a * b) == c)
		cout << a << " * " << b << " = " << c << endl;
	}
    }

    ifs.close();
    return 0;
}

void get_operations(vector<tuple<int, int, int> >& operations,
		    const set<int>& c_first, const set<int>& c_second, const set<int>& c_result){
    set<int>::const_iterator i;
    for (i=c_first.begin(); i!=c_first.end(); ++i){
	set<int>::const_iterator j;
	for (j=c_second.begin(); j!=c_second.end(); ++j){
	    set<int>::const_iterator k;
	    for (k=c_result.begin(); k!=c_result.end(); ++k){
		operations.push_back(make_tuple(*i, *j, *k));
	    }
	}
    }
}

void get_combinations(set<int>& combinations, const KanjiWord& word){
    if (word.size() == 1){
	combinations.insert(to_int(word));
    }
    else {
	vector<Kanji> numeric, factor;
	clasify_kanjis(numeric, factor, word);

	KanjiWord aux;
	build_combinations(combinations, aux, numeric, factor);
    }
}

void build_combinations(set<int>& combinations, KanjiWord current,
			const vector<Kanji>& numeric, const vector<Kanji>& factor){
    if (numeric.size() == 0 && factor.size() == 0){
	combinations.insert(to_int(current));
    }
    else if (factor.size() == 0){
	if (is_factor(current.back())){
	    current.push_back(numeric.back());
	    build_combinations(combinations, current, erase(numeric, numeric.back()), factor);
	}
    }
    else if (numeric.size() == 0){
	Kanji max = get_max(factor);
	current.push_back(max);
	build_combinations(combinations, current, numeric, erase(factor, max));
    }
    else {
	Kanji max = get_max(factor);
	bool isK10000 = max == k10000;
	vector<Kanji> f_temp = erase(factor, max);
	for (int i=0; i<numeric.size(); ++i){
	    const Kanji& k = numeric[i];
	    if (isK10000 || !(k == k1)){
		current.push_back(k);
		current.push_back(max);
	        build_combinations(combinations, current, erase(numeric, k), f_temp);
		current.pop_back();
		current.pop_back();
	    }
	    if (!isK10000 || current.size()>0){
		current.push_back(max);
		build_combinations(combinations, current, numeric, f_temp);
		current.pop_back();
	    }
	}
    }
}

int to_int(const KanjiWord& word, int i, const Kanji* last){
    if (i >= word.size()){
	return last != NULL ? last->value() : 0;
    } else {
	const Kanji& k = word[i];
	int v = k.value();
	if (is_factor(k)){
	    return v*(last!=NULL ? last->value() : 1) + to_int(word, i+1, NULL);
	} else {
	    return to_int(word, i+1, &k);
	}
    }
}

int to_int(const KanjiWord& word){
    return to_int(word, 0, NULL);
}

vector<Kanji> erase(const vector<Kanji>& kanjis, const Kanji& k){
    vector<Kanji> temp;
    bool found = false;
    for (int i=0; i<kanjis.size(); ++i) {
	if (!found && kanjis[i] == k){
	    found = true;
	} else {
	    temp.push_back(kanjis[i]);
	}
    }
    return temp;
}

Kanji get_max(const vector<Kanji>& group){
    Kanji max = K10();
    for (int i=0; i<group.size(); ++i){
	if (max < group[i]){
	    max = group[i];
	}
    }
    return max;
}

void clasify_kanjis(vector<Kanji>& numeric, vector<Kanji>& factor, const KanjiWord& kanjis){
    for (int i=0; i<kanjis.size(); ++i){
	const Kanji& k = kanjis[i];
	if (is_factor(k))
	    factor.push_back(k);
	else
	    numeric.push_back(k);
    }
}

bool is_factor(const Kanji& k){
    return k10 <= k && k <= k10000;
}

void build_kanji_word(KanjiWord& v_kanjis, const char * str_kanjis){
    int max = strlen(str_kanjis);
    for (int i=0; i<max; i+=3){
	v_kanjis.push_back(parse_kanji(str_kanjis[i], str_kanjis[i+1], str_kanjis[i+2]));
    }
}

Kanji parse_kanji(int a, int b, int c){
    if (a == -28 && b == -72 && c == -128) return k1;
    else if (a == -28 && b == -70 && c == -116)	return k2;
    else if (a == -28 && b == -72 && c == -119) return k3;
    else if (a == -27 && b == -101 && c == -101) return k4;
    else if (a == -28 && b == -70 && c == -108)	return k5;
    else if (a == -27 && b == -123 && c == -83)	return k6;
    else if (a == -28 && b == -72 && c == -125) return k7;
    else if (a == -27 && b == -123 && c == -85) return k8;
    else if (a == -28 && b == -71 && c == -99) return k9;
    else if (a == -27 && b == -115 && c == -127) return k10;
    else if (a == -25 && b == -103 && c == -66) return k100;
    else if (a == -27 && b == -115 && c == -125) return k1000;
    else if (a == -28 && b == -72 && c == -121)return k10000;
}
