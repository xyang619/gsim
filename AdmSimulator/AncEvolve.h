//
//  AncEvolve.h
//  AdmSimulator
//
//  Created by young on 1/20/15.
//  Copyright (c) 2015 young. All rights reserved.
//

#ifndef __AdmSimulator__AncEvolve__
#define __AdmSimulator__AncEvolve__

#include <vector>
#include "Chrom.h"

class AncPop{
public:
    AncPop(int size, double length);
    AncPop(const std::vector<Chrom> & haplos);
    void evolve(int gen);
    std::vector<Chrom> & getHaplos();
    ~AncPop();
private:
    std::vector<Chrom> haplos;
};

#endif /* defined(__AdmSimulator__AncEvolve__) */
