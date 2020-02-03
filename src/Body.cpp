//
//  Body.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "Body.h"

Body::Body() {
	contents.clear();
}

void Body::playBody() {
    pthread_t tid;
    
    pthread_create(&tid, NULL, playBody_helper, this);
}

void* Body::playBody_helper(void *params) {
    // play the list of mediaOjects and Contents
	list<pair<void*, string> >::iterator compIt;
    Body* mySelf;
    
    mySelf = (Body*)params;
        
	for( compIt = mySelf->contents.begin(); compIt!=mySelf->contents.end(); compIt++) {
		if( !((*compIt).second).compare("Container") ) {
			((Composite*)(*compIt).first)->playContent();
		}
		else { // MediaObject

			((SmilMediaObject*)(*compIt).first)->play();
		}
	}
}

void Body::setContents(list<pair<void*, string> > contents) {
    this->contents = contents;
}

void Body::stop() {
    // play the list of mediaOjects and Contents
	list<pair<void*, string> >::iterator compIt;
    
    for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
		if( !((*compIt).second).compare("Container") ) {
			((Composite*)(*compIt).first)->stop();
		}
	}
}

void Body::printBody() {
	list<pair<void*, string> >::iterator compIt;

	cout << "<body>\n";

	for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
		if( !((*compIt).second).compare("Container") ) {
			((Composite*)(*compIt).first)->printContent();
		}
		else { // MediaObject
			((SmilMediaObject*)(*compIt).first)->printMediaObject();
		}
	}

	cout << "</body>\n";

}
