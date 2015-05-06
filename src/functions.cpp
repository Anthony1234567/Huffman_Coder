#include "functions.h"

int count(const vector<letter> &members) {
	int counter = 0;
	for(auto i = 0; i < members.size(); ++i) {
		counter += members[i].getFreq();
	}
	return counter;
}

void calculateProbabilities(vector<letter> &members) {
	for(auto&& i : members) {
		i.setProbability(count(members));
	}
}

void printAlphabet(const vector<letter> &members) {
	for(auto i : members) {
	cout << "Symbol: ";
		if(i.getChar() == '\n') {
			cout << "\\n ";
		}
		else if(i.getChar() == ' ') {
			cout << "space ";
		}
		else {
			cout << i.getChar() << ' ';
		}
		cout << "Frequency: " << i.getFreq() << "/" << count(members) << ' '
			<< "Probability: " << i.getProb() << ' '
			<< "Encoding: " << i.getCode() << endl;
	}
}

void huffman(vector<letter> &members) {
	vector<vector<letter>> fusionTrei;
	fusionTrei.push_back(members);
	for(auto i = 0; i < fusionTrei[0].size() - 1; ++i) {
		vector<letter> iMembers;
		for(auto j = 0; j < fusionTrei[i].size() - 2; ++j) {
			iMembers.push_back(fusionTrei[i][j]);
		}
		iMembers.push_back(fusionTrei[i][fusionTrei[i].size() - 2].fuse(fusionTrei[i][fusionTrei[i].size() - 1]));
		sort(iMembers.begin(), iMembers.end());
		fusionTrei.push_back(iMembers);
	}

	for(auto i = 0; i < fusionTrei.size(); ++i) {
		for(auto j = 0; j < fusionTrei[i].size(); ++j) {
			if(fusionTrei[i][j].getChar() != '\0') {
				for(auto k = 0; k < members.size(); ++k) {
					if(members[k].getChar() == fusionTrei[i][j].getChar()) {
						members[k].bootlegencode(fusionTrei[i][j].getCode());
					}
				}
			}
		}
	}
}

bool seen(const char &l, vector<letter> &members) {
	for(auto&& i : members) {
		if(l == i.getChar()) {
			i.incrementFreq();
			return true;
		}
	}
	return false;
}

double calcAvgWordLen(vector<letter> members) {
	double len = 0;
	for(auto i = 0; i < members.size(); ++i) {
		len += members[i].getCode().size() * members[i].getProb();
	}
	return len;
}
