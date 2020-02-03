//
//  Video.h
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__TextStream__
#define __smil_parser__TextStream__

#include <iostream>
#include "SmilMediaObject.h"

class TextStream : public SmilMediaObject {
public:
	TextStream();
	~TextStream();

    void play() {
    	execPlay();
    }

    void printMediaObject() {
    	cout << "\t" << "<textstream id=\"" << id << "\" src=\"" << src << "\" begin=\""<< begin <<"\" >\n";
    }

    string toString() {
        stringstream ss;
    	ss << "<textstream id=\"" << id <<"\" "
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

#endif /* defined(__smil_parser__TextStream__) */
