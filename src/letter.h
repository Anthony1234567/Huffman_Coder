#ifndef __Huffman_Coder__letter__
#define __Huffman_Coder__letter__

#include <iostream>
#include <vector>

class letter {
private:
  // data per letter
  char character; // representation of letter
  int frequency; // how many times character is seen in text
  double probability; // times seen / total chars in text
  std::string encoding; // alternate codeword for this letter
	std::vector<letter*> otherCharsLarger; // for combining letters when encoding
	std::vector<letter*> otherCharsSmaller; // for combining letters when encoding
public:
  letter(char); // constructor ~ seen for first time
  letter(double); // data is known (for "fusing" 2 letters)
  letter(char, std::string); // for decoding
  const char getChar() const; // grab character that represents letter in alphabet
  const int getFreq() const; // grab frequency of letter
  const double getProb() const; // grab probability of letter happening
  const std::string getCode() const; // grab codeword corresponding to letter
  void incrementFreq(); // everytime a letter is seen
	void decrimentFreq(); // to remove '\n' node
  void setProbability(const int &); // update probability
	void encode(char); // does the encoding
	void code(); // encodes 1 to front of larger subgroup and 0 to smaller
  letter fuse(letter&); // binary fusion
  bool operator<(const letter&) const; // sorting purposes
	void bootlegencode(std::string); // enough said
};

#endif
