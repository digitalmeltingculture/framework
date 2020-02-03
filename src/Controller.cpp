using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <list>

#include "Transition.h"
#include "Content.h"
#include "TmpTransition.h"
#include "Controller.h"
#include "State.h"
#include "Root.h"
#include "Floor.h"

void Controller::sendContentParallel() {

	int clients;
	bool flag;
	myFloor=Floor::getInstance("", 0, 0);

    myFloor->sendContentParallelState(this->getId());
}


void Controller::sendFlagParentela() {

	int clients;
	bool flag;
	myFloor=Floor::getInstance("", 0, 0);

	string flagParentela= std::to_string(parentela);

    myFloor->sendParentela(this->getId(),flagParentela.c_str());
}



void Controller::changeState(string event,string cond){

	list<Transition*> allTransitions;
	list<Transition*>::iterator it;

	allTransitions =currentState->getTransitions();

	it=allTransitions.begin();
	while( it!=allTransitions.end()) {

		if((!(*it)->getCond().compare(cond.c_str()))&&
				(!(*it)->getEvent().compare(event.c_str()))){

			nextState = (*it)->getTarget();

			gestioneRelazioneSottostati();
			gestioneParentela();

			transitToNextState();
			break;
		}
		it++;
	}
}


void Controller::gestioneParentela(){

	if(currentState->getParallel()== NULL){

		if( nextState->isSubState(currentState)==true){
			parentela=1;
		}
		else parentela=0;
	}
	else parentela=0;
}

void Controller::gestioneRelazioneSottostati(){
	if(precedentState->isSubState(nextState)){
		setParallel(true);
	}
	else{
		if(currentState->isSubState(nextState)){
			setParallel(true);
			precedentState=currentState;
		}
		else{
			setParallel(false);
			precedentState=currentState;
		}
	}
}



void Controller::transitToNextState(){

	if(currentState ) {
		onStateExit();
	}

	cout << "transit";

	if( currentState )	cout << " from " << currentState->getId();
	if( nextState ) 	cout << " to " << nextState->getId() ;

	cout << endl;

	currentState = nextState;
	processState();

}

void Controller::processState(){
	onStateEntry();
}

void Controller::onStateEntry(){

	if( currentState && !currentState->getOnentry().compare("loadContent") ) {

		this->sendFlagParentela();
		parentela=0;

		if(parallel){
	    	this->sendContentParallel();
	    }
	    (*(currentState->getContent().begin())).playContent();
	}
}

void Controller::onStateExit(){

}


void Controller::setCurrentState(State* currentState){
	this->currentState=currentState;
}

State* Controller::getCurrentState(){
	return this->currentState;
}

void Controller::setNextState(State* nextState){
	this->nextState=nextState;
}

State* Controller::getNextState(){
	return this->nextState;
}

void Controller::setParallel(bool parallel){
	this->parallel=parallel;
}

bool Controller::getParallel(){
	return this->parallel;
}

int Controller::getId(){
	return id;
}

void Controller::setId(int id){
	this->id=id;
}


void Controller::setFloor(Floor* myFloor)
{
	this->myFloor;
}

Floor* Controller::getFloor(){
	return this->myFloor;
}

Controller::Controller(int id,State* currentState,Root* initState){
	this->id=id;
	this->precedentState=currentState;
	this->currentState=currentState;
	this->nextState=NULL;
	this->parallel=false;
	this->myFloor=NULL;
	this->initState=initState;
}

void Controller::setParentela(int parentela){
	this->parentela=parentela;
}

int Controller::getParentela(){
	return this->parentela;
}

