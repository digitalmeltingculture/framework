/*
 * State.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#include<iostream>
#include<string>
#include<list>
#include<map>

using namespace std;

#include "Content.h"
#include "Transition.h"
#include "TmpTransition.h"
#include "Parallel.h"
#include "State.h"

State::State(string id,string type) {
	this->id = id;
	this->type=type;
	parallel = NULL;
}
State::State(string id, Parallel* parallel){
	this->id = id;
	this->parallel = parallel;
}

bool State::isSubState(State *next){
	list<State*> statesList;
	list<State*>::iterator it;
	Parallel *p;

	if( ( p = getParallel() ) == NULL)
		return false;

	statesList = p->getStates();

	for(it=statesList.begin();it!=statesList.end();it++){
		if(! next->getId().compare((*it)->getId())){
			return true;
		}
	}

	return false;
}

ostream& operator<<(ostream& os, State& state){
//	list<TmpTransition> tmpTransList;
//	list<TmpTransition>::iterator tmpTransIter;

	list<Transition*>::iterator transIter;

	os << "State: id=" + state.id + "; address="<< &state <<"; onentry=" + state.onentry + "; onexit=" + state.onexit +".\n";

	if( state.parallel != NULL)
		os << *state.parallel;


	if( !state.transitions.empty()){
		cout << "numero di transizioni: " << state.transitions.size()<<endl;

		list<Transition*> stateTransitions = state.transitions;
		list<Transition*>::iterator stateTransitionsIter;

		for(stateTransitionsIter = stateTransitions.begin(); stateTransitionsIter != stateTransitions.end(); stateTransitionsIter++){
//			os << (*stateTransitionsIter) << endl;
			cout << (*stateTransitionsIter)->getEvent() << endl;
			cout << "Allatta anche me!" << endl;

		}
    }
	os << "finito di stampare stato."<<endl;

//    list<Content> content = state.content;
//    list<Content>::iterator contentIter;

//    if(!content.empty()){
//		for(contentIter = content.begin(); contentIter != content.end(); ++contentIter){
//			os << contentIter;
//		}
//    }
    return os;
}

const string State::getType(){
	return this->type;
}

void State::setType(const string type) {
   this->type = type ;
}

list<Content> State::getContent(){
	return  this->content;
}

void State::setContent(const list<Content> content) {
	this->content = content;
}

const string State::getId(){
	return id;
}

void State::setId(const string id) {
	this->id = id;
}

string State::getOnentry(){
	return onentry;
}

void State::setOnentry(const string onEntryAction) {
	this->onentry = onEntryAction;
}

string State::getOnexit() {
	return onexit;
}

void State::setOnexit(const string onExitAction) {
	this->onexit = onExitAction;
}

Parallel* State::getParallel(){
	return parallel;
}

void State::setParallel(Parallel* parallel) {
	this->parallel = parallel;
}

list<TmpTransition> State::getTmpTransitions(){
	return tmpTransitions;
}

void State::setTmpTransitions(list<TmpTransition> tmpTransitions){
	this->tmpTransitions = tmpTransitions;
}

void State::addTmpTransition(TmpTransition trans){
	this->tmpTransitions.push_back(trans);
}

list<Transition*> State::getTransitions(){
	return transitions;
}

void State::setTransitions(list<Transition*> transitions){
	this->transitions = transitions;
}

void State::addTransition(Transition* transition){
	this->transitions.push_back(transition);
}







