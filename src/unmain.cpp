#include <iostream>
#include <fstream>
#include <bitset>

using namespace std;

int main(int argc, const char** argv) {
	fstream infile;
	infile.open(argv[1]);

	infile.close();
}
