/*
 * Population.h
 *
 *  Created on: Sep 28, 2014
 *      Author: xiong_yang
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "ChromPair.h"

class Population {
private:
	int ne;
    std::vector<ChromPair> indivs;
public:
	Population();
    Population(int ne, const std::vector<ChromPair> & indivs);
	int getNe();
    std::vector<ChromPair> getIndivs() const;
	void updateNe();
	void addIndiv(ChromPair & indiv);
	Population & evolve(int nec);
    std::vector<ChromPair> sample(int nsamp) const;
    std::vector<Population> split(double prop);
	virtual ~Population();
};

#endif /* POPULATION_H_ */
