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
		hashNumber++;
		hashInt[str] = hashNumber;
		hashString[hashNumber] = str;
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

void lookForBugs(CallGraph callGraph,int firstMethod,int secondMethod,int support,double confidence){
	//calculate support for individual methods and pairs
	for(CallGraph::iterator it = callGraph.begin(); it!= callGraph.end(); ++it){	
		//we go through sets of methods of every scope
		int scope = it->first;
		set<int> scopeMethods = it->second;
		if(scopeMethods.find(firstMethod) != scopeMethods.end() //first method is in this scope 
			&& scopeMethods.find(secondMethod) == scopeMethods.end()) //second method is not in this scope
			{
				string firstMethodString = hashIntToString(firstMethod).c_str();
				string secondMethodString = hashIntToString(secondMethod).c_str();
				
				if (firstMethodString>secondMethodString){
					string help = firstMethodString;
					firstMethodString = secondMethodString;
					secondMethodString = help;
				}
				
				printf("bug: %s in %s, pair: (%s, %s), support: %d, confidence: %.2f%%\n",
					hashIntToString(firstMethod).c_str(),
					hashIntToString(scope).c_str(),
					firstMethodString.c_str(),
					secondMethodString.c_str(),
					support, 
					confidence);
				
			}
		
	}
}

int main (int argc, char* argv[]) {
	
	int support = atoi(argv[1]);
	double confidence = atoi(argv[2]);
	//printf("Support is %d",support);
	//printf("Confidence is %f", confidence);
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
		//printf("%s\n",line.c_str());	
		//parsing the input callgraph from opt and inserting it to our map representation of it	
		//scope line
		if(words.size() == 7){
			//std::cout << words[5];
			scope = words[5];
			size_t firstQuote = scope.find_first_of('\'');
			size_t lastQuote = scope.find_last_of('\'');
			scope = scope.substr(firstQuote+1, lastQuote-firstQuote-1);
			scopeInt = hashStringToInt(scope);
			//std::cout << scope;
			//printf("%s\n",hashIntToString(scopeInt).c_str());
		}
		//method in the scope
		else if (words.size() == 4){
			if(words[2] == "external" && words[3] == "scope") {
			  continue;
			}
			if(words[2] == "external" && words[3] == "node") {
			  continue;
			}			
			// we don't care about the root
			if (words[1] == "Root" || scope == "") {
			  continue;
			}
			string method = words[3];
			size_t firstQuote = method.find_first_of('\'');
			size_t lastQuote = method.find_last_of('\'');
			method = method.substr(firstQuote+1, lastQuote-firstQuote-1);
			int methodInt = hashStringToInt(method);
			//std::cout << methodInt;
			
			// add scope and method to callgraph
			callGraph[scopeInt].insert(methodInt);
			//printf("inserting into callgraph: %s, %s\n",scope.c_str(), hashIntToString(methodInt).c_str());
		}
		//std::cout << line// << std::endl;//outprint line to test it
		
		
	}
	
	
	
	//CallGraph::iterator it = callGraph.begin();
	//calculate support for individual methods and pairs
	//while(it != callGraph.end()){
		
	for(CallGraph::iterator it = callGraph.begin(); it!= callGraph.end(); ++it){
		//we go through sets of methods of every scope
		set<int> scopeMethods = it->second;
		
		//calculate individual support
		for(set<int>::iterator it2 = scopeMethods.begin();it2!=scopeMethods.end(); ++it2){
			individualSupport[*it2]++;
			//std::cout << individualSupport[*it2];
		}
		
		//calculate pair support within the scope in while loop, add it to the pair support table
		for(set<int>::iterator it3 = scopeMethods.begin();it3!=scopeMethods.end(); ++it3){	//iterate over all methods	
			for(set<int>::iterator it4 = /*scopeMethods.begin()*/it3;it4!=scopeMethods.end(); ++it4){ //iterate over all methods again
				if(*it3 != *it4){//except the one we already have in the loop one level up
					//printf("comparing %s and %s\n",hashIntToString(*it3).c_str(),hashIntToString(*it4).c_str());
					pair<int,int> pair;			
					if(*it3<*it4){
						pair = make_pair(*it3,*it4);
					}
					else{
						pair = make_pair(*it4,*it3);
					}
					pairSupport[pair]++;	
				}	
							
			}
			
			//std::cout << individualSupport[*it2];
		}		
	}
	
	for(PairSupport::iterator it5 = pairSupport.begin(); it5!= pairSupport.end(); ++it5){
		pair<int,int> pair = it5->first;
		int thisPairSupport = it5->second;
		if (thisPairSupport >= support){
			int firstMethod = pair.first;
			int secondMethod = pair.second;
			double firstConfidence = ((double)thisPairSupport/(double)individualSupport[firstMethod])*100.00;
			//printf("thispairsupport: %.2f",(double)thisPairSupport);
			//printf("/ individualsupport: %.2f",(double)individualSupport[firstMethod]);
			
			
			
			double secondConfidence = ((double)thisPairSupport/(double)individualSupport[secondMethod])*100.00;
			
			//printf("firstConfidence: %f", firstConfidence);
			//printf("secondConfidence: %f", secondConfidence);
			//printf("confidence: %f", confidence);
			
			if(firstConfidence >= confidence){
				lookForBugs(callGraph,firstMethod,secondMethod,thisPairSupport,firstConfidence);
			}
			if(secondConfidence >= confidence){
				lookForBugs(callGraph,secondMethod,firstMethod,thisPairSupport,secondConfidence);
			}
		}
	}
	
	
		
	return 0;
}