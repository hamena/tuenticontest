#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <locale>
#include <codecvt>

using namespace std;

vector<char16_t> legal_chars = {
    'a', 'b', 'c', 'd',  'e', 'f',  'g',  'h',  'i',  'j',  'k',
    'l', 'm', 'n', u'ñ', 'o', 'p',  'q',  'r',  's',  't',  'u',
    'v', 'w', 'x', 'y',  'z', u'á', u'é', u'í', u'ó', u'ú', u'ü'};
wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

void read_fortunata_y_jacinta(ifstream& ifs, map<string, size_t>& word_count);
void formalize(string &str);
char16_t tolowercase(char16_t c);

void rank_words(const map<string, size_t>& word_count,
		vector< pair<string, size_t> >& ranking);

bool is_number(const string& s);

int main(int argc, char **argv) {
    
    if (argc >= 3) {
	map<string, size_t> word_count;
	
	ifstream ifs(argv[1]); // pg17013.txt
	read_fortunata_y_jacinta(ifs, word_count);
	ifs.close();

	vector< pair<string, size_t> > ranking;
	rank_words(word_count, ranking);

        ifs.open(argv[2]); // input
	size_t n_cases, i_case=1;
	ifs >> n_cases;
	
	string line;
        while (n_cases --> 0) {
	    cout << "Case #" << i_case++ << ": ";
	    string result;
	    ifs >> line;
	    if (is_number(line)) {
		size_t i = stoi(line)-1;
		cout << ranking[i].first << " " << ranking[i].second << endl;
	    } else {
		bool found = false;
		size_t i=0;
		for (; !found && i<ranking.size(); ++i) {
		    found = ranking[i].first == line;
		}
		cout << ranking[i-1].second << " #" << i << endl;
	    }
	}
	
	ifs.close();
	
    } else {
	return 1;
    }
    return 0;
}

void read_fortunata_y_jacinta(ifstream& ifs, map<string, size_t>& word_count) {
    string line;
    while (!ifs.eof()) {
	getline(ifs,line);
	formalize(line);

	istringstream iss(line);
	vector<string> words;
	copy_if(istream_iterator<string>(iss), istream_iterator<string>(),
		back_inserter(words), [](const string& str){
					  return convert.from_bytes(str).length() > 2;
				      });

	for (const string& word : words) {
	    auto it = word_count.find(word);
	    if (it == word_count.end()){
		word_count.insert(make_pair(word, 1));
	    } else {
		++(it->second);
	    }
	}
    }
}

void formalize(string& str) {
    u16string u16str = convert.from_bytes(str);
    transform(u16str.begin(), u16str.end(), u16str.begin(),
	      [] (char16_t a) {
		  a = tolowercase(a);
		  if (find(legal_chars.begin(), legal_chars.end(),a) == legal_chars.end()){
		      return u' ';
		  } else {
		      return a;
		  }
	      });
    str = convert.to_bytes(u16str);
}

char16_t tolowercase(char16_t c) {
    switch(c) {
    case u'Á':
	return u'á';
    case u'É':
	return u'é';
    case u'Í':
	return u'í';
    case u'Ó':
	return u'ó';
    case u'Ú':
	return u'ú';
    case u'Ü':
	return u'ü';
    case u'Ñ':
	return u'ñ';
    default:
	return tolower(c);
    }
}

void rank_words(const map<string, size_t>& word_count,
		vector< pair<string, size_t> >& ranking) {
    copy(word_count.begin(), word_count.end(), back_inserter(ranking));
    sort(ranking.begin(), ranking.end(),
	 [] (const pair<string, size_t>& p1, const pair<string, size_t>& p2) {
	     if (p1.second == p2.second) {
		 return p1.first < p2.first;
	     } else
		 return p1.second > p2.second;
	 });
}

bool is_number(const string& s) {
    return !s.empty() &&
	find_if(s.begin(), s.end(), [](char c) { return !isdigit(c); }) == s.end();
}
