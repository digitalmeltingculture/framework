//
//  historyChina.cpp
//  iPast_mw_2_0
//
//  Created by Dario Di Mauro on 16/05/15.
//
//

#include "historyChina.h"
#include <stdio.h>

HistoryChina::HistoryChina(int length) {
    MAX_HISTORY = length;
    MAX_DETECTION_HISTORY = length;
    
    eyeCenterHistory = new int[MAX_HISTORY];
    eyeCenterHistAcc = new int[MAX_HISTORY];
    weightedEyeCenterHistory = new float[MAX_HISTORY];
    
    historyWeights = new float[MAX_HISTORY];
    historyCENTROWeights = new float[MAX_HISTORY];
    detectionHistoryWeights = new float[MAX_HISTORY];
    
    detectionHistory = new int[MAX_HISTORY];
    detectionHistAcc = new int[MAX_HISTORY];
    weightedDetectionHistory = new float[MAX_HISTORY];
}

void HistoryChina::initHistoryBuffers() {
	initHistoryWeights();
	initDetectionHistoryWeights();
	initEyeCenterHistory();
	initEyeCenterHistAcc();
	initWeightedEyeCenterHistory();
}

void HistoryChina::updateHistoryBuffers(int prova) {
	updateEyeCenterHistory(prova);
	updateEyeCenterHistAcc();
	updateWeightedEyeCenterHistory();
	updateHistoryWeights();
	updateCurrentFrame();
}

/*** DETECTION HISTORY ***/
void HistoryChina::initDetectionHistory(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++)
		detectionHistory[i] = 0;
	currentDetectionFrameIndex = 0;
}

void HistoryChina::initDetectionHistAcc(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++)
		detectionHistAcc[i] = 0;
}

void HistoryChina::initWeightedDetectionHistory(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++)
		weightedDetectionHistory[i] = 0;
}

int HistoryChina::updateDetectionHistory(int value){
	detectionHistory[currentDetectionFrameIndex% MAX_DETECTION_HISTORY] = value;
	return currentDetectionFrameIndex;
}

void HistoryChina::updateDetectionHistAcc(){
	int cont = 0;
	int i=0, j=0;
    
	for(i=0; i<MAX_DETECTION_HISTORY; i++){
		cont = 0;
		for(j=0; j<MAX_DETECTION_HISTORY; j++){
			if(detectionHistory[i]==detectionHistory[j])
				cont++;
		}
		for(j=0; j<MAX_DETECTION_HISTORY; j++){
			if(detectionHistory[i]==detectionHistory[j])
				detectionHistAcc[j] = cont;
		}
	}
}

void HistoryChina::updateWeightedDetectionHistory(){
	float weight;
    
	for(int i=0; i<MAX_DETECTION_HISTORY; i++){
		//La storia pesata considera accumulatore e pesi.
		weight = detectionHistoryWeights[i] * detectionHistAcc[i];
		weightedDetectionHistory[i] = weight;
	}
}

//se c'è restituisce il valore che compare almeno il MAX_DETECTION_HISTORY*DETECTION_PRECISION + 1 delle volte;
//Pesa i valori dando più importanza al presente che al passato.
int HistoryChina::getDetectionWeightedAvgValue(float detection_precision){
	int maxIndex = getMaxValueIndex(weightedDetectionHistory,MAX_DETECTION_HISTORY);
    
	if (weightedDetectionHistory[maxIndex] > MAX_DETECTION_HISTORY*detection_precision)
		return detectionHistory[maxIndex];
	return 0;
}

void HistoryChina::printDetectionHistory(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++){
		if(i == currentDetectionFrameIndex)
			printf("\nDetection History[%d]=%d*", i, detectionHistory[i]);
		else
			printf("\nDetection History[%d]=%d", i, detectionHistory[i]);
	}
	printf("\n");
}

void HistoryChina::printDetectionHistAcc(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++){
		if(i == currentDetectionFrameIndex)
			printf("\ndetectionHistAcc[%d]=%d*",i,detectionHistAcc[i]);
		else
			printf("\ndetectionHistAcc[%d]=%d",i,detectionHistAcc[i]);
	}
	printf("\n");
}

void HistoryChina::printWeightedDetectionHistory(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++){
		if(i==currentDetectionFrameIndex)
			printf("\nweightedAcc[%d]=%16.2f* = %d*%16.2f",i,weightedDetectionHistory[i],detectionHistAcc[i],detectionHistoryWeights[i]);
		else
			printf("\nweightedAcc[%d]=%16.2f = %d*%16.2f",i,weightedDetectionHistory[i],detectionHistAcc[i],detectionHistoryWeights[i]);
	}
	printf("\n");
}

/*** EYE HISTORY ***/
void HistoryChina::initEyeCenterHistory(){
	for(int i=0; i<MAX_HISTORY; i++)
		eyeCenterHistory[i] = -1;
	currentFrameIndex = 0;
}

void HistoryChina::initEyeCenterHistAcc(){
	for(int i=0; i<MAX_HISTORY; i++)
		eyeCenterHistAcc[i] = 0;
}

void HistoryChina::initWeightedEyeCenterHistory(){
	for(int i=0; i<MAX_HISTORY; i++)
		weightedEyeCenterHistory[i] = 0;
}

void HistoryChina::resetEyeCenterHistory(int value) {
    for (int i=0; i<MAX_HISTORY; i++ ) {
        eyeCenterHistory[i] = value;
    }
}

int HistoryChina::updateEyeCenterHistory(int value){
	eyeCenterHistory[currentFrameIndex% MAX_HISTORY] = value;
	return currentFrameIndex;
}

