#ifndef __Huffman_Coder__functions__
#define __Huffman_Coder__functions__

#include <iostream>
#include <vector>
#include "letter.h"

using namespace std;

int count(const vector<letter> &);
void calculateProbabilities(vector<letter> &);
void printAlphabet(const vector<letter> &);
void huffman(vector<letter> &);
bool seen(const char &, vector<letter> &);
double calcAvgWordLen(vector<letter>);

#endif
