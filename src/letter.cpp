#include "letter.h"

letter::letter(char c)
	: character(c), frequency(1)
{}

letter::letter(double p)
	: character('\0'), probability(p)
{}

const char letter::getChar() const {
	return character;
}

const int letter::getFreq() const {
	return frequency;
}

const double letter::getProb() const {
	return probability;
}

const std::string letter::getCode() const {
	return encoding;
}

void letter::incrementFreq() {
  frequency += 1;
}

void letter::decrimentFreq() {
  frequency -= 1;
}

void letter::setProbability(const int &total) {
  probability = static_cast<double>(frequency) / total;
}

void letter::encode(char num) {
	encoding = num + encoding;
}

void letter::code() {
	for(auto i = 0; i < otherCharsLarger.size(); ++i) {
		otherCharsLarger[i]->encode('1');
	}
	for(auto i = 0; i < otherCharsSmaller.size(); ++i) {
		otherCharsSmaller[i]->encode('0');
	}
}

letter letter::fuse(letter &otherLetter) {
	letter product(probability + otherLetter.probability);

	product.otherCharsLarger.push_back(this);
	for(auto i = 0; i < otherCharsLarger.size(); ++i) {
		product.otherCharsLarger.push_back(otherCharsLarger[i]);
	}
	for(auto i = 0; i < otherCharsSmaller.size(); ++i) {
		product.otherCharsLarger.push_back(otherCharsSmaller[i]);
	}

	product.otherCharsSmaller.push_back(&otherLetter);
	for(auto i = 0; i < otherLetter.otherCharsLarger.size(); ++i) {
		product.otherCharsSmaller.push_back(otherLetter.otherCharsLarger[i]);
	}
	for(auto i = 0; i < otherLetter.otherCharsSmaller.size(); ++i) {
		product.otherCharsSmaller.push_back(otherLetter.otherCharsSmaller[i]);
	}

	product.code();

	return product;
}

bool letter::operator<(const letter& compareTo) const {
	return probability > compareTo.probability;
}

void letter::bootlegencode(std::string real) {
	encoding = real;
}
