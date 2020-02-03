/*
 * Transition.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: prisca-davide
 */

using namespace std;

#include <iostream>
#include <string>
#include <list>

#include "Content.h"
#include "TmpTransition.h"
#include "State.h"
#include "Transition.h"

Transition::Transition(string event, State* target){
	this->event = event;
	this->target = target;
}

Transition::Transition(string event, string cond, State* target){
	this->event = event;
	this->cond = cond;
	this->target = target;
}

string Transition::getEvent(){return event;}
string Transition::getCond(){return cond;}
State* Transition::getTarget(){return target;}

void Transition::setEvent(string event){
	this->event = event;
}

void Transition::setCond(string cond){
	this->cond = cond;
}

void Transition::setTarget(State* target){
	this->target = target;
}

ostream& operator<<(ostream& os, Transition& transition){
	os << "Transition: address="<< &transition << "; event=" + transition.event + "; + cond=" + transition.cond;
	os << "; target = " << (*transition.target).getId()<< endl;
    return os;
}
