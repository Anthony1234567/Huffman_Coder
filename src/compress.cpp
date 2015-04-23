#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct letter {
	char character;
	int frequency;
	double probability;
	string encoding;
	vector<char> otherChars;
	letter(char c) : character(c), frequency(1) {}
	//letter(char larger, double combinedProbability, char smaller, vector<char> largerOtherChars, vector<char> smallerOtherChars)
	//: character(larger), probability(combinedProbability) {
		//for(auto i : smallerOtherChars) {
			//otherChars.push_back(i);
		//}
		//for(auto i : largerOtherChars) {
			//otherChars.push_back(i);
		//}
		//otherChars.push_back(smaller);
	//}
	letter(const letter& old) : character(old.character), frequency(old.frequency), probability(old.probability), encoding(old.encoding) {}
	void incrementFreq() {frequency += 1;}
	void setProbability(int total) {probability = static_cast<double>(frequency) / total;}
	//letter add(letter& larger, letter& smaller) {
		//return letter(larger.character, larger.probability + smaller.probability, smaller.character, larger.otherChars, smaller.otherChars);
	//}
};

class alphabet {
	private:
		vector<letter> members;
	public:
		void addmember(letter l) {
			members.push_back(l);
		}
		int alphabetsize() {
			return members.size();
		}
		bool seen(char l) {
			for(auto&& i : members) {
				if(l == i.character) {
					i.incrementFreq();
					return true;
				}
			}
			return false;
		}
		int count() {
			int counter = 0;
			for(auto i = 0; i < members.size(); ++i) {
				counter += members[i].frequency;
			}
			return counter;
		}
		void calculateProbabilities() {
			for(auto&& i : members) {
				i.setProbability(count());
			}
		}
		void printalphabet() {
			calculateProbabilities();
			for(auto i : members) {
				cout << "Symbol: ";
				if(i.character == '\n')	{
					cout << "\\n ";
				} else if(i.character == ' ') {
					cout << "space ";
				} else {cout << i.character << ' ';}
				cout << "Frequency: " << i.frequency << "/" << count() << ' '
					<< "Probability: " << i.probability << ' '
					<< "Encoding: " << i.encoding << endl;
			}
		}
		void reverseSort() {
			int tempfreq = 0;
			char tempchar;
			auto i = 0;
			for(auto j = 1; j < members.size(); j++) {
				tempfreq = members[j].frequency;
				tempchar = members[j].character;
				i = j - 1;
				while(i >= 0 && members[i].frequency < tempfreq){
					members[i + 1].frequency = members[i].frequency;
					members[i + 1].character = members[i].character;
					i = i - 1;
					members[i + 1].frequency = tempfreq;
					members[i + 1].character = tempchar;
				}
			}
		}
		void encode() {
			if(members.size() == 1) {
				return;
			}
			if(members[alphabetsize() - 1].probability	>= members[alphabetsize() - 2].probability) {
				members[alphabetsize() - 1].encoding = '1' + members[alphabetsize() - 1].encoding;

				members[alphabetsize() - 2].encoding = '0' + members[alphabetsize() - 2].encoding;
			}

		}
};

int main(int argc, char **argv) {
	std::fstream file;
	file.open(argv[1]);

	char viewing;
	string content;
	alphabet m;

	while(file.get(viewing)) {
		content += viewing;
		letter v(viewing);
		if(!m.seen(viewing)) {
			m.addmember(v);
		}
	}
	m.reverseSort();
	cout << "Content: " << endl << content;
	cout << endl;
	m.printalphabet();
	file.close();
	return 0;
}
