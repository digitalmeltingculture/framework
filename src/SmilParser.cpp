/*
 * SmilParser.cpp
 *
 *  Created on: 05/dic/2014
 *      Author: dariux881
 */

#include "SmilParser.h"


Smil* SmilParser::parse(const string smilPath) {
    Smil *smilContent = NULL;
    xmlDocPtr doc;
    xmlNode *root_element;

    doc = xmlReadFile(smilPath.c_str(), NULL, 0);
    if( doc==NULL ) {
    	cerr << "Failed to parse " << smilPath << endl;
    	return NULL;
    }

    root_element = xmlDocGetRootElement(doc);

    if( !xmlStrcmp(root_element->name, (const xmlChar*)"smil") ) {
    	smilContent = readSmil(root_element->children);
    }

    xmlFreeDoc(doc);

    return smilContent;
}

Smil* SmilParser::readSmil(xmlNode* node) {
	xmlNode *cur_node = NULL;
	Smil *smil;
	Head* head;
	Body* body;

	smil = new Smil();

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"head") ) {
			head = readHead(cur_node->children);
			if( head==NULL)
				return NULL;
			smil->setHead( head );
		}
		else if( !xmlStrcmp(cur_node->name, (const xmlChar*)"body") ) {
			body = readBody(cur_node->children);
			if( body==NULL )
				return NULL;
			smil->setBody( body );
		}
	}

	return smil;
}

Head* SmilParser::readHead(xmlNode* node) {
	Head* head;
	xmlNode *cur_node = NULL;

	head = new Head();

	int i=0;
	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"layout") ) {
			// In presenza di più layout prendo solo il primo in considerazione
			head = readLayout(cur_node->children, head);
			return head;
		}
		i++;
	}

	// Non ho trovato nessun layout specificato. Ne metto uno di default
	if( !head->getTopLayouts().size() ) {
		head->addTopLayout(new TopLayout( new RootLayout() ) );
	}

	return head;
}

Head* SmilParser::readLayout(xmlNode* node, Head* head) {
	xmlNode* cur_node;
	bool	 rootLayoutFounded = false;
	RootLayout* 		defRL;
	list<TopLayout*> 	allTL;
	TopLayout			*defTL, *tmpTL;
	list<TopLayout*>::iterator tlIt;
	Region*				tmpRegion;

	defRL = new RootLayout();
	defTL = new TopLayout(defRL);

	for (cur_node = node; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"root-layout") ) {
			if( rootLayoutFounded )
				return NULL;
			else {
				defRL = readRootLayout(cur_node, defRL);
				if( defRL == NULL)
					return NULL;
			}
		}
		else if( !xmlStrcmp(cur_node->name, (const xmlChar*)"topLayout") ) {
			if( defTL->getChildrenRegion().size()!=0 )
				// Trovato un topLayout ma in precedenza ho letto una region
				return NULL;

			tmpTL = readTopLayout( cur_node );

			if( tmpTL==NULL )
				return NULL;
			allTL.push_back(tmpTL);
		}
		else if( !xmlStrcmp(cur_node->name, (const xmlChar*)"region") ) {
			if( allTL.size()!=0 ) {
				// Trovato un layout non corretto
				return NULL;
			}
			else {
				tmpRegion = readRegion( cur_node );
				if( tmpRegion==NULL )
					return NULL;
				defTL->addChildRegion( tmpRegion );
			}
		}
	}

	if( allTL.size()==0) {
		// Non ho trovato topLayout. Può essere un BasicLayout
		allTL.push_back(defTL);
	}

	for(tlIt=allTL.begin(); tlIt!=allTL.end(); tlIt++) {
		(*tlIt)->setParent(defRL);
	}

	head->setTopLayouts(allTL);

	return head;
}

RootLayout* SmilParser::readRootLayout(xmlNode* node, RootLayout* defRL) {
	xmlAttr *prop;
	string  propStr;
	int	tmpSize;

	if( !defRL )
		defRL = new RootLayout();

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			defRL->setId( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"title") ) {
			defRL->setTitle( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundColor") ) {
			defRL->setBackgroundColor( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundOpacity") ) {
			defRL->setBackgroundOpacity( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"width") ) {
			tmpSize = processPosition( propStr );
			if( !tmpSize )
				return NULL;
			defRL->setWidth( tmpSize );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"height") ) {
			tmpSize = processPosition( propStr );
			if( !tmpSize )
				return NULL;
			defRL->setHeight( tmpSize );
		}

		prop = prop->next;
	}

	return defRL;
}

