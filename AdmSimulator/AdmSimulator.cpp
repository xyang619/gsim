/*
 * AdmSimulator.cpp
 *
 *  Created on: Dec 11, 2014
 *      Author: young
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <map>
#include <vector>
#include "GeneralModel.h"

using namespace std;

static const string version = "1.0.0";

void help();
int findPos(const vector<double> & poss, double pos);
string copySeq(const vector<double> & poss, const string & seq, double start, double end);

int main(int argc, char **argv) {
	if (argc > 1 && (string(argv[1]) == "-h" || string(argv[1]) == "--help")) {
		help();
		exit(0);
	}

	if (argc < 5) {
		cerr << "Need more arguments than provided, use -h/--help to get more help" << endl;
		exit(1);
	}

	int gen = 1;
	int nanc = 2;
    int nsample = 10;
    unsigned seed = 0;
    bool givenSeed = 0;
	double len = 1.0;
	string file = "";
	string prefix = "";
	string output = "";

	for (int i = 0; i < argc; ++i) {
		string arg(argv[i]);
		if (arg == "-h" || arg == "--help") {
			help();
			exit(0);
		} else if (arg == "-f" || arg == "--file") {
			file = string(argv[++i]);
		} else if (arg == "-g" || arg == "--gen") {
			gen = atoi(argv[++i]);
		} else if (arg == "-k" || arg == "--nanc") {
			nanc = atoi(argv[++i]);
		} else if (arg == "-l" || arg == "--len") {
			len = atof(argv[++i]);
		} else if (arg == "-n" || arg == "--samp") {
			nsample = atoi(argv[++i]);
		} else if (arg == "-i" || arg == "--input") {
			prefix = string(argv[++i]);
		} else if (arg == "-o" || arg == "--output") {
			output = string(argv[++i]);
		} else if (arg == "-s" || arg == "--seed") {
			seed = unsigned(atol(argv[++i]));
			givenSeed = 1;
		}
	}

	if (file.size() == 0) {
		cerr << "Model description file must be specified" << endl;
		abort();
	}
	if (prefix.size() == 0) {
		cerr << "Input file must be specified" << endl;
		abort();
	}
	if (output.size() == 0) {
		cerr << "Warning, output prefix using default" << endl;
        output = "output";
	}
	if (!givenSeed) {
		seed = unsigned(time(0));
	}
	srand(seed);
	//output the model description
	cerr << "========================================================" << endl;
	cerr << "Arguments summary:" << endl;
	cerr << "generation: " << gen << endl;
	cerr << "number of ancestral populations: " << nanc << endl;
	cerr << "chromosome length: " << len << endl;
	cerr << "number of samples: " << nsample << endl;
	cerr << "model file: " << file << endl;
	cerr << "input file: " << prefix << ".hap " << prefix << ".map" << endl;
	cerr << "output prefix: " << output << endl;
	cerr << "random seed: " << seed << endl;
	cerr << "========================================================" << endl;

	GeneralModel gm(file, gen, nanc);
	gm.evolve(gen, nanc, len);
	vector<ChromPair> sample = gm.getPop().sample(nsample);
	string mapfile = prefix + ".map";
	ifstream mfin(mapfile.c_str());
	if (!mfin.is_open()) {
		cerr << "Can't open file " << mapfile << endl;
		abort();
	}
	vector<double> poss;
	while (mfin) {
		double tmp;
		mfin >> tmp;
		poss.push_back(tmp);
	}
	//here is tricky, even when last line is empty, still add 0 to the vector, remove that one
    poss.pop_back();
	mfin.close();
	string ancfile = prefix + ".hap";
	ifstream hfin(ancfile.c_str());
	if (!hfin.is_open()) {
		cerr << "Can't open file " << ancfile << endl;
		abort();
	}
	map<int, vector<string> > anchaps;
	for (int j = 0; j < nanc; ++j) {
		vector<string> haps;
		anchaps[j + 1] = haps;
	}
	string line;
	int key = 1;
	int nInd = 0;
	int nAnc = gm.getNhaps()[0];
	while (getline(hfin, line)) {
		if (nInd < nAnc) {
			anchaps.at(key).push_back(line);
			++nInd;
		} else {
			++key;
			nInd = 0;
			nAnc = gm.getNhaps()[key - 1];
			anchaps.at(key).push_back(line);
			++nInd;
		}
	}
	hfin.close();
	//prepare output
	string segfile = string(output) + ".seg";
	string hapfile = string(output) + ".hap";
	ofstream segout(segfile.c_str());
	ofstream hapout(hapfile.c_str());
	if (!segout.is_open() || !hapout.is_open()) {
		cerr << "Can't open file " << segfile << " or " << hapfile << endl;
		abort();
	}
	//cout << "//admixed haplotypes" << endl;
	for (int i = 0; i < nsample; ++i) {
		ChromPair cp = sample.at(i);
		for (int j = 1; j < 3; ++j) {
			Chrom chr = cp.getChrom(j);
			int nseg = chr.getNumSegments();
			string outStr = "";
			for (int k = 0; k < nseg; ++k){
				Segment seg = chr.getSegment(k);
				int key = seg.getLabel() / 10000;
				int index = seg.getLabel() % 10000;
				double start = seg.getStart();
				double end = seg.getEnd();
                outStr = outStr + copySeq(poss, anchaps.at(key).at(index), start, end);
			}
			hapout << outStr << endl;

			chr.smooth();
			nseg = chr.getNumSegments();
			for (int k = 0; k < nseg; ++k) {
				Segment seg = chr.getSegment(k);
				double start, end;
				int label;
				start = seg.getStart();
				end = seg.getEnd();
				label = seg.getLabel() / 10000;
				segout << setprecision(8) << setw(16) << start << setw(16) << end << setw(4) << label << endl;
			}
		}
	}
	segout.close();
	hapout.close();
	return 0;
}

void help() {
    cout << "====================================================================================" << endl;
    cout << "AdmSimulator v" << version << endl;
	cout << "Description: A forward time simulator for generalized model" << endl;
	cout << "Arguments:" << endl;
	cout << "	-h/--help	print help message[optional]" << endl;
	cout << "	-f/--file	model description file [required]" << endl;
    cout << "	-i/--input	prefix of input file [required]" << endl;
	cout << "	-g/--gen	generation since admixture [optional, default=1]" << endl;
	cout << "	-k/--nanc	number of ancestral populations [optional, default=2]" << endl;
	cout << "	-l/--len	length of chromosome to be simulated [optional, default=1]" << endl;
	cout << "	-n/--samp	number of individuals to be sampled [optional, default=10]" << endl;
	cout << "	-o/--output	prefix of output file [optional, default=output]" << endl;
	cout << "	-s/--seed	seed of random generator [optional, default=current time]" << endl;
    cout << "====================================================================================" << endl;
}

int findPos(const vector<double> & poss, double pos) {
	int left = 0;
	int right = static_cast<int>(poss.size());
	if (pos <= poss.front())
		return left;
	if (pos > poss.back())
		return right;
	int mid = (left + right + 1) / 2;
	while (left < right) {
		if (pos > poss.at(mid))
			left = mid;
		else
			right = mid - 1;
		mid = (left + right + 1) / 2;
	}
	return left;
}

string copySeq(const vector<double> & poss, const string & seq, double start, double end) {
	int left, right;
	left = findPos(poss, start);
	right = findPos(poss, end);
	return seq.substr(left, right - left);
}

