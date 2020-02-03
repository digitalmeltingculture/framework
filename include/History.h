//
//  History.h
//  iPast_mw
//
//  Created by Dario Di Mauro on 09/02/15.
//
//

#ifndef __iPast_mw__History__
#define __iPast_mw__History__

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <cmath>

using namespace std;

class History {
public:
    History();
    History(int length);
    History(int length, int pos);
    bool operator () ( const int & i1, const int & i2 );

    enum WeightsType { GAMMA, UNIFORM, LINEAR };

    void initHistory(float value);
    void initWeights(WeightsType, float);
    
    void addElement(float);
    float getMedianValue();
    float getMeanValue();
    float getPreviousValue(int oldPos);
        
private:
    void initValues(int, int);
    void initIndexes();
    void shiftIndexesVector();
    void updateDetectionHistAcc();
    
    vector<float> history_vect;
    vector<float> weights;
    vector<int>   indexes;
    
    int history_length;
    int value_position;
    
    int current_pos;
};

#endif /* defined(__iPast_mw__History__) */
