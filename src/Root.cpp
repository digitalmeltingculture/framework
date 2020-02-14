/*
 * Root.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */
#include <iostream>
#include <string>
#include <list>

#include "../include/Content.h"
#include "../include/TmpTransition.h"
#include "../include/State.h"
#include "../include/Root.h"

using namespace std;

State* Root::initialState = NULL;

Root::~Root() {
	// TODO Auto-generated destructor stub
}

State* Root::getInitialState() {
    if( !initial.compare("") ) {
        if( !states || states->size()==0 ) {
            return NULL;
        }
        
        return *(states->begin());
    }

    return getStateById(initial);
}

State* Root::getStateById(string id) {
    list<State*>::iterator it;
    
    for( it= states->begin(); it!=states->end(); it++) {
        if( !(*it)->getId().compare(id) )
            return *it;
    }
    
    return NULL;
}

ostream& operator<<(ostream& os, const Root& root){
    os << "scxml: name="+ root.name + "; "+ "initial=" + root.initial <<endl;
    return os;
}

