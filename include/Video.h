//
//  Video.h
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__Video__
#define __smil_parser__Video__

#include <iostream>
#include "SmilMediaObject.h"

class Video : public SmilMediaObject {
public:
	Video();
	~Video();

    void play() {
    	execPlay();
    }

    void printMediaObject() {
    	cout << "\t" << "<video id=\"" << id << "\" src=\"" << src << "\" begin=\""<< begin <<"\" >\n";
    }

    string toString() {
        stringstream ss;
    	ss << "<video id=\"" << id <<"\" "
        <<"title=\"" << title <<"\" "
		<<"loop=\"" << loop <<"\" "
		<<"fps=\"" << fps <<"\" "
		<<"src=\"" << src <<"\" "
		<<"player=\"" << player <<"\" "
		<<"type=\"" << type <<"\" "
        <<"alt=\"" << alt <<"\" "
        <<"begin=\""<< begin <<"\" "
        <<"dur=\""<< dur <<"\" "
        <<"end=\""<< end <<"\" "
        <<"clipBegin=\""<< clipBegin <<"\" "
        <<"clipEnd=\""<< clipEnd <<"\" "
        <<"region=\"" << windowName <<"\" "
		<<"backgroundClient=\"" << backgroundClient <<"\" "
		<<"/>";
        
        return ss.str();
    }

private:
    void execPlay();
};

#endif /* defined(__smil_parser__Video__) */
