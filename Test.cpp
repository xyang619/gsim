//
//  Test.cpp
//  AdmSimulator
//
//  Created by young on 1/18/15.
//  Copyright (c) 2015 young. All rights reserved.
//

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

int findPos(const vector<double> & poss, double pos) {
    int left = 0;
    int right = static_cast<int>(poss.size());
    cout<<"size " <<right<<endl;
    if (pos <= poss.front())
        return left;
    if (pos >= poss.back())
        return right;
    int mid = (left + right + 1) / 2;
    while (left < right) {
        if (pos > poss.at(mid))
            left = mid;
        else
            right = mid - 1;
        mid = (left + right + 1) / 2;
    }
    return left+1;
}

string copySeq(const vector<double> & poss, const string & seq, double start, double end) {
    int left, right;
    left = findPos(poss, start);
    right = findPos(poss, end);
    cout<<"left "<<left<<" right "<<right<<endl;
    return seq.substr(left, right - left);
}

int main(){
    string seq="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string posstr="0.45315182 0.45329516 0.45527908 0.45817462 0.45831724 0.46223668 0.46542124 0.4656376 0.4699259 0.47055458 0.47987616 0.48838196 0.48859116 0.49787115 0.49938338 0.50059907 0.50154634 0.50162934 0.5086825 0.51470921 0.51589352 0.51743042 0.52308478 0.52419694 0.53198 0.53199";
    istringstream iss(posstr);
    vector<double> poss;
    while(iss){
        double tmp;
        iss>>tmp;
        poss.push_back(tmp);
    }
    poss.pop_back();
    cout<<poss.size()<<endl;
    string cmb="";
    double start[3]={0.45315182, 0.48, 0.5};
    double end[3]={0.48,0.5,0.53199};
    for (int i=0; i<3; ++i) {
        cmb+=copySeq(poss, seq, start[i],end[i]);
        cout<<cmb<<endl;
    }
    return 0;
}