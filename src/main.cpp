#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>

#include "letter.h"
#include "functions.h"

using namespace std;

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

	string out;

	for(auto i : content) {
		for(auto j = 0; j < alphabet.size(); ++j) {
			if(i == alphabet[j].getChar()) {
				out += alphabet[j].getCode();
			}
		}
	}

	cout << "Bit output: " << out << endl;
	cout << "File size: " << out.size() << "bits"  << " ~ " << static_cast<double>(out.size())/8 << "bytes" << endl;

	// write to new .zip file
	string name2 = argv[1];
	fstream outfile2;
	outfile2.open(name2 + ".info" , ios::binary | fstream::out);

	for(auto i = 0; i < alphabet.size(); ++i) {
		bitset<8> bitOut(alphabet[i].getChar());
		outfile2.write((char*) &bitOut, 1); // char representation
		bitset<8> sizeOut(alphabet[i].getCode().size());
		outfile2.write((char*) &sizeOut, 1); // codeword size
		bitset<8> codeOut(alphabet[i].getCode());
		outfile2.write((char*) &codeOut, 1); // codeword
	}

	outfile2.close();

	// write to new .zip file
	string name = argv[1];
	fstream outfile;
	outfile.open(name + ".extension" , ios::binary | fstream::out);

	for(auto i = 0; i < out.size(); i+=8) {
		bitset<8> bitOut(out.substr(i, 8));
		outfile.write((char*) &bitOut, 1);
	}

	outfile.close();
	return 0;
}
