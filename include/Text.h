//
//  Text.h
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__Text__
#define __smil_parser__Text__

#include <iostream>
#include "SmilMediaObject.h"

class Text : public SmilMediaObject {
public:
	Text();
	~Text();

    void play() {
    	execPlay();
    }

    void printMediaObject() {
    	cout << "\t" << "<text id=\"" << id << "\" src=\"" << src << "\" begin=\""<< begin <<"\" >\n";
    }

    string toString() {
        stringstream ss;
    	ss << "<text id=\"" << id <<"\" "
        <<"title=\"" << title <<"\" "
        <<"src=\"" << src <<"\" "
        <<"type=\"" << type <<"\" "
        <<"alt=\"" << alt <<"\" "
        <<"begin=\""<< begin <<"\" "
        <<"dur=\""<< dur <<"\" "
        <<"end=\""<< end <<"\" "
        <<"clipBegin=\""<< clipBegin <<"\" "
        <<"clipEnd=\""<< clipEnd <<"\" "
        <<"region=\"" << windowName <<"\" "
        <<"/>";
        
        return ss.str();
    }

private:
    void execPlay();
};

#endif /* defined(__smil_parser__Text__) */
