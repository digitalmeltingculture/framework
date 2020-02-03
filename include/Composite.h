/*
 * Composite.h
 *
 *  Created on: 10/dic/2014
 *      Author: dariux881
 */

#ifndef COMPOSITE_H_
#define COMPOSITE_H_

#include <string>
#include <list>
#include "Region.h"
#include "SmilMediaObject.h"

using namespace std;

class Composite {
public:
	Composite();
	virtual ~Composite();

    /* Getter and setter */
    void setId(string id);
    void setTitle(string title);

    void setBegin(long begin);
    void setDur(long dur);
    void setEnd(long end);

    void setRepeat(float repeat);
    void setRegion(Region* region);

    void setHardSync(bool hs);

    void setContents(list<pair<void*, string> > contents);
    list<pair<void*, string> > getContents() {
        return contents;
    }

    virtual string toString() = 0;
    virtual void printContent() = 0;
    virtual void playContent() = 0;
    virtual void stop() = 0;

public:
    /* Attributes */
    list<pair<void*, string> > contents;

    string type; // {par, seq, excl}

    string  id;     // Id of this Content
    string title; // Title of the Content

    long begin; // begin delay in ms
    long dur;   // duration time in ms
    long end;   // end time in ms

    float  repeat; // How many time should this Content repeat?

    Region* region; // optional: region of the window
    bool hard_sync; // reserved for seq Content: the end of a Media Object stops all the list
};

#endif /* COMPOSITE_H_ */
