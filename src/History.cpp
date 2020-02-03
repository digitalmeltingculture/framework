//
//  History.cpp
//  iPast_mw
//
//  Created by Dario Di Mauro on 09/02/15.
//
//

#include "History.h"

//---------------------------------
/**  Constructor
 */
History::History() {
    initValues(20, 10);
}

//---------------------------------
/**  Constructor
 */
History::History(int length) {
    initValues(length, length/2);
}

//---------------------------------
/**  Constructor
 */
History::History(int length, int pos) {
    initValues(length, pos);
}

//---------------------------------
/**  Init the member values
 */
void History::initValues(int length, int pos) {
    history_length = length;
    value_position = pos;
    current_pos = 0;
    
    history_vect.reserve(length);
    indexes.reserve(length);
    
    history_vect.insert(history_vect.begin(), length, 0);
    indexes.insert(indexes.begin(), history_length, 0);
    
    initIndexes();
    initWeights(UNIFORM, 0);
}

//---------------------------------
/**  Init indexes vector
 */
void History::initIndexes() {
    int i = 0;
    
    for(i=0; i<indexes.size(); i++) {
        indexes[i] = i;
    }
}

//---------------------------------
/**  init all a vector to value
 */
void History::initHistory(float value) {
    for(int i=0; i<history_vect.size(); i++) {
        history_vect[i] = value;
    }
}

//---------------------------------
/**  init all a vector to value
 */
void History::initWeights(WeightsType t, float gamma) {
    int   i;
    float value;

    // Init vector
    if( weights.size()==0 ) {
        // First init
        weights.insert(weights.begin(), history_length, 1);
    }
    
    // Current init
    int size = weights.size();
    for(i=0; i<size; i++) {
        weights[i] = (float)(size-i)/size;
    }

    // Weight 
    if( t==GAMMA ) {
        for(i=0; i<weights.size(); i++) {
            weights[i] = pow(weights[i], gamma);
        }
    }
    else if( t==UNIFORM ) {
        value = 1.0f/(float)weights.size();
        
        for(i=0; i<weights.size(); i++) {
            weights[i] = value;
        }
    }
    else if( t==LINEAR ) {
        // Va bene così
    }
}

//---------------------------------
/**  Add an element in the vector
 */
void History::addElement(float value) {
    history_vect[current_pos] = value;

    current_pos = (current_pos+1)%history_length;
    
    shiftIndexesVector();
}

//---------------------------------
/**  Shift the indexes vector to right
 */
void History::shiftIndexesVector() {
    int popTmp, i;
    
    popTmp = indexes[indexes.size()-1];
    for(i=indexes.size()-1; i>0; i--) {
        indexes[i] = indexes[i-1];
    }
    indexes[0] = popTmp;
}

//---------------------------------
/**  returns the median value of the history vector
 */
float History::getMedianValue() {
    float median;
    
    sort( indexes.begin(), indexes.end(), *this);
    
    if( history_vect.size()==history_length ) {
        median = history_vect[indexes[value_position]];
    }
    else if( history_vect.size()==0 ) {// Empty vector
        median = 0;
    }
    else {
        // Not full history vector, but I take median value
        median = history_vect[indexes[history_vect.size()/2]];
    }
    
    initIndexes();
    
    return median;
}

//---------------------------------
/**  returns the mean value of the history vector
 */
float History::getMeanValue() {
    int i;
    float mean, weightsSum;
    
    mean = weightsSum = 0.0f;
    
    for( i=0; i<history_vect.size(); i++ ) {
        mean += weights[i]*history_vect[ indexes[i] ];
        weightsSum += weights[i];
    }
    
    return mean/weightsSum;
}

//---------------------------------
/**  returns the value in the past of oldPos times before
 */
float History::getPreviousValue(int oldPos) {
    int requiredPos;
    
    if( oldPos<0 || oldPos>history_vect.size() ) {
        cerr << "invalid position" << endl;
        return -1;
    }
    
    requiredPos = current_pos-oldPos;
    
    if( requiredPos < 0 ) {
        requiredPos = history_vect.size() + requiredPos;
    }
    
    return history_vect[requiredPos];
}

//---------------------------------
/**  Support operator to sort the history vector
 */
bool History::operator () ( const int & i1, const int & i2 ) {
    return history_vect[i1] < history_vect[i2];    
}
