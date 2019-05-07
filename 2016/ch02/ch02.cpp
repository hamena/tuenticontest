#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <map>
#include <algorithm>
#include <vector>

#define GAP 9

typedef std::string Word;
typedef std::map<Word,unsigned> Dictionary;
typedef std::map<Word,unsigned>::iterator DictionaryIterator;
typedef std::vector<DictionaryIterator> Solution;

class Comparator{
public:
  bool operator()(const DictionaryIterator& it1, const DictionaryIterator& it2){
    return it1->second < it2->second;
  }
};

int main(){
  std::ifstream fileCorpus("corpus.txt");
  std::ifstream fileInput("testInput.txt");
  std::ofstream fileOutput("testOutput.txt");
  
  int nCases = 0;
  fileInput >> nCases;

  int start = 0, end = 0;
  for (int i=0; i<nCases; ++i){
    fileInput >> start;
    fileInput >> end;

    --start;       // Starting word is included
    
    start *= GAP;  // Start position in text
    end *= GAP;    // End position in text
    fileCorpus.seekg(start,std::ios::beg);  // Go to start position
    
    Dictionary dictionary;
    Solution solution;
    solution.clear();
    while (start < end){
      Word newWord;
      fileCorpus >> newWord;
      
      DictionaryIterator it = dictionary.find(newWord);
      if (it != dictionary.end())
	++(it->second);
      else{
	it = dictionary.insert(std::make_pair(newWord,1)).first;
	solution.push_back(it);
      }
      
      start += GAP;
    }

    // Print solution
    std::make_heap(solution.begin(), solution.end(), Comparator());
    fileOutput << "Case #" << i+1 << ": ";
    fileOutput << solution.front()->first << " " << solution.front()->second << ",";
    std::pop_heap(solution.begin(), solution.end(), Comparator());
    solution.pop_back();
    fileOutput << solution.front()->first << " " << solution.front()->second << ",";
    std::pop_heap(solution.begin(), solution.end(), Comparator());
    solution.pop_back();
    fileOutput << solution.front()->first << " " << solution.front()->second << std::endl;
  }

  fileCorpus.close();
  fileInput.close();
  fileOutput.close();
  
  return 0;
}
