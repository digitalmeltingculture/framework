/*
 * TmpTransition.cpp
 *
 *  Created on: Dec 1, 2014
 *      Author: prisca-davide
 */

#include <string>
#include<iostream>

using namespace std;

#include "TmpTransition.h"

TmpTransition::TmpTransition(string event,string target){
	this->event = event;
	this->target = target;
}

TmpTransition::TmpTransition(string event, string cond, string target){
	this->event = event;
	this->cond = cond;
	this->target = target;
}

string TmpTransition::getEvent(){return event;}
string TmpTransition::getCond(){return cond;}
string TmpTransition::getTarget(){return target;}

void TmpTransition::setEvent(string event){
	this->event = event;
}

void TmpTransition::setCond(string cond){
	this->cond = cond;
}

void TmpTransition::setTarget(string target){
	this->target = target;
}

ostream& operator<<(ostream& os, const TmpTransition& transition){

	os << "Transition: event=" + transition.event + "; + cond=" + transition.cond+ "; target=" + transition.target +".\n";

    return os;
}
