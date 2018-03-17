#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main (int argc, char* argv[]) {
	
	int support = atoi(argv[1]);
	int confidence = atoi(argv[2]);
	//printf("Support is %d",support);
	//printf("Confidence is %d", confidence);
	
	
	string line;
	
	/*we create scope before, so its value can be kept even while 
	iterating over its methods*/
	string scope = ""; 
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
		while (std::getline(ss, word, ' ')) {
			if (word != "") {
			  words.push_back(word);
			}
		}		
			
		//parsing	
		
		
		//scope line
		if(words.size() == 7){
			std::cout << words[5];
			scope = words[5];
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
			std::cout << words[3];
		}
		//TODO add scope and method to callgraph
		
		
		
		
		
		//std::cout << line << std::endl;//outprint line to test it
	}
	
	//TODO process graph and outprint results
		
	return 0;
}