void HistoryChina::updateEyeCenterHistAcc(){
	int cont = 0;
	int i=0, j=0;
    
	for(i=0; i<MAX_HISTORY; i++){
		cont = 0;
		for(j=0; j<MAX_HISTORY; j++){
			if(eyeCenterHistory[i]==eyeCenterHistory[j])
				cont++;
		}
		for(j=0; j<MAX_HISTORY; j++){
			if(eyeCenterHistory[i]==eyeCenterHistory[j])
				eyeCenterHistAcc[j] = cont;
		}
	}
}

void HistoryChina::updateWeightedEyeCenterHistory(){
	float weight;
    
	for(int i=0; i<MAX_HISTORY; i++){
		//La storia pesata considera accumulatore e pesi.
		weight = historyWeights[i] * eyeCenterHistAcc[i];
		weightedEyeCenterHistory[i] = weight;
	}
}

//se c'è restituisce il valore che compare almeno il 50% + 1 delle volte;
//-1 altrimenti.
//Pesa i valori dando più importanza al presente che al passato.
int HistoryChina::getEyeCenterWeightedAvgValue(float eye_precision){
	int maxIndex = getMaxValueIndex(weightedEyeCenterHistory,MAX_HISTORY);
    
	if (weightedEyeCenterHistory[maxIndex] > MAX_HISTORY*eye_precision){
		return eyeCenterHistory[maxIndex];
	}
    
	return -2;
}

void HistoryChina::printEyeCenterHistory(){
	for(int i=0; i<MAX_HISTORY; i++){
		if(i == currentFrameIndex)
			printf("\nHistory[%d]=%d*", i, eyeCenterHistory[i]);
		else
			printf("\nHistory[%d]=%d", i, eyeCenterHistory[i]);
	}
	printf("\n");
}

void HistoryChina::printEyeCenterHistAcc(){
	for(int i=0; i<MAX_HISTORY; i++){
		if(i == currentFrameIndex)
			printf("\neyeCenterHistAcc[%d]=%d*",i,eyeCenterHistAcc[i]);
		else
			printf("\neyeCenterHistAcc[%d]=%d",i,eyeCenterHistAcc[i]);
	}
	printf("\n");
}

void HistoryChina::printWeightedEyeCenterHistory(){
	for(int i=0; i<MAX_HISTORY; i++){
		if(i==currentFrameIndex)
			printf("\n*weightedAcc[%d]=%16.2f = %d*%16.2f",i,weightedEyeCenterHistory[i],eyeCenterHistAcc[i],historyWeights[i]);
		else
			printf("\nweightedAcc[%d]=%16.2f = %d*%16.2f",i,weightedEyeCenterHistory[i],eyeCenterHistAcc[i],historyWeights[i]);
	}
	printf("\n");
}



/** Current Frame **/
void HistoryChina::updateCurrentFrame(){
	currentFrameIndex = (currentFrameIndex+1)%MAX_HISTORY;
}

void HistoryChina::updateDetectionCurrentFrame(){
	currentDetectionFrameIndex = (currentDetectionFrameIndex+1)%MAX_DETECTION_HISTORY;
}


/*** WEIGHTS ***/
void HistoryChina::initHistoryWeights(){
	for(int i=0; i<MAX_HISTORY; i++)
		historyWeights[i] = 0;
}

void HistoryChina::initHistoryCENTROWeights(){
	for(int i=0; i<MAX_HISTORY; i++)
		historyCENTROWeights[i] = 0;
}

void HistoryChina::updateHistoryWeights(){
	float weight;
    
	for(int i=0; i<MAX_HISTORY; i++){
		//questo peso dà più importanza al presente che al passato.
		weight = ( (i+MAX_HISTORY) - currentFrameIndex)%MAX_HISTORY +1;
		//normalizzo tra 0 e 1;
		weight = weight/MAX_HISTORY;
        
		historyWeights[i]=weight;
	}
}

void HistoryChina::printHistoryWeights(){
	for(int i=0; i<MAX_HISTORY; i++){
		if(i == currentFrameIndex)
			printf("\neyeCenterHistWeights[%d]=%16.2f*",i,historyWeights[i]);
		else
			printf("\neyeCenterHistWeights[%d]=%16.2f",i,historyWeights[i]);
	}
	printf("\n");
}

/*** DETECTION WEIGHTS ***/
void HistoryChina::initDetectionHistoryWeights(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++)
		detectionHistoryWeights[i] = 0;
}

void HistoryChina::updateDetectionHistoryWeights(){
	float weight;
    
	for(int i=0; i<MAX_DETECTION_HISTORY; i++){
		//questo peso dà più importanza al presente che al passato.
		weight = ( (i+MAX_DETECTION_HISTORY) - currentDetectionFrameIndex)%MAX_DETECTION_HISTORY +1;
		//normalizzo tra 0 e 1;
		weight = weight/MAX_DETECTION_HISTORY;
        
		detectionHistoryWeights[i]=weight;
	}
}

void HistoryChina::printDetectionHistoryWeights(){
	for(int i=0; i<MAX_DETECTION_HISTORY; i++){
		if(i == currentDetectionFrameIndex)
			printf("\ndetectionHistWeights[%d]=%16.2f*",i,detectionHistoryWeights[i]);
		else
			printf("\ndetectionHistWeights[%d]=%16.2f",i, detectionHistoryWeights[i]);
	}
	printf("\n");
}


int HistoryChina::getMaxValueIndex(float a[], int n){
	float max = -1;
	int index = -1;
	for(int i=0; i<n; i++)
		if(a[i]>max){
			max = a[i];
			index = i;
		}
	return index;
}
