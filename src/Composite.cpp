/*
 * Composite.cpp
 *
 *  Created on: 10/dic/2014
 *      Author: dariux881
 */

#include "Composite.h"

Composite::Composite() {
	type = "";
    region = NULL;
    begin = dur = end = 0;
    repeat = 0;
    hard_sync = false;
}

Composite::~Composite() {
}

void Composite::setContents(list<pair<void*, string> > contents) {
	this->contents = contents;
}

void Composite::setId(string id) {
    this->id = id;
}

void Composite::setTitle(string title) {
    this->title = title;
}

void Composite::setBegin(long begin) {
    this->begin = begin;
}

void Composite::setDur(long dur) {
    this->dur = dur;
}

void Composite::setEnd(long end) {
    this->end = end;
}

void Composite::setRepeat(float repeat) {
    this->repeat = repeat;
}

void Composite::setRegion(Region* region) {
    this->region = region;
}

void Composite::setHardSync(bool hs) {
    this->hard_sync = hs;
}