TopLayout*  SmilParser::readTopLayout(xmlNode* node ) {
	TopLayout* newTL;
	xmlAttr *prop;
	string  propStr;
	int	tmpSize;

	xmlNode* cur_node;
	Region*  tmpRegion;
	Roi*	 tmpRoi;
	Mask*	 tmpMask;

	newTL = new TopLayout();

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			newTL->setId( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"title") ) {
			newTL->setTitle( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"name") ) {
			newTL->setName( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"open") ) {
			if( !checkTopLayoutParams("open", propStr))
				return NULL;
			newTL->setOpen( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"close") ) {
			if( !checkTopLayoutParams("close", propStr))
				return NULL;
			newTL->setClose( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundColor") ) {
			newTL->setBackgroundColor( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundOpacity") ) {
			newTL->setBackgroundOpacity( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"width") ) {
			tmpSize = processPosition( propStr );
			if( !tmpSize )
				return NULL;
			newTL->setWidth( tmpSize );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"height") ) {
			tmpSize = processPosition( propStr );
			if( !tmpSize )
				return NULL;
			newTL->setHeight( tmpSize );
		}

		prop = prop->next;
	}

	// Scorro i figli eventuali
	bool prima=true;
	for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"region") ) {
			tmpRegion = readRegion( cur_node );
			if( tmpRegion==NULL )
				return NULL;
            /*if(prima){
            	tmpRegion->setIsRoi(false);
            	prima=false;
            }*/
            tmpRegion->setTopLayout(newTL);
			newTL->addChildRegion( tmpRegion );
		}

		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"roi") ) {
			tmpRoi = readRoi( cur_node );
			if( tmpRoi==NULL )
				return NULL;

		    tmpRoi->setTopLayout(newTL);
			newTL->addChildRoi( tmpRoi );
		}

		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"mask") ) {
			tmpMask = readMask( cur_node );
			if( tmpMask==NULL )
				return NULL;
			tmpMask->setTopLayout(newTL);
			newTL->addChildMask( tmpMask);
		}
	}

	return newTL;
}

Region* SmilParser::readRegion(xmlNode* node) {
	xmlAttr *prop;
	string  propStr;
	xmlNode* cur_node;
	Region* newRegion, *tmpRegion;
	int xc;

	newRegion = new Region();

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			newRegion->setId( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"title") ) {
			newRegion->setTitle( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"regionName") ) {
			newRegion->setRegionName( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"top") ) {

			newRegion->setTop( propStr );

			xc = processPosition( propStr );  //francesco
			if( xc==-1 )
				return NULL;
			newRegion->setTopInt( xc );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"bottom") ) {
			newRegion->setBottom( propStr );

			xc = processPosition( propStr ); //francesco
			if( xc==-1 ) return NULL;
			newRegion->setBottomInt( xc );

		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"left") ) {
			newRegion->setLeft( propStr );

			xc = processPosition( propStr ); //francesco
			if( xc==-1 ) return NULL;
			newRegion->setLeftInt( xc );

		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"right") ) {
			newRegion->setRight( propStr );

			xc = processPosition( propStr ); //francesco
			if( xc==-1 ) return NULL;
			newRegion->setRightInt( xc );

		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundColor") ) {
			newRegion->setBackgroundColor( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundOpacity") ) {
			newRegion->setBackgroundOpacity( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"fit") ) {
			newRegion->setFit( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"showBackground") ) {
			newRegion->setShowBackground( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"zIndex") ) {
			newRegion->setZIndex( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"width") ) {
			newRegion->setWidth( propStr );

			xc = processPosition( propStr ); //francesco
			if( xc==-1 ) return NULL;
			newRegion->setWidthInt( xc );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"height") ) {
			newRegion->setHeight( propStr );

			xc = processPosition( propStr ); //francesco
			if( xc==-1 ) return NULL;
			newRegion->setHeightInt( xc );

		}

		prop = prop->next;
	}

	// Scorro i figli eventuali
	for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"region") ) {
			tmpRegion = readRegion( cur_node );
			if( tmpRegion==NULL )
				return NULL;
            
            tmpRegion->setParentRegion(newRegion);
			newRegion->addChild( tmpRegion );
		}
	}

	return newRegion;
}

/*----------------------------------------------------

-----------------------------------------------*/
Roi* SmilParser::readRoi(xmlNode* node) {
	xmlAttr *prop;
	string  propStr;
	xmlNode* cur_node;
	Roi* newRoi, *tmpRoi;

	int xc;

	newRoi = new Roi();

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			newRoi->setId( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"roiName") ) {
			newRoi->setRoiName( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"top") ) {
			newRoi->setTop( propStr );

			xc = processPosition( propStr );  //francesco
			if( !xc ) return NULL;
			newRoi->setTopLong( xc );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"left") ) {
			newRoi->setLeft( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newRoi->setLeftLong( xc );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"width") ) {
			newRoi->setWidth( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newRoi->setWidthInt( xc );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"height") ) {
			newRoi->setHeight( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newRoi->setHeightInt( xc );
		}

		prop = prop->next;
	}

	// Scorro i figli eventuali
	for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"roi") ) {
			tmpRoi = readRoi( cur_node );
			if( tmpRoi==NULL )
				return NULL;

            tmpRoi->setParentRoi(newRoi);
            newRoi->addChild(tmpRoi);
		}
	}

	return newRoi;
}


