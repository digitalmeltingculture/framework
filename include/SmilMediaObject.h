//
//  MediaObject.h
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__MediaObject__
#define __smil_parser__MediaObject__

#include <iostream>
#include <string>
#include "Region.h"
#include "Floor.h"
#include "ofApp.h"

using namespace std;

class SmilMediaObject {
public:
	virtual ~SmilMediaObject() { };


	void setBackgroundClient(string name);
	string getBackgroundClient();

	string backgroundClient = "false";

    /* SETTER */

	void setLoop(string);
	string getLoop();

    void setId(string id);
    void setTitle(string title);
    
    void setBegin(long begin);
    void setDur(long dur);
    void setEnd(long end);
    void setClipBegin(string begin);
    void setClipEnd(string begin);
    
    void setPlayer(string player);
    string getPlayer();

    void setSrc(string src);
    void setAlt(string alt);
    void setType(string type);
    void setRegion(Region* region);
    void setWindowName(string name);
    void setWindow(TopLayout* window);
    
    void setFps(int);
    int getFps();

    /* GETTER */
    string getId();
    string getTitle();
    
    long getBegin();
    long getDur();
    long getEnd();
    string getClipBegin();
    string getClipEnd();
    
    string getSrc();
    string getAlt();
    string getType();
    Region* getRegion();
    
    string  getWindowName() {
        return windowName;
    }
    
    TopLayout* getWindow() {
        if( window )
            return window;
        
        if( region )
            return region->getTopLayout();
        
        return NULL;
    }
    
    virtual string toString() = 0;
    virtual void printMediaObject() = 0;
    virtual void play() = 0;
    
    ofApp*  general;

protected:
    /* Attributes */
    string  id="";     // Id of this Content
    string title=""; // Title of the Content

    int fps=0;

    string player="";
    long begin=0; // begin delay in ms
    string clipBegin="";
    long dur=0;   // duration time in ms
    long end=0;   // end time in ms
    string clipEnd="";
    
    string loop="true";
    string src="";  // source path
    string type=""; // source type
    string alt="";  // alternative to the src
    
    Region* region; // optional: region of the window
    
    TopLayout* window;
    string     windowName;
    
    Floor*  client;
    
};

#endif /* defined(__smil_parser__MediaObject__) */
