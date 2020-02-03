/*
 * Smil.h
 *
 *  Created on: 05/dic/2014
 *      Author: dariux881
 */

#ifndef SMIL_H_
#define SMIL_H_

#include <iostream>
#include "Body.h"
#include "Head.h"

class Smil {
public:
	Smil();
	virtual ~Smil();

	void setHead(Head*);
	void setBody(Body*);
    void playSmil();
    
	Head* getHead();
	Body* getBody();

private:
    void linkBodyAndHead();
    void linkChild(Composite*);
    TopLayout* getLayoutFromName(string);
    Region* getRegionFromName(string);
    Region* getRegionFromList(list<Region*>, string);
    
	Body *body;
	Head *head;
};

#endif /* SMIL_H_ */
