#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

int main(int argc, const char** argv) {
	fstream infile;
	infile.open(argv[1], fstream::binary | fstream::in);

	char viewing;
	while(infile.get(viewing)) {
		bitset<8> input(viewing);
		cout << input;
	}
	cout << endl;

	infile.close();
}
