/*
 * GeneralModel.h
 *
 *  Created on: Nov 9, 2014
 *      Author: young
 */

#ifndef GENERALMODEL_H_
#define GENERALMODEL_H_

#include <string>
#include "Population.h"

class GeneralModel {
private:
    int gen;
    int nanc;
	int *nhaps;
	int *nes;
	double **prop;
	Population pop;
public:
    GeneralModel(const std::string & file, int gen, int nanc);
    GeneralModel(const GeneralModel & gm);
    GeneralModel & operator=(const GeneralModel & gm);
    bool isValidNhap(int nanc) const;
	bool isValidNe(int gen) const;
	bool isValidProp(int gen, int nanc) const;
    void readParam(const std::string & file, int gen, int nanc);
    int getGen() const;
    int getNanc() const;
	void evolve(int gen, int nanc, double len);
	const int * getNhaps() const;
	Population getPop() const;
	virtual ~GeneralModel();
};

#endif /* GENERALMODEL_H_ */
