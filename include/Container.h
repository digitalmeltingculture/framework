//
//  Content.h
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__Content__
#define __smil_parser__Content__

#include <stdio.h>
#include <list>
#include <string>
#include "Composite.h"
#include "Region.h"
#include "SmilMediaObject.h"

using namespace std;

class Container : public Composite {
public:
    /* Methods */
    Container(string type);

    void stop() {
        execStop();
    }
    
    /* Play Content */
    void playContent() {
    	execContent();
    }


    string toString2(){
    	stringstream ss;
    	list<pair<void*, string> >::iterator compIt;

    	ss << type <<" title=\""<<title<< "\"";

    	return ss.str();
    }

    string toString() {
        stringstream ss;
		list<pair<void*, string> >::iterator compIt;
        
		ss << "<" << type <<" id=\""<<id<<"\" repeatCount=\""<< repeat << "\">\n";
        
		for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
			if( !((*compIt).second).compare("Container") ) {
				ss << ((Composite*)(*compIt).first)->toString();
                ss << endl;
			}
			else { // MediaObject
				ss << ((SmilMediaObject*)(*compIt).first)->toString();
                ss << endl;
			}
		}
        
		ss << "</" << type << ">\n";
        return ss.str();
    }
    
    void printContent() {
		list<pair<void*, string> >::iterator compIt;

		cout << "<" << type <<" id=\""<<id<<"\" repeatCount=\""<< repeat << "\">\n";

		for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
			cout << "\t";
			if( !((*compIt).second).compare("Container") ) {
				((Composite*)(*compIt).first)->printContent();
			}
			else { // MediaObject
				((SmilMediaObject*)(*compIt).first)->printMediaObject();
			}
		}

		cout << "</" << type << ">\n";
    }
private:
    void execContent();
    void execStop();
    
    static Container* containerInstance;
    static string buildXMLRootElement(string rootName, string body,string term);

    static void* sendContent_helper(void*);
    static void* sendContent_parallel_helper(void *sp) ;

    void  playInPar(string, void*);
    void  playInSeq(string, void*);
    static void* playContainerInPar(void*);
    static void* playContentInPar(void*);
    static void* playContentInSeq(void*);
};

class sender_params {
public:
    Floor* myFloor;
    int    window;
    string content;
};

#endif /* defined(__smil_parser__Content__) */
