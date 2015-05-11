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

	// size of key
	int bytesRequiredForKeyLength;
	if(alphabet.size() >= 256) {
		bytesRequiredForKeyLength = 2;
	}
	else {
		bytesRequiredForKeyLength = 1;
	}
	bitset<8> keyLengthBytes(bytesRequiredForKeyLength);
	key += keyLengthBytes.to_string<char,std::string::traits_type,std::string::allocator_type>();

	auto longestCodeWordLength = alphabet.back().getCode().size();
	bitset<8> longestCodeLength(longestCodeWordLength);
	key += longestCodeLength.to_string<char,std::string::traits_type,std::string::allocator_type>();

	auto keyLength = alphabet.size() * 3 * 8;
	if(longestCodeWordLength > 8) {
		keyLength = alphabet.size() * 4 * 8;
	}
	if(bytesRequiredForKeyLength == 1) {
		bitset<8> keySize(keyLength);
		key += keySize.to_string<char,std::string::traits_type,std::string::allocator_type>();
	}
	else {
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
		else {//if(longestCodeWordLength > 8) {
			bitset<16> codeOut(alphabet[i].getCode());
			letterInfo = bitOut.to_string<char,std::string::traits_type,std::string::allocator_type>() + sizeOut.to_string<char,std::string::traits_type,std::string::allocator_type>() + codeOut.to_string<char,std::string::traits_type,std::string::allocator_type>();
		}
		key += letterInfo;
	}
	//cout << key << endl;

	string rawOutput = key + out;
	//cout << rawOutput << endl;

	/*
	//------------------------
	int rem = stoi(key.substr(0,8));
	cout << "remainder: " << key.substr(0,8) << ' ' << rem << endl;
	key = key.substr(8);
	cout << key << endl;

	int keylenbyt = stoi(key.substr(0,8));
	cout << "key length bytes: " << key.substr(0,8) << ' ' << keylenbyt << endl;
	key =	key.substr(8);
	cout << key << endl;

	int longestcode = stoi(key.substr(0,8));
	cout << "longest code word: " << key.substr(0,8) << ' ' << longestcode << endl;
	key = key.substr(8);
	cout << key << endl;

	string keysize;
	if(keylenbyt == 1) {
		keysize = key.substr(0,8);
		cout << "key size: " << key.substr(0,8) << ' ' << keysize << endl;
		key = key.substr(8);
	}
	else if(keylenbyt == 10) {
		keysize = key.substr(0,16);
		cout << "key size: " << key.substr(0,16) << ' ' << keysize << endl;
		key = key.substr(16);
	}
	cout << key << endl;

	string letter, codesize, code, data;
	while(key.size() > 0) {
		letter = key.substr(0,8);
		key = key.substr(8);
		codesize = key.substr(0,8);
		key = key.substr(8);
		if(longestCodeWordLength > 8) {
			code = key.substr(0,16);
			key = key.substr(16);
		}
		else {
			code = key.substr(0,8);
			key = key.substr(8);
		}
		data = letter + codesize + code;
		cout << data << endl;
	}
*/

	//bitset<8> keyBytes(bytesRequiredForKey);
	//key += keyBytes.to_string<char,std::string::traits_type,std::string::allocator_type>();

	//out += "00000000";

	//cout << "Output: " << out << endl;
	// personal uses
	//cout << "File size: " << out.size() + 2*8 + alphabet.size()*3*8 << "bits"
		//<< " ~ " << out.size() + 2 + static_cast<double>(alphabet.size())*3/8
				//<< "bytes" << endl;

//-----------------------------------------------------------------------------

	//// .info file holds key
	fstream outfile;
	outfile.open(fname + ".compressed" , ios::binary | fstream::out);


	//// exta bits at the end due to hard coded 8 bit size
	//bitset<8> sizeRemainder(8 - out.size() % 8);
	//outfile.write((char*) &sizeRemainder, 1);
	//for(auto i = 0; i < sizeRemainder.to_ulong(); ++i) {
		//out += "0";
	//}

	//cout << "remainder: " << sizeRemainder.to_ulong() << endl;

	//bitset<8> keySize(alphabet.size());
	//outfile.write((char*) &keySize, 1);

	//cout << "keySize: " << keySize.to_ulong() * 8 * 3 << endl;

	//// formatting key
	//for(auto i = 0; i < alphabet.size(); ++i) {
		//bitset<8> bitOut(alphabet[i].getChar());
		//outfile.write((char*) &bitOut, 1); // char representation
		//bitset<8> sizeOut(alphabet[i].getCode().size());
		//outfile.write((char*) &sizeOut, 1); // codeword size
		//bitset<8> codeOut(alphabet[i].getCode());
		//outfile.write((char*) &codeOut, 1); // codeword
	//}

	//// output content to new file
	for(auto i = 0; i < rawOutput.size(); i+=8) {
		bitset<8> bitOut(rawOutput.substr(i, 8));
		outfile.write((char*) &bitOut, 1);
		////cout << bitOut << endl;
	}

	outfile.close();

	return 0;
}
