/*
 * Smil.cpp
 *
 *  Created on: 05/dic/2014
 *      Author: dariux881
 */

#include "Smil.h"

Smil::Smil() {
	head = NULL;
	body = NULL;
}

Smil::~Smil() {
}

void Smil::playSmil() {
    if( !body )
        return;

    body->playBody();
}

void Smil::setHead(Head *head) {
	this->head = head;
    
    linkBodyAndHead();
}

void Smil::setBody(Body *body) {
	this->body = body;
    
    linkBodyAndHead();
}

Head* Smil::getHead() {
	return head;
}

Body* Smil::getBody() {
	return body;
}

void Smil::linkBodyAndHead() {
    list<pair<void*, string> > contents;
    list<pair<void*, string> >::iterator compIt;
    SmilMediaObject* tmpNode;
    
    if( !body )
        return;
    
    contents = body->getContents();
    
    for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
        if( !((*compIt).second).compare("Container") ) {
            linkChild( (Composite*)(*compIt).first );
        }
        else { // MediaObject
            tmpNode = (SmilMediaObject*)(*compIt).first;
            tmpNode->setWindow(getLayoutFromName(tmpNode->getWindowName()));
            tmpNode->setRegion(getRegionFromName(tmpNode->getWindowName()));
        }
    }
}

void Smil::linkChild( Composite *child ) {
    list<pair<void*, string> > contents;
    list<pair<void*, string> >::iterator compIt;
    SmilMediaObject* tmpNode;
    
    if( !child )
        return;
    
    contents = child->getContents();
    
    for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
        if( !((*compIt).second).compare("Container") ) {
            linkChild( (Composite*)(*compIt).first );
        }
        else { // MediaObject
            tmpNode = (SmilMediaObject*)(*compIt).first;
            tmpNode->setWindow(getLayoutFromName(tmpNode->getWindowName()));
            tmpNode->setRegion(getRegionFromName(tmpNode->getWindowName()));
        }
    }
}

TopLayout* Smil::getLayoutFromName(string name) {
    list<TopLayout*> allLayouts;
    list<TopLayout*>::iterator it;
    
    if( !head )
        return NULL;
    
    allLayouts = head->getTopLayouts();
    
    for( it=allLayouts.begin(); it!=allLayouts.end(); it++) {
        if( !name.compare((*it)->getId()) )
            return *it;
    }
    
    return NULL;
}


Region* Smil::getRegionFromName(string name) {
    list<TopLayout*> allLayouts;
    list<TopLayout*>::iterator it;
    Region* retReg;
    
    if( !head )
        return NULL;
    
    allLayouts = head->getTopLayouts();
    for( it=allLayouts.begin(); it!=allLayouts.end(); it++) {
        if( (retReg = getRegionFromList((*it)->getChildrenRegion(), name)) )
            return retReg;
    }
    
    return NULL;
}

Region* Smil::getRegionFromList(list<Region*> regions, string name) {
    Region* tmp;
    list<Region*>::iterator it;
    
    for( it=regions.begin(); it!=regions.end(); it++) {
        tmp = getRegionFromList((*it)->getChildren(), name);
        if(tmp)
            return tmp;
        
        if( !name.compare((*it)->getId()) )
            return *it;
    }
    
    return NULL;
}
