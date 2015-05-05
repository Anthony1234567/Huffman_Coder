#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>

#include "letter.h"

using namespace std;

int count(const vector<letter> &members) {
	int counter = 0;
	for(auto i = 0; i < members.size(); ++i) {
		counter += members[i].getFreq();
	}
	return counter;
}

void calculateProbabilities(vector<letter> &members) {
	for(auto&& i : members) {
		i.setProbability(count(members));
	}
}

void printAlphabet(const vector<letter> &members) {
	for(auto i : members) {
	cout << "Symbol: ";
		if(i.getChar() == '\n') {
			cout << "\\n ";
		}
		else if(i.getChar() == ' ') {
			cout << "space ";
		}
		else {
			cout << i.getChar() << ' ';
		}
		cout << "Frequency: " << i.getFreq() << "/" << count(members) << ' '
			<< "Probability: " << i.getProb() << ' '
			<< "Encoding: " << i.getCode() << endl;
	}
}

void huffman(vector<letter> &members) {
	vector<vector<letter>> fusionTrei;
	fusionTrei.push_back(members);
	for(auto i = 0; i < fusionTrei[0].size() - 1; ++i) {
		vector<letter> iMembers;
		for(auto j = 0; j < fusionTrei[i].size() - 2; ++j) {
			iMembers.push_back(fusionTrei[i][j]);
		}
		iMembers.push_back(fusionTrei[i][fusionTrei[i].size() - 2].fuse(fusionTrei[i][fusionTrei[i].size() - 1]));
		sort(iMembers.begin(), iMembers.end());
		fusionTrei.push_back(iMembers);
	}

	for(auto i = 0; i < fusionTrei.size(); ++i) {
		for(auto j = 0; j < fusionTrei[i].size(); ++j) {
			if(fusionTrei[i][j].getChar() != '\0') {
				for(auto k = 0; k < members.size(); ++k) {
					if(members[k].getChar() == fusionTrei[i][j].getChar()) {
						members[k].bootlegencode(fusionTrei[i][j].getCode());
					}
				}
			}
		}
	}
}

bool seen(const char &l, vector<letter> &members) {
	for(auto&& i : members) {
		if(l == i.getChar()) {
			i.incrementFreq();
			return true;
		}
	}
	return false;
}

double calcAvgWordLen(vector<letter> members) {
	double len = 0;
	for(auto i = 0; i < members.size(); ++i) {
		len += members[i].getCode().size() * members[i].getProb();
	}
	return len;
}

int main(int argc, const char** argv) {
	fstream infile;
	infile.open(argv[1]);

	char viewing;
	string content;
	vector<letter> alphabet;

	while(infile.get(viewing)) {
		content += viewing;
		letter v(viewing);
		if(!seen(viewing, alphabet)) {
			alphabet.push_back(v);
		}
	}

	// get rid of extra '\n' character at end of text;
	for(auto i = 0; i < alphabet.size(); ++i) {
		if(alphabet[i].getChar() == '\n') {
			alphabet[i].decrimentFreq();
			if(alphabet[i].getFreq() == 0) {
				alphabet.erase(alphabet.begin() + i);
			}
		}
	}

	calculateProbabilities(alphabet);
	sort(alphabet.begin(), alphabet.end());
	huffman(alphabet);
	printAlphabet(alphabet); //after encoding
	cout << "Average word length: " << calcAvgWordLen(alphabet) << endl;

	infile.close();

	// write to new .zip file
	string name = argv[1];
	fstream outfile;
	outfile.open(name + ".extension" , ios::binary | fstream::out);

	string out;

	for(auto i : content) {
		for(auto j = 0; j < alphabet.size(); ++j) {
			if(i == alphabet[j].getChar()) {
				//outfile << alphabet[j].getCode();
				out += alphabet[j].getCode();
			}
		}
	}

	for(auto i = 0; i < out.size(); i+=8) {
		bitset<8> bitOut(out.substr(i,8));
		outfile.write((char*) &bitOut, 1);
	}

	outfile.close();
	return 0;
}
