#include "Mask.h"

string Mask::toString() {
	list<Mask*>::iterator maskIt;
	stringstream sstm;

	sstm << "<mask id=\"" << id << "\" "
         <<"src=\""       << src << "\" "
         << ">" << endl;

	for(maskIt=children.begin(); maskIt!=children.end(); maskIt++) {
	        sstm << (*maskIt)->toString();
	    }


    sstm << "</mask>" << endl;

    return sstm.str();
}
