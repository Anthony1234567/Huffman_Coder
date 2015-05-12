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

	// fill remaining bits in content (for consistency)
	auto remainder = out.size() % 8;
	if(remainder != 0) {
		for(auto i = 0; i < 8 - remainder; ++i) {
			out += "0";
		}
	}

	// encode key and other info needed in beginning of file
	string key;

	// remainder of output content in bits
	bitset<8> outRemainder(remainder);
	key += outRemainder.to_string<char,std::string::traits_type,std::string::allocator_type>();

	// longest code length
	auto longestCodeWordLength = alphabet.back().getCode().size();
	bitset<8> longestCodeLength(longestCodeWordLength);
	key += longestCodeLength.to_string<char,std::string::traits_type,std::string::allocator_type>();

	// size of key
	int bytesRequiredForKeyLength;
	if(alphabet.size() >= 256 || longestCodeWordLength > 8) {
		bytesRequiredForKeyLength = 2;
	}
	else {
		bytesRequiredForKeyLength = 1;
	}
	bitset<8> keyLengthBytes(bytesRequiredForKeyLength);
	key += keyLengthBytes.to_string<char,std::string::traits_type,std::string::allocator_type>();

	auto keyLength = alphabet.size() * 3 * 8;
	if(longestCodeWordLength > 8) {
		keyLength = alphabet.size() * 4 * 8;
	}
	if(bytesRequiredForKeyLength == 1) {
		bitset<8> keySize(keyLength);
		key += keySize.to_string<char,std::string::traits_type,std::string::allocator_type>();
	}
	else if(bytesRequiredForKeyLength == 2) {
		bitset<16> keySize(keyLength);
		key += keySize.to_string<char,std::string::traits_type,std::string::allocator_type>();
	}

	// formatting key
	for(auto i = 0; i < alphabet.size(); ++i) {
		bitset<8> bitOut(alphabet[i].getChar());
		bitset<8> sizeOut(alphabet[i].getCode().size());
		string letterInfo;
		if(longestCodeWordLength <= 8) {
			bitset<8> codeOut(alphabet[i].getCode());
			letterInfo = bitOut.to_string<char,std::string::traits_type,std::string::allocator_type>() + sizeOut.to_string<char,std::string::traits_type,std::string::allocator_type>() + codeOut.to_string<char,std::string::traits_type,std::string::allocator_type>();
		}
		else {
			bitset<16> codeOut(alphabet[i].getCode());
			letterInfo = bitOut.to_string<char,std::string::traits_type,std::string::allocator_type>() + sizeOut.to_string<char,std::string::traits_type,std::string::allocator_type>() + codeOut.to_string<char,std::string::traits_type,std::string::allocator_type>();
		}
		key += letterInfo;
	}

	// output for outfile
	string rawOutput = key + out;

//-----------------------------------------------------------------------------

	//// .info file holds key
	fstream outfile;
	outfile.open(fname + ".compressed" , ios::binary | fstream::out);

	//// output content to new file
	for(auto i = 0; i < rawOutput.size(); i+=8) {
		bitset<8> bitOut(rawOutput.substr(i, 8));
		outfile.write((char*) &bitOut, 1);
	}

	outfile.close();

	return 0;
}
