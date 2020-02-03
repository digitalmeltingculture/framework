/*
 * Parallel.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: prisca-davide
 */

using namespace std;

#include <iostream>
#include <string>
#include <list>

#include "Content.h"
#include "TmpTransition.h"
#include "State.h"
#include "Parallel.h"

Parallel::Parallel(string id){
	this->id = id;
}

Parallel::Parallel(string id, list<State*> states) {
	this->id = id;
	this->states = states;
}

Parallel::~Parallel(){}

string Parallel::getId(){
	return id;
}

void Parallel::setId(const string id) {
	this->id = id;
}

list<State*> Parallel::getStates(){
	return states;
}

void Parallel::setStates(list<State*> states) {
	this->states = states;
}

ostream& operator<<(ostream& os, Parallel& parallel){
	list<State*>::iterator iter;

	os << "Parallel: id="+parallel.getId()+";"<< endl ;

	if(parallel.states.empty())
		return os;

	os << "n° states=" << parallel.states.size() <<endl;

	for(iter = parallel.states.begin(); iter != parallel.states.end(); ++iter){
		os << **iter;
	}

    return os;
}
