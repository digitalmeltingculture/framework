#include "Roi.h"

string Roi::toString() {
    list<Roi*>::iterator rIt;
    stringstream sstm;

	sstm << "<region id=\"" << id << "\" "
         <<"title=\""       << title << "\" "
         <<"width=\""       << width << "\" "
         <<"height=\""      << height << "\" "
         <<"top=\""       << top << "\" "
         <<"left=\""       << left << "\" "
         <<"right=\""      << right << "\" "
         << ">" << endl;

    for(rIt=this->children.begin(); rIt!=children.end(); rIt++) {
        sstm << (*rIt)->toString();
    }

    sstm << "</roi>" << endl;

    return sstm.str();
}
