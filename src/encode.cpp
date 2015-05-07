#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <bitset>

#include "letter.h"
#include "functions.h"

using namespace std;

int main(int argc, const char** argv) {
	// filename
	string fname = argv[1];

	// open file
	fstream infile;
	infile.open(fname);

	// content for file as string
	string content;
	char viewing;
	vector<letter> alphabet;

	// count occurence of each character seen
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

	// find alternate code for characters
	calculateProbabilities(alphabet);
	sort(alphabet.begin(), alphabet.end());
	huffman(alphabet); // huffman-ish algo on sorted list
	printAlphabet(alphabet); //after encoding

	// for personal uses
	cout << "Average codeword length: " << calcAvgWordLen(alphabet) << endl;

	infile.close();

	// output
	string out;

	// encode content
	for(auto i : content) {
		for(auto j = 0; j < alphabet.size(); ++j) {
			if(i == alphabet[j].getChar()) {
				out += alphabet[j].getCode();
			}
		}
	}

	// personal uses
	cout << "File size: " << out.size() + 2*8 + alphabet.size()*3*8 << "bits"
		<< " ~ " << out.size() + 2 + static_cast<double>(alphabet.size())*3/8
				<< "bytes" << endl;

//-----------------------------------------------------------------------------

	// .info file holds key
	fstream outfile;
	outfile.open(fname + ".compressed" , ios::binary | fstream::out);


	// exta bits at the end due to hard coded 8 bit size
	bitset<8> sizeRemainder(8 - out.size() % 8);
	outfile.write((char*) &sizeRemainder, 1);

	bitset<8> keySize(alphabet.size() * 8 * 3);
	outfile.write((char*) &keySize, 1);

	// formatting key
	for(auto i = 0; i < alphabet.size(); ++i) {
		bitset<8> bitOut(alphabet[i].getChar());
		outfile.write((char*) &bitOut, 1); // char representation
		bitset<8> sizeOut(alphabet[i].getCode().size());
		outfile.write((char*) &sizeOut, 1); // codeword size
		bitset<8> codeOut(alphabet[i].getCode());
		outfile.write((char*) &codeOut, 1); // codeword
	}

	// output content to new file
	for(auto i = 0; i < out.size(); i+=8) {
		bitset<8> bitOut(out.substr(i, 8));
		outfile.write((char*) &bitOut, 1);
	}

	outfile.close();

	return 0;
}
