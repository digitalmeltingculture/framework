/*
 * SmilParser.h
 *
 *  Created on: 05/dic/2014
 *      Author: dariux881
 */

#ifndef SMILPARSER_H_
#define SMILPARSER_H_

#include <stdio.h>
#include <iostream>

#include <libxml/parser.h>

#include "Smil.h"
#include "Head.h"
#include "RootLayout.h"
#include "TopLayout.h"
#include "Region.h"
#include "Roi.h"
#include "Mask.h"
#include "Body.h"
#include "Composite.h"
#include "Container.h"
#include "SmilMediaObject.h"
#include "Audio.h"
#include "Image.h"
#include "Text.h"
#include "TextStream.h"
#include "Video.h"
#include "Extra.h"

using namespace std;

class SmilParser {
public:

	long proporzione(long val1, long val2, long val3);
	Smil* parse(const string smilPath);
    Smil* readSmil(xmlNode*);

private:
    Head* readHead(xmlNode*);
    Body* readBody(xmlNode*);

    Head*		readLayout(xmlNode*, Head*);
    RootLayout* readRootLayout( xmlNode*, RootLayout* );
    TopLayout*  readTopLayout( xmlNode* );
    Region*  	readRegion( xmlNode* );
    Roi* 		readRoi(xmlNode*);
    Mask*		readMask(xmlNode*);

    Composite* readContainer(xmlNode*, const xmlChar*);
    bool  readContainerProperty(Composite*, xmlNode*);

    SmilMediaObject* readMediaObject(xmlNode*, const xmlChar*);
    bool  readMediaObjectProperty(SmilMediaObject*, xmlNode*);

    long  processTime(string strTime);
    int  processPosition(string strTime);
    bool  checkTopLayoutParams(string key, string value);
};

#endif /* SMILPARSER_H_ */
