//
//  AncEvolve.cpp
//  AdmSimulator
//
//  Created by young on 1/20/15.
//  Copyright (c) 2015 young. All rights reserved.
//

#include "AncEvolve.h"
#include "ChromPair.h"
#include <cstdlib>

AncPop::AncPop(int size, double length){
    std::vector<Chrom> haps;
    for (int i = 0; i < size; ++i) {
        Segment seg(0, length, i);
        std::vector<Segment> segs;
        segs.push_back(seg);
        haps.push_back(Chrom(segs));
    }
    haplos = haps;
}

AncPop::AncPop(const std::vector<Chrom> & haplos):haplos(haplos){
    
}

AncPop::~AncPop(){
    
}

void AncPop::evolve(int gen){
    std::vector<int> ids;
    int size = static_cast<int>(haplos.size());
    for( int g = 0; g < gen; ++g){
        ids.clear();
        for (int i = 0; i < size; ++i) {
            ids.push_back(i);
        }
        std::vector<int> shuffle;
        for (int i = 0; i < size; ++i) {
            int tmp = rand() % ids.size();
            shuffle.push_back(ids.at(tmp));
            ids.erase(ids.begin() + tmp);
        }
        std::vector<Chrom> haploNext;
        for (int i = 0; i < size / 2; ++i) {
            ChromPair cp(haplos.at(i * 2), haplos.at(i * 2 + 1));
            cp = cp.recombine();
            haploNext.push_back(cp.getChrom(1));
            haploNext.push_back(cp.getChrom(2));
        }
        haplos = haploNext;
    }
}

std::vector<Chrom> & AncPop::getHaplos(){
    return haplos;
}