#include <iostream>
#include <fstream>
#include <bitset>

#include "letter.h"
#include "functions.h"

using namespace std;

int main(int argc, const char** argv) {
	// info gathering
	fstream infile2;
	infile2.open(argv[2], fstream::binary | fstream::in);

	char viewing2;
	string content;
	while(infile2.get(viewing2)) {
		bitset<8> input(viewing2);
		content += input.to_string<char,std::string::traits_type,std::string::allocator_type>();
	}

	vector<letter> key;

	for(auto i = 0; i < content.size(); i+=24) {
		bitset<8> x(content.substr(i,8));
		bitset<8> y(content.substr(i+8,8));
		bitset<8> z(content.substr(i+8+8,8));
		letter member((char) x.to_ulong(), z.to_string<char,std::string::traits_type,std::string::allocator_type>().substr(8-y.to_ulong(), y.to_ulong()));
		key.push_back(member);
	}

	printAlphabet(key);

	infile2.close();

	// actual decompression
	fstream infile;
	infile.open(argv[1], fstream::binary | fstream::in);
	string content2;
	char viewing;
	while(infile.get(viewing)) {
		bitset<8> input(viewing);
		content2 += input.to_string<char,std::string::traits_type,std::string::allocator_type>();
	}
	cout << content2 << endl;

	int index = 0;
	//while(content2.size() > 0) {
		//content[index];
	//}

	infile.close();
}
