/*
 * Population.cpp
 *
 *  Created on: Sep 28, 2014
 *      Author: xiong_yang
 */

#include <cstdlib>
#include "Population.h"

Population::Population() :
		ne(100) {
    std::vector<ChromPair> indivs;
	this->indivs = indivs;
}

Population::Population(int ne, const std::vector<ChromPair> &indivs) :
    ne(ne), indivs(indivs) {
}

int Population::getNe() {
	updateNe();
	return ne;
}

std::vector<ChromPair> Population::getIndivs() const {
	return indivs;
}

void Population::updateNe() {
	ne = static_cast<int>(indivs.size());
}

void Population::addIndiv(ChromPair & indiv) {
	indivs.push_back(indiv);
}

Population & Population::evolve(int nec) {
    std::vector<ChromPair> indivs_next;
	int size = static_cast<int>(indivs.size());
	for (int i = 0; i < nec; ++i) {
		int ind1, ind2;
		ind1 = rand() % size;
		ind2 = rand() % size;
		while (ind1 == ind2) {
			ind2 = rand() % size;
		}
		int chr_index = rand() % 2;
		Chrom chr1 = indivs.at(ind1).getChrom(chr_index);
		chr_index = rand() % 2;
		Chrom chr2 = indivs.at(ind2).getChrom(chr_index);
		ChromPair cp(chr1, chr2);
		indivs_next.push_back(cp.recombine());
	}
	//return Population(label, ne, indivs_next);
	this->ne = nec;
	this->indivs = indivs_next;
	return *this;
}

std::vector<ChromPair> Population::sample(int nsamp) const {
	int size = static_cast<int>(indivs.size());
    std::vector<ChromPair> samp;
	for (int i = 0; i < nsamp; ++i) {
		samp.push_back(indivs.at(rand() % size));
	}
	return samp;
}

std::vector<Population> Population::split(double prop) {
    std::vector<ChromPair> indivs1;
    std::vector<ChromPair> indivs2;
	int size = static_cast<int>(indivs.size());
	int size1 = static_cast<int>(size * prop);
    std::vector<int> index, index1;
	int i;
	for (i = 0; i < size; ++i) {
		index.push_back(i);
	}
	for (i = 0; i < size1; ++i) {
		int tmp = rand() % index.size();
		index1.push_back(index.at(tmp));
		index.erase(index.begin() + tmp);
	}
	for (i = 0; i < size1; ++i) {
		indivs1.push_back(indivs.at(index1.at(i)));
	}
	int ilen = static_cast<int>(index.size());
	for (i = 0; i < ilen; ++i) {
		indivs2.push_back(indivs.at(index.at(i)));
	}
    std::vector<Population> pops;
	Population pop1(size1, indivs1);
	Population pop2(ne - size1, indivs2);
	pops.push_back(pop1);
	pops.push_back(pop2);
	return pops;
}
Population::~Population() {
	// TODO Auto-generated destructor stub
}
//
//int main(){
//	srand(time(NULL));
//	vector<ChromPair> indivs;
//	vector<Segment> segs;
//	segs.push_back(Segment(0,2,1));
//	Chrom chr1(segs);
//	vector<Segment> segs2;
//	segs2.push_back(Segment(0,2,2));
//	Chrom chr2(segs2);
//	indivs.push_back(ChromPair(chr1,chr1));
//	indivs.push_back(ChromPair(chr2,chr2));
//	Population p(1,indivs);
//	p=p.evolve(100);
//	p=p.evolve(100);
//	p.split(0.2);
//
//	vector<ChromPair> sp = p.sample(5);
//	for(size_t i=0;i<sp.size();++i){
//		sp.at(i).getChrom(1).print();
//		//sp.at(i).getChrom(2).print();
//	}
//}
