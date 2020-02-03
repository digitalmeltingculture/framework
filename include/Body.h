//
//  Body.h
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__Body__
#define __smil_parser__Body__

#include <stdio.h>
#include <list>
#include "Composite.h"
#include "SmilMediaObject.h"

class Body {
public:
	Body();

    void playBody();
    
    void setContents(list<pair<void*, string> > contents);
    list<pair<void*, string> > getContents() {
        return contents;
    }
    
    void printBody();
    void stop();
    
private:
    static void* playBody_helper(void*);
    
    list<pair<void*, string> > contents;

};

#endif /* defined(__smil_parser__Body__) */
