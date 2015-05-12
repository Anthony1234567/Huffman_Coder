#include <iostream>
#include <fstream>
#include <bitset>

#include "letter.h"
#include "functions.h"

using namespace std;

int main(int argc, const char** argv) {
	// info gathering
	fstream infile;
	infile.open(argv[1], fstream::binary | fstream::in);

	// grab characters in file
	char viewing;
	string content;
	while(infile.get(viewing)) {
		bitset<8> input(viewing);
		content += input.to_string<char,std::string::traits_type,std::string::allocator_type>();
	}

	bitset<8> remainder(content.substr(0,8));
	auto fillSpace = 0;
	if(remainder.to_ulong() != 0) {
		fillSpace = 8 - remainder.to_ulong();
	}
	content = content.substr(8);

	bitset<8> longestCode(content.substr(0,8));
	auto longestCodeWordLength = longestCode.to_ulong();
	content = content.substr(8);

	bitset<8> keylenbyt(content.substr(0,8));
	auto bytesForKeyLength = keylenbyt.to_ulong();
	content = content.substr(8);

	auto keySizeInBits = 0;
	if(bytesForKeyLength == 1) {
		bitset<8> keySize(content.substr(0,8));
		keySizeInBits = keySize.to_ulong();
		content = content.substr(8);
	}
	else if(bytesForKeyLength == 2) {
		bitset<16> keySize(content.substr(0,16));
		keySizeInBits = keySize.to_ulong();
		content = content.substr(16);
	}
	string key = content.substr(0,keySizeInBits);
	content = content.substr(keySizeInBits);
	content = content.substr(0,content.size() - fillSpace);

	char symbol;
	int size;
	string codeword;
	vector<letter> alphabet;
	while(key.size() > 0) {
		bitset<8> character(key.substr(0,8));
		key = key.substr(8);
		symbol = (char) character.to_ulong();
		bitset<8> codeSize(key.substr(0,8));
		key = key.substr(8);
		size =  codeSize.to_ulong();
		if(longestCodeWordLength > 8) {
			bitset<16> code(key.substr(0,16));
			key = key.substr(16);
			codeword = code.to_string<char,std::string::traits_type,std::string::allocator_type>();
		}
		else if(longestCodeWordLength <= 8) {
			bitset<8> code(key.substr(0,8));
			key = key.substr(8);
			codeword = code.to_string<char,std::string::traits_type,std::string::allocator_type>();
		}
		letter member(symbol,codeword.substr(codeword.size() - size));
		alphabet.push_back(member);
	}

	cout << "Converting code back into original content..." << endl;
	string decodedContent = decode(content, alphabet);

	infile.close();

//------------------------------------------------------------------------------

	// get true name of file ... not the path
	string newName = argv[1];
	newName = getFilename(newName);

	fstream outfile;
	outfile.open(newName, fstream::out);

	outfile << decodedContent << '\n'; // output content to file

	outfile.close();

	return 0;
}