Mask* SmilParser::readMask(xmlNode* node) {
	xmlAttr *prop;
	string  propStr;
	xmlNode* cur_node;

	int xc;

	Mask* newMask, *tmpMask;

	newMask = new Mask();

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			newMask->setId( propStr );
		}

		else if(!xmlStrcmp(prop->name, (const xmlChar*)"src") ) {
			newMask->setSrc( propStr );
		}

		if(!xmlStrcmp(prop->name, (const xmlChar*)"width") ) {
			newMask->setWidth( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newMask->setWidthInt( xc );
		}

		if(!xmlStrcmp(prop->name, (const xmlChar*)"height") ) {
			newMask->setHeight( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newMask->setHeightInt( xc );
		}

		if(!xmlStrcmp(prop->name, (const xmlChar*)"pivotX") ) {
			newMask->setPivotX( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newMask->setPivotXInt( xc );
		}

		if(!xmlStrcmp(prop->name, (const xmlChar*)"pivotY") ) {
			newMask->setPivotY( propStr );

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;
			newMask->setPivotYInt( xc );

		}

		prop = prop->next;
	}


	for (cur_node = node->children; cur_node; cur_node = cur_node->next) {
		if( !xmlStrcmp(cur_node->name, (const xmlChar*)"mask") ) {
			tmpMask = readMask( cur_node );
			if( tmpMask==NULL )
				return NULL;

			tmpMask->setParentMask(newMask);
			newMask->addChild(tmpMask);
		}
	}

	return newMask;
}


Body* SmilParser::readBody(xmlNode* node) {
	xmlNode* 			curNode;
	Body* 				body;
	list<pair<void*, string> > allMyContents;
	SmilMediaObject* 	tmpSmo;
	Composite* 			tmpContainer;

	if( node==NULL)
		return NULL;

	body = new Body();

	for (curNode = node; curNode; curNode = curNode->next) {
		if( curNode->type!=XML_ELEMENT_NODE)
			continue;

		if( !xmlStrcmp(curNode->name, (const xmlChar*)"par") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"seq") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"excl") ) {

			tmpContainer = readContainer(curNode->children, curNode->name);
			if( !tmpContainer)
				continue;

			readContainerProperty( tmpContainer, curNode );
			allMyContents.push_back( make_pair(tmpContainer, "Container") );
		}
		else if(!xmlStrcmp(curNode->name, (const xmlChar*)"audio") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"extra") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"img") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"text") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"textstream") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"video") ) {
			tmpSmo = readMediaObject(curNode, curNode->name);
			if( !tmpSmo )
				continue;
			allMyContents.push_back( make_pair(tmpSmo, "MediaObject") );
		}
	}

	body->setContents(allMyContents);

	return body;
}

Composite* SmilParser::readContainer(xmlNode* node, const xmlChar* type) {
	Composite* 			cont;
	xmlNode* 			curNode;
	list<pair<void*, string> > allMyContents;
	SmilMediaObject* 	tmpSmo;
	Composite* 			tmpContainer;

	if( node==NULL )
		return NULL;

	cont = new Container( (const char*) type );

	for (curNode = node; curNode; curNode = curNode->next) {
		if( curNode->type!=XML_ELEMENT_NODE)
			continue;

		if( !xmlStrcmp(curNode->name, (const xmlChar*)"par") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"seq") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"excl") ) {

			tmpContainer = readContainer(curNode->children, curNode->name);
			if( !tmpContainer)
				continue;

			readContainerProperty( tmpContainer, curNode );
			allMyContents.push_back( make_pair(tmpContainer, "Container") );
		}
		else if(!xmlStrcmp(curNode->name, (const xmlChar*)"audio") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"extra") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"img") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"text") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"textstream") ||
				!xmlStrcmp(curNode->name, (const xmlChar*)"video") ) {

			tmpSmo = readMediaObject(curNode, curNode->name);
			if( !tmpSmo )
				continue;
			allMyContents.push_back( make_pair(tmpSmo, "MediaObject") );
		}
	}

	cont->setContents(allMyContents);

	return cont;
}

