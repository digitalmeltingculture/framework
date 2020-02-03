/*
 * TopLayout.cpp
 *
 *  Created on: 13/dic/2014
 *      Author: dariux881
 */

#include "TopLayout.h"

TopLayout::TopLayout() {
    constructor();
}

TopLayout::TopLayout(RootLayout* newParent) {
	constructor();

	parent = newParent;
}

TopLayout::~TopLayout() {
}

void TopLayout::constructor() {
	width = 0;
	height = 0;
    
	parent = NULL;
    
	open = "onStart";
	close = "onRequest";
}

void   TopLayout::print() {
	list<Region*>::iterator rIt;
    list<Roi*>::iterator roiIt;
    list<Mask*>::iterator maskIt;

    cout << "<topLayout id=\""<< id
    <<"\" title=\""<< title
    <<"\" width=\""<< width
    <<"\" height=\""<< height <<"\">\n";
    

	for(roiIt=childrenRoi.begin(); roiIt!=childrenRoi.end(); roiIt++) {
			(*roiIt)->print();
	}

	for(rIt=childrenRegion.begin(); rIt!=childrenRegion.end(); rIt++) {
		(*rIt)->print();
	}
    
	for(maskIt=childrenMask.begin(); maskIt!=childrenMask.end(); maskIt++) {
		(*maskIt)->print();
	}

	cout << "</topLayout>" << endl;
}

string TopLayout::toString() {
	list<Region*>::iterator rIt;
	list<Roi*>::iterator roiIt;
	list<Mask*>::iterator maskIt;


    stringstream sstm("");

//    if( parent )
//        sstm << parent->toString();
    
	sstm << "<topLayout id=\"" << id << "\" "
         <<"title=\""       << title << "\" "
         <<"name=\""       << name << "\" "
         <<"backgroundColor=\""       << backgroundColor << "\" "
         <<"backgroundOpacity=\""       << backgroundOpacity << "\" "
         <<"width=\""       << width << "px" <<"\" "
         <<"height=\""      << height << "px" << "\" "
         <<"open=\""      << open << "\" "
         <<"close=\""      << close << "\" "
         << ">" << endl;

	for(rIt=childrenRegion.begin(); rIt!=childrenRegion.end(); rIt++) {
		sstm << (*rIt)->toString();
	}

	/*for(roiIt=childrenRoi.begin(); roiIt!=childrenRoi.end(); roiIt++) {
		sstm <<	(*roiIt)->toString();
	}

	for(maskIt=childrenMask.begin(); maskIt!=childrenMask.end(); maskIt++) {
		sstm <<	(*roiIt)->toString();
	}*/

	sstm << "</topLayout>" << endl;
    return sstm.str();
}
