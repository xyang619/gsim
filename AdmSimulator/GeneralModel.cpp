/*
 * GeneralModel.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: young
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include "GeneralModel.h"

GeneralModel::GeneralModel(const std::string & file, int gen, int nanc):gen(gen), nanc(nanc) {
	readParam(file, gen, nanc);
	if (isValidNhap(nanc) && isValidNe(gen) && isValidProp(gen, nanc))
		pop =  Population();
	else
		exit(1);
}

GeneralModel::GeneralModel(const GeneralModel & gm):gen(gm.gen), nanc(gm.nanc){
    nhaps = new int[nanc];
    for (int i = 0; i < nanc; ++i){
        nhaps[i] = gm.nhaps[i];
    }
    nes = new int[gen];
    for (int i = 0; i < gen; ++i){
        nes[i] = gm.nes[i];
    }
    prop = new double *[gen];
    for (int i = 0; i < gen; ++i) {
        prop[i] = new double[nanc];
    }
    for (int i = 0; i < gen; ++i) {
        for(int j = 0; j < nanc; ++j){
            prop[i][j] = gm.prop[i][j];
        }
    }
    pop = gm.pop;
}

GeneralModel & GeneralModel::operator=(const GeneralModel & gm){
    if (this != &gm) {
        gen = gm.gen;
        nanc = gm.nanc;
        if (nhaps != NULL)
            delete[] nhaps;
        if (nes != NULL)
            delete[] nes;
        if (prop != NULL) {
            for(int i = 0; i < gen; ++i){
                delete[] prop[i];
            }
            delete [] prop;
        }
        nhaps = new int[nanc];
        for (int i = 0; i < nanc; ++i){
            nhaps[i] = gm.nhaps[i];
        }
        nes = new int[gen];
        for (int i = 0; i < gen; ++i){
            nes[i] = gm.nes[i];
        }
        prop = new double *[gen];
        for (int i = 0; i < gen; ++i) {
            prop[i] = new double[nanc];
        }
        for (int i = 0; i < gen; ++i) {
            for(int j = 0; j < nanc; ++j){
                prop[i][j] = gm.prop[i][j];
            }
        }
        pop = gm.pop;
    }
    return *this;

}

bool GeneralModel::isValidNhap(int nanc) const{
    for (int i = 0; i < nanc; ++i) {
        if (nhaps[i] <= 0) {
            std::cerr << "Error: The size of ancestral haplotypes must be positive" << std::endl;
            return 0;
        }
    }
    return 1;
}

bool GeneralModel::isValidNe(int gen) const {
	for (int i = 0; i < gen; ++i) {
		if (nes[i] <= 0) {
            std::cerr << "Error: Effective population size must be positive" << std::endl;
			return 0;
		}
	}
	return 1;
}

bool GeneralModel::isValidProp(int gen, int nanc) const {
	for (int i = 0; i < gen; ++i) {
		double tsum = 0;
		for (int j = 0; j < nanc; ++j) {
			if (prop[i][j] < 0 || prop[i][j] > 1) {
                std::cerr << "Error: Admixture proportion must be between 0 and 1" << std::endl;
				return 0;
			}
			tsum += prop[i][j];
		}
		if (i == 0 && tsum != 1.0) {
            std::cerr << "Error: Then admixture proportion of initial generation must sum to 1" << std::endl;
			return 0;
		}
		if (tsum > 1) {
            std::cerr << "Error: The sum of admixture proportion in generation " << i << " larger than 1" << std::endl;
			return 0;
		}
	}
	return 1;
}

//void GeneralModel::readParam(const string & file, int gen, int nanc) {
//	ifstream fin(file.c_str());
//	if (!fin.is_open()) {
//		cout << "Can't open model file " << file << endl;
//		exit(1);
//	}
//	nhaps = new int[nanc];
//	for (int j = 0; j < nanc; ++j)
//		fin >> nhaps[j];
//	nes = new int [gen];
//    prop = new double * [gen];
//    for(int i = 0; i < gen; ++i){
//        prop[i] = new double[nanc];
//    }
//	for (int i = 0; i < gen; ++i) {
//        fin >> nes[i];
//		for (int j = 0; j < nanc; ++j) {
//            fin >> prop[i][j];
//		}
//	}
//	fin.close();
//}

void GeneralModel::readParam(const std::string & file, int gen, int nanc) {
    std::ifstream fin(file.c_str());
    if (!fin.is_open()) {
        std::cout << "Can't open model file " << file << std::endl;
        exit(1);
    }
    nhaps = new int[nanc];
    nes = new int [gen];
    prop = new double * [gen];
    for(int i = 0; i < gen; ++i){
        prop[i] = new double[nanc];
    }
    bool ancSet = 0;
    bool isStart = 0;
    int g = 0;
    std::string line;
    while(getline(fin, line)){
        //pure comments
        if (line.size() > 0 && line.at(0)=='#') {
            continue;
        }
        //remove comments
        size_t pos = line.find('#');
        if ( pos != std::string::npos) {
            line = line.substr(0,pos);
        }
        if (line.substr(0,2) == "//") {
            isStart = 1;
            continue;
        }
        if (isStart && !ancSet) {
            std::cerr << "Uninitialized number of ancestral population haplotypes" << std::endl;
            fin.close();
            exit(1);
        }
        std::istringstream iss(line);
        if (!isStart && iss) {
            for (int i = 0; i < nanc; ++i) {
                iss >> nhaps[i];
            }
            ancSet = 1;
        }
        else if (isStart && g < gen) {
            iss >> nes[g];
            for (int i = 0; i < nanc; ++i) {
                iss >> prop[g][i];
            }
            ++g;
        }
        if (g >= gen) {
            break;
        }
    }
    fin.close();
}

void GeneralModel::evolve(int gen, int nanc, double len) {
	for (int i = 0; i < gen; ++i) {
		int numbIndsPrev = 0;
		int curNe = nes[i];
		int numbInds[nanc];
		int sumNumbInds = 0;
		for (int j = 0; j < nanc; ++j) {
			numbInds[j] = (int) (curNe * prop[i][j]);
			sumNumbInds += numbInds[j];
		}
		// prepare individuals in current generation
		numbIndsPrev = curNe - sumNumbInds;
        std::vector<ChromPair> indsCur;
		if (numbIndsPrev > 0) {
			indsCur = pop.sample(numbIndsPrev);
		}
		for (int j = 0; j < nanc; j++) {
			if (numbInds[j] > 0) {
				for (int k = 0; k < numbInds[j]; k++) {
                    std::vector<Segment> segs1;
					int lab = (j + 1) * 10000 + rand() % nhaps[j];
					segs1.push_back(Segment(0, len, lab));
					Chrom chr1(segs1);
                    std::vector<Segment> segs2;
					lab = (j + 1) * 10000 + rand() % nhaps[j];
					segs2.push_back(Segment(0, len, lab));
					Chrom chr2(segs2);
					indsCur.push_back(ChromPair(chr1, chr2));
				}
			}
		}
		Population tmpPop(curNe, indsCur);
		pop = tmpPop.evolve(tmpPop.getNe());
	}

}

const int * GeneralModel::getNhaps() const{
    return nhaps;
}

Population GeneralModel::getPop() const {
	return pop;
}

GeneralModel::~GeneralModel() {
    if (nhaps != NULL)
        delete[] nhaps;
    if (nes != NULL)
        delete[] nes;
    if (prop != NULL) {
        for(int i = 0; i < gen; ++i){
            delete[] prop[i];
        }
        delete [] prop;
    }
}

