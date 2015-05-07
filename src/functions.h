#ifndef __Huffman_Coder__functions__
#define __Huffman_Coder__functions__

#include <iostream>
#include <vector>
#include "letter.h"

std::string getFilename(const std::string &); // get true name of a file
int count(const std::vector<letter> &); // retuens size of alphabet
void calculateProbabilities(std::vector<letter> &); // sets letter probability
void printAlphabet(const std::vector<letter> &); // print alphabet data
void huffman(std::vector<letter> &); // huffman algo for lossless compression
bool seen(const char &, std::vector<letter> &); // if seen increment freq. return true else false
double calcAvgWordLen(std::vector<letter> &); // size of encoding
std::vector<letter> matching(std::string, std::vector<letter>); // find subkey that matces content so far
std::string decode(std::string &, std::vector<letter> &); // decodes content of compressed file

#endif
