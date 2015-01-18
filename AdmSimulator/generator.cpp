/*
 * generator.cpp
 *
 *  Created on: Sep 25, 2014
 *  Author: xiong_yang
 *  generate a series of position between 0 and length, and generate a sequence of given character sets
 *  It's bad to use srand many times, it's said srand should only call once in each application
 */
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "generator.h"

char random(char Set[], int len) {
	return Set[rand() % len];
}

vector<double> genPos(long size, double length) {
	vector<double> pos;
	for (int i = 0; i < size; ++i) {
		pos.push_back(length * ((1.0 * rand()) / RAND_MAX));
	}
	sort(pos.begin(), pos.end());
	return pos;
}

char *genSeq(long size, char Set[], int len) {
	char *seq;
	seq = new char[size + 1];
	for (int i = 0; i < size; ++i) {
		seq[i] = random(Set, len);
	}
	seq[size] = '\0';
	return seq;
}

//int main(int argc, char ** argv) {
//	long size = 100;
//	if (argc > 1) {
//		size = atol(argv[1]);
//	} else {
//		cerr << "Warning, using default length 100" << endl;
//		cerr << "Usage: generator sequence_length" << endl;
//	}
//	srand (time(NULL));
//	char set[] = { 'A', 'C', 'G', 'T' };
//	vector<double> p = genPos(size, 1.0);
//	char *s = genSeq(size, set, 4);
//	for (int i = 0; i < size; ++i)
//		cout << p[i] << " ";
//	cout << endl;
//	cout << s << endl;
//}
