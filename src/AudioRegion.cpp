/*
 * AudioRegion.cpp
 *
 *  Created on: 13/dic/2014
 *      Author: dariux881
 */

#include "AudioRegion.h"
#include <sstream>

AudioRegion::AudioRegion() {
	// TODO Auto-generated constructor stub

}

AudioRegion::~AudioRegion() {
	// TODO Auto-generated destructor stub
}

string AudioRegion::toString() {
    stringstream ss;
    
    ss << "<audioLayout id=\"" << id << "\" "
    <<"title=\""       << title << "\" "
    <<"soundLevel=\""  << soundLevel << "\" "
    << endl;
    
    return ss.str();
}