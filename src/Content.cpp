/*
 * Content.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#include <string>
using namespace std;
#include "Content.h"
#include "Smil.h"

Content::Content() {
    myContent = NULL;
}

Content::~Content() {
	// TODO Auto-generated destructor stub
}

void Content::playContent() {
    if( !myContent )
        return;
    myContent->playSmil();
}

ostream& operator<<(ostream& os, const Content& content){
    os << "Print Content \n";
    return os;
}
