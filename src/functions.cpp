#include "functions.h"

std::string getFilename(const std::string &path) {
	std::string filename;
  unsigned found = path.find_last_of(".");
  filename = path.substr(0,found);
  found = filename.find_last_of("/");
  filename = filename.substr(found+1);
  return filename;
}

int count(const std::vector<letter> &members) {
	int counter = 0;
	for(auto i = 0; i < members.size(); ++i) {
		counter += members[i].getFreq();
	}
	return counter;
}

void calculateProbabilities(std::vector<letter> &members) {
	for(auto&& i : members) {
		i.setProbability(count(members));
	}
}

void printAlphabet(const std::vector<letter> &members) {
	for(auto i : members) {
		std::cout << "Symbol: ";
		if(i.getChar() == '\n') {
			std::cout << "\\n ";
		}
		else if(i.getChar() == '\t') {
			std::cout << "\\t ";
		}
		else if(i.getChar() == '\v') {
			std::cout << "\\v ";
		}
		else if(i.getChar() == '\b') {
			std::cout << "\\b ";
		}
		else if(i.getChar() == '\r') {
			std::cout << "\\r ";
		}
		else if(i.getChar() == '\f') {
			std::cout << "\\f ";
		}
		else if(i.getChar() == '\a') {
			std::cout << "\\a ";
		}
		else if(i.getChar() == '\\') {
			std::cout << "\\ ";
		}
		else if(i.getChar() == '\?') {
			std::cout << "\? ";
		}
		else if(i.getChar() == '\'') {
			std::cout << "\' ";
		}
		else if(i.getChar() == '\"') {
			std::cout << "\" ";
		}
		else if(i.getChar() == '\0') {
			std::cout << "\\0 ";
		}
		else if(i.getChar() == ' ') {
			std::cout << "space ";
		}
		else {
			std::cout << i.getChar() << ' ';
		}
		std::cout << "Frequency: " << i.getFreq() << "/" << count(members) << ' '
			<< "Probability: " << i.getProb() << ' '
			<< "Encoding: " << i.getCode() << std::endl;
	}
}

void huffman(std::vector<letter> &members) {
	std::vector<std::vector<letter>> fusionTrei;
	fusionTrei.push_back(members);
	for(auto i = 0; i < fusionTrei[0].size() - 1; ++i) {
		std::vector<letter> iMembers;
		for(auto j = 0; j < fusionTrei[i].size() - 2; ++j) {
			iMembers.push_back(fusionTrei[i][j]);
		}
		iMembers.push_back(fusionTrei[i][fusionTrei[i].size() - 2].fuse(fusionTrei[i][fusionTrei[i].size() - 1]));
		sort(iMembers.begin(), iMembers.end());
		fusionTrei.push_back(iMembers);
	}
	for(auto i = 0; i < fusionTrei.size(); ++i) {
		for(auto j = 0; j < fusionTrei[i].size(); ++j) {
			if(fusionTrei[i][j].getFreq() != -1) {
				for(auto k = 0; k < members.size(); ++k) {
					if(members[k].getChar() == fusionTrei[i][j].getChar()) {
						members[k].bootlegencode(fusionTrei[i][j].getCode());
					}
				}
			}
		}
	}
}

bool seen(const char &l, std::vector<letter> &members) {
	for(auto&& i : members) {
		if(l == i.getChar()) {
			i.incrementFreq();
			return true;
		}
	}
	return false;
}

double calcAvgWordLen(std::vector<letter> &members) {
	double len = 0;
	for(auto i = 0; i < members.size(); ++i) {
		len += members[i].getCode().size() * members[i].getProb();
	}
	return len;
}


std::vector<letter> matching(std::string content, std::vector<letter> key) {
	std::vector<letter> matchingKeywords;
	for(auto i = 0; i < key.size(); ++i) {
		if(key[i].getCode() == content) {
			matchingKeywords.push_back(key[i]);
		}
	}
	return matchingKeywords;
}

std::string decode(std::string &content, std::vector<letter> &key) {
	unsigned long long index = 0;
	std::string decodedContent;
	while(content.size() > 0) {
		//std::cout << index << '/' << content.size() << std::endl;
		if(matching(content.substr(0, index), key).size() == 1) {
			decodedContent += matching(content.substr(0, index), key)[0].getChar();
			content = content.substr(index);
			index = 0;
		}
		index++;
	}
	return decodedContent;
}