SmilMediaObject* SmilParser::readMediaObject(xmlNode* node, const xmlChar* type) {
	SmilMediaObject* smo;

	smo = NULL;

	if( node==NULL ) {
		return NULL;
	}

	if( !xmlStrcmp(type, (const xmlChar*)"audio") ) {
		smo = new Audio();
		readMediaObjectProperty(smo, node);

	}
	else if( !xmlStrcmp(type, (const xmlChar*)"extra") ) {
			smo = new Extra();
			readMediaObjectProperty(smo, node);

	}
	else if( !xmlStrcmp(type, (const xmlChar*)"img") ) {
		smo = new Image();
		readMediaObjectProperty(smo, node);

	}
	else if( !xmlStrcmp(type, (const xmlChar*)"text") ) {
		smo = new Text();
		readMediaObjectProperty(smo, node);
	}
	else if( !xmlStrcmp(type, (const xmlChar*)"textstream") ) {
		smo = new TextStream();
		readMediaObjectProperty(smo, node);
	}
	else if( !xmlStrcmp(type, (const xmlChar*)"video") ) {
		smo = new Video();
		readMediaObjectProperty(smo, node);
	}
	else {
		cout << "[other] " << node->name << endl;
	}

	return smo;
}

bool SmilParser::readMediaObjectProperty(SmilMediaObject* obj, xmlNode* node) {
	xmlAttr *prop;
	string  propStr;
	int xc;

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			obj->setId( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"player") ) {
			obj->setPlayer(propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"src") ) {
			obj->setSrc(propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"loop") ) {
			obj->setLoop(propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"title") ) {
			obj->setTitle(propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"type") ) {
			obj->setType(propStr);
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"alt") ) {
			obj->setAlt(propStr);
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"clipBegin") ) {
			obj->setClipBegin(propStr);
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"clipEnd") ) {
			obj->setClipEnd(propStr);
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"dur") ) {
			obj->setDur(processTime(propStr));
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"begin") ) {
			obj->setBegin(processTime(propStr));
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"fps") ) {

			xc = processPosition( propStr ); //francesco
			if( !xc ) return NULL;

			obj->setFps(xc);
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"end") ) {
			obj->setEnd(processTime(propStr));
		}
        else if(!xmlStrcmp(prop->name, (const xmlChar*)"region") ) {
            obj->setWindowName(propStr);
        }
        else if(!xmlStrcmp(prop->name, (const xmlChar*)"backgroundClient") ) {
            obj->setBackgroundClient(propStr);
        }


		prop = prop->next;
	}

	return true;
}

bool SmilParser::readContainerProperty(Composite* obj, xmlNode* node) {
	xmlAttr *prop;
	string  propStr;

	prop = node->properties;

	while(prop && prop->name && prop->children) {
		propStr = string((const char*)xmlNodeListGetString(node->doc, prop->children, 1));

		if(!xmlStrcmp(prop->name, (const xmlChar*)"id") ) {
			obj->setId( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"title") ) {
			obj->setTitle( propStr );
		}
		else if(!xmlStrcmp(prop->name, (const xmlChar*)"repeatCount") ) {
			if( !propStr.compare("indefinite") ) {
				cout << "ripetizione indefinita" << endl;
				obj->setRepeat(-1);
			}
			else if( isdigit(propStr.c_str()[0] ) ) {
				obj->setRepeat( atof(propStr.c_str()) );
			}
			else {
				cout << "WARN: repeat value is not number\n";
			}
		}

		prop = prop->next;
	}

	return true;
}

/**
 * Analizza la stringa che contiene un tempo e lo restituisce in ms.
 */
long SmilParser::processTime(string strTime) {
	long msFactor = 1;

	if( !isdigit(strTime.c_str()[0])) {
		cerr <<"uncorrect time"<<endl;
		return 0;
	}

	if( strTime.find("ms")!=string::npos) {
		msFactor = 1;
		strTime.replace(strTime.find("ms"), strTime.find("ms"), "");
	}
	else if( strTime.find("s")!=string::npos) {
		msFactor = 1000;
		strTime.replace(strTime.find("s"), strTime.find("s"), "");
	}
	else if( strTime.find("m")!=string::npos) {
		msFactor = 60000;
		strTime.replace(strTime.find("m"), strTime.find("m"), "");
	}

	return atoi(strTime.c_str())*msFactor;
}

int SmilParser::processPosition(string strPos) {
    size_t pos;
	if( !isdigit(strPos.c_str()[0])) {
		cerr <<"uncorrect position"<<endl;
		return -1;
	}

    pos = strPos.find("px");
	if( pos!=string::npos) {
		strPos.replace(pos, pos+2, "");
	}

	return atoi(strPos.c_str());
}

bool SmilParser::checkTopLayoutParams(string key, string value) {

	if( !key.compare("open") ) {
		if( !value.compare("onStart") || !value.compare("whenActive") )
			return true;
	}
	else if( !key.compare("close") ) {
		if( !value.compare("onRequest") || !value.compare("whenNotActive") )
			return true;
	}

	return false;
}


