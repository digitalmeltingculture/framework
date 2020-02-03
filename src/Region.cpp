//
//  Region.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "Region.h"

string Region::toString() {
    list<Region*>::iterator rIt;
    stringstream sstm;
    
	sstm << "<region id=\"" << id << "\" "
         <<"title=\""       << title << "\" "
         <<"regionName=\""       << regionName << "\" "
         <<"backgroundColor=\""       << backgroundColor << "\" "
         <<"backgroundOpacity=\""       << backgroundOpacity << "\" "
         <<"width=\""       << width << "\" "
         <<"height=\""      << height << "\" "
         <<"top=\""       << top << "\" "
         <<"bottom=\""      << bottom << "\" "
         <<"left=\""       << left << "\" "
         <<"right=\""      << right << "\" "
         <<"fit=\""      << fit << "\" "
         <<"showBackground=\""      << showBackground << "\" "
         <<"z-index=\""      << zIndex << "\" "
         << ">" << endl;
    
    for(rIt=children.begin(); rIt!=children.end(); rIt++) {
        sstm << (*rIt)->toString();
    }
    
    sstm << "</region>" << endl;
    
    return sstm.str();
}
