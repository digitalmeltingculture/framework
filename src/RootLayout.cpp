/*
 * RootLayout.cpp
 *
 *  Created on: 13/dic/2014
 *      Author: dariux881
 */

#include "RootLayout.h"
#include <sstream>

RootLayout::RootLayout() {
	width = height = 0;
}

RootLayout::~RootLayout() {
	// TODO Auto-generated destructor stub
}

string RootLayout::toString() {
    stringstream sstm("");
    
    sstm << "<root-layout id=\"" << id << "\" "
         <<"title=\""       << title << "\" "
         <<"backgroundColor=\""       << backgroundColor << "\" "
         <<"backgroundOpacity=\""       << backgroundOpacity << "\" "
         <<"width=\""       << width << "\" "
         <<"height=\""      << height << "\" "
         << "/>" << endl;

    return sstm.str();
}