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

	// first byte in file contains info on the xtra bits at the end of the file
	bitset<8> rem(content.substr(0,8));
	int remainder = rem.to_ulong();
	content = content.substr(8, content.size() - remainder - 8);

	// second byte in file contains info about the size of the key
	bitset<8> keyS(content.substr(0,8));
	int keySize = keyS.to_ulong();
	content = content.substr(8);

	// peie together the key info is set up in 3's
	vector<letter> key;
	for(auto i = 0; i < keySize; i+=24) {
		bitset<8> x(content.substr(i,8)); // char representation
		bitset<8> y(content.substr(i+8,8)); // code length
		bitset<8> z(content.substr(i+8+8,8)); // code word
		// put data in letter structure
		letter member((char) x.to_ulong(), z.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(8-y.to_ulong(), y.to_ulong()));
		// put letter in key vector
		key.push_back(member);
	}

	printAlphabet(key);

	content = content.substr(keySize, content.size() - keySize);
	string decodedContent = decode(content, key);

	infile.close();

// separate input from output
//------------------------------------------------------------------------------

	// get true name of file ... not the path
	string newName = argv[1];
	newName = getFilename(newName);

	fstream outfile;
	outfile.open(newName + ".decompressed", fstream::out);

	outfile << decodedContent; // output content to file

	outfile.close();

	return 0;
}
