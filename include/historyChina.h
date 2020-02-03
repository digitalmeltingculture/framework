//
//  historyChina.h
//  iPast_mw_2_0
//
//  Created by Dario Di Mauro on 16/05/15.
//
//

#ifndef __iPast_mw_2_0__historyChina__
#define __iPast_mw_2_0__historyChina__

#include <iostream>

class HistoryChina {
public:
    HistoryChina(int length);

    void initHistoryBuffers();
    void updateHistoryBuffers(int prova);
    int getEyeCenterWeightedAvgValue(float eye_precision);
    void resetEyeCenterHistory(int value);

//private:
    
    /*** DETECTION HISTORY ***/
    void initDetectionHistory();
    void initDetectionHistAcc();
    void initWeightedDetectionHistory();
    int updateDetectionHistory(int value);
    void updateDetectionHistAcc();
    void updateWeightedDetectionHistory();
    int getDetectionWeightedAvgValue(float detection_precision);
    void printDetectionHistory();
    void printDetectionHistAcc();
    void printWeightedDetectionHistory();
    
    /*** EYE HISTORY ***/
    void initEyeCenterHistory();
    void initEyeCenterHistAcc();
    void initWeightedEyeCenterHistory();
    int updateEyeCenterHistory(int value);
    void updateEyeCenterHistAcc();
    void updateWeightedEyeCenterHistory();
    void printEyeCenterHistory();
    void printEyeCenterHistAcc();
    void printWeightedEyeCenterHistory();
    
    /** Current Frame **/
    void updateCurrentFrame();
    void updateDetectionCurrentFrame();
    
    /*** WEIGHTS ***/
    void initHistoryWeights();
    void initHistoryCENTROWeights();
    void updateHistoryWeights();
    void printHistoryWeights();
    
    /*** DETECTION WEIGHTS ***/
    void initDetectionHistoryWeights();
    void updateDetectionHistoryWeights();
    void printDetectionHistoryWeights();
    int getMaxValueIndex(float a[], int n);

    
    int MAX_HISTORY, MAX_DETECTION_HISTORY;
    
    int *eyeCenterHistory;
    int *eyeCenterHistAcc;
    float *weightedEyeCenterHistory;
    
    float *historyWeights;
    float *historyCENTROWeights;
    float *detectionHistoryWeights;
    
    
    int *detectionHistory;
    int *detectionHistAcc;
    float *weightedDetectionHistory;
    
    int currentFrameIndex = 0;
    int currentDetectionFrameIndex = 0;

};

#endif /* defined(__iPast_mw_2_0__historyChina__) */
