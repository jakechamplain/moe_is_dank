#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <set>


using namespace std;

//callgraph with scope as key and its methods
typedef map<int, set<int> > CallGraph;

//hashtable for translating to hashes/translating back to names of scopes and methods
typedef map <int, string> HashString;

typedef map <string, int> HashInt;

typedef map <int, int> IndividualSupport;
typedef map <pair<int,int>, int> PairSupport;

int hashNumber = 0;
HashString hashString;
HashInt hashInt;
IndividualSupport individualSupport;
PairSupport pairSupport;

int hashStringToInt (string str){
	if (hashInt.count(str)){
		return hashInt[str];
	}
	else{
		hashInt[str] = hashNumber;
		hashString[hashNumber] = str;
		hashNumber++;
		return hashNumber;
	}	
}

string hashIntToString (int i){
	if (hashString.count(i)){
		return hashString[i];
	}
	else{		
		return "";
	}	
}

int main (int argc, char* argv[]) {
	
	int support = atoi(argv[1]);
	int confidence = atoi(argv[2]);
	//printf("Support is %d",support);
	//printf("Confidence is %d", confidence);
	CallGraph callGraph;
	
	string line;
	
	/*we create scope before, so its value can be kept even while 
	iterating over its methods*/
	string scope = ""; 
	int scopeInt = 0;
	
	while (getline(cin, line)){
		if (line == ""){
			continue;
		}
		
		//trimming line so it has no spaces before or after itself
		line = line.substr(line.find_first_not_of(' '), line.find_last_not_of(' ')+1);
		
		//chopping the input line into individual words for parsing
		vector<string> words;
		stringstream ss(line);
		string word;
		int wordInt;
		while (std::getline(ss, word, ' ')) {
			if (word != "") {
			  words.push_back(word);
			}
		}		
			
		//parsing the input callgraph from opt and inserting it to our map representation of it	
		//scope line
		if(words.size() == 7){
			//std::cout << words[5];
			scope = words[5];
			size_t first = scope.find_first_of('\'');
			size_t last = scope.find_last_of('\'');
			scope = scope.substr(first+1, last-first-1);
			scopeInt = hashStringToInt(scope);
			//std::cout << scope;
		}
		//method in the scope
		else if (words.size() == 4){
			if(words[2] == "external" && words[3] == "scope") {
			  continue;
			}
			// we don't care about the root
			if (words[1] == "Root" || scope == "") {
			  continue;
			}
			string method = words[3];
			int methodInt = hashStringToInt(method);
			//std::cout << methodInt;
			
			// add scope and method to callgraph
			callGraph[scopeInt].insert(methodInt);
		}
		//std::cout << line << std::endl;//outprint line to test it
		
		
	}
	
	
	
	CallGraph::iterator it = callGraph.begin();
	//calculate support for individual methods and pairs
	while(it != callGraph.end()){
		//we go through sets of methods of every scope
		set<int> scopeMethods = it->second;
		
		//calculate individual support
		for(set<int>::iterator it2 = scopeMethods.begin();it2!=scopeMethods.end(); ++it2){
			individualSupport[*it2]++;
			//std::cout << individualSupport[*it2];
		}
		
		//calculate pair support within the scope in while loop, add it to the pair support table
		for(set<int>::iterator it3 = scopeMethods.begin();it3!=scopeMethods.end(); ++it3){	//iterate over all methods	
			for(set<int>::iterator it4 = scopeMethods.begin();it4!=scopeMethods.end(); ++it4){ //iterate over all methods again
				if(*it3 == *it4){
					continue; //except the one we already have in the loop one level up
				}	
				pair<int,int> pair;			
				if(*it3<*it4){
					pair = make_pair(*it3,*it4);
				}
				else{
					pair = make_pair(*it4,*it3);
				}
				pairSupport[pair]++;				
			}
			
			//std::cout << individualSupport[*it2];
		}
		
		it++;
	}
	
	//analyze graph with our knowledge of support
	CallGraph::iterator it5 = callGraph.begin();
	while(it5 != callGraph.end()){		//over every scope
		set<int> scopeMethods2 = it5->second;
		int scopeInteger = it5->first;
		//calculate pair support within the scope in while loop, add it to the pair support table
		for(set<int>::iterator it7 = scopeMethods2.begin();it7!=scopeMethods2.end(); ++it7){	//iterate over all methods		
			int method1 = *it7;
			double methodConfidence;
			for(set<int>::iterator it8 = scopeMethods2.begin();it8!=scopeMethods2.end(); ++it8){ //iterate over all methods again
				if(*it7 == *it8){
					continue; //except the one we already have in the loop one level up
				}				
				pair<int,int> pair;
				if(*it7<*it8){
					pair = make_pair(*it7,*it8);
				}
				else{
					pair = make_pair(*it8,*it7);
				}
				methodConfidence = (pairSupport[pair] / individualSupport[*it7])*100;	
				//std::cout << methodConfidence;
				
				if (methodConfidence >= confidence && pairSupport[pair] >= support){
					printf("bug: %s in %s, pair: (%s, %s), support: %d, confidence: %.2f%%\n",hashIntToString(*it7).c_str(),hashIntToString(scopeInteger).c_str(),hashIntToString(*it7).c_str(),hashIntToString(*it8).c_str(),pairSupport[pair], methodConfidence);
				}
			}
			
			//std::cout << individualSupport[*it2];
		}
		
		it5++;
	}
	
		
	return 0;
}