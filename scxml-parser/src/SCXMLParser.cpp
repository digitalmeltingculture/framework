/*
 * scxmlParser.cpp
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#include<iostream>
#include<string>
#include<list>
#include<map>

using namespace std;

#include <libxml/xmlreader.h>

#include "../include/SCXMLException.h"
#include "../include/TmpTransition.h"
#include "../include/Transition.h"
#include "../include/Content.h"
#include "../include/State.h"
#include "../include/Parallel.h"
#include "../include/Root.h"

#include "../include/SCXMLParser.h"

SCXMLParser::SCXMLParser(string src){

	this->src = src;
    xmlDocPtr docPtr;


    if( (docPtr = xmlReadFile(src.c_str(), NULL, 0)) == NULL)
     	throw SCXMLException();

    if( (rootNode = xmlDocGetRootElement(docPtr)) == NULL)
		throw SCXMLException();


}

Root    SCXMLParser::parse() {
    SCXMLParser parser(this->src);
	Root root = parser.parseDocument();
    
	list<State*>* states = root.getStates();
	list<State*>::iterator iter;
    
    list<State*>allStatesRefs = parser.getAllStatesRefs();
    
    buildTransitions(allStatesRefs);
    
	states = root.getStates();
	list<Transition*> stateTransitions;
	list<Transition*>::iterator stateTransitionsIter;
    
    return root;
}

void SCXMLParser::buildTransitions(list<State*> allStatesRefs){
	list<State*>::iterator stateIter;
    
	list<TmpTransition> tmpTransitions;
	list<TmpTransition>::iterator tmpTransIter;
    
	Transition* transition=NULL;
	//list<Transition*> transList;
	State *statePtr=NULL, *tmp=NULL;
    
    
	for(stateIter = allStatesRefs.begin(); stateIter != allStatesRefs.end(); stateIter++){
		tmpTransitions = (*stateIter)->getTmpTransitions();
		if(!tmpTransitions.empty()){
			for(tmpTransIter = tmpTransitions.begin(); tmpTransIter != tmpTransitions.end(); ++tmpTransIter){
				if( (statePtr = getStateFromId(allStatesRefs, tmpTransIter->getTarget())) == NULL)
					throw SCXMLException();
				transition = new Transition(tmpTransIter->getEvent(), tmpTransIter->getCond(), statePtr);
				(*stateIter)->addTransition(transition);
			}
		}
	}
}

State* SCXMLParser::getStateFromId(list<State*> states, string id){
	list<State*>::iterator statesIter;
	State* result = NULL;
    
	statesIter = states.begin();
	while(statesIter != states.end() && !result){
		if( (*statesIter)->getId().compare(id) == 0)
			result = *statesIter;
		else
			statesIter++;
	}
	return result;
}

Content *SCXMLParser::parseDataNode(_xmlNode* nodePtr){
    /*SmilParser smilParser;
    Smil *tmpSmil = NULL;
    string tmpName;
	Content *c;
    
    if( !nodePtr->children )
        return NULL;
    
    c = new Content();
    
    while( nodePtr->children ) {
        tmpName = (char*)nodePtr->children->name;
        if( !tmpName.compare("smil") ) {
            tmpSmil = smilParser.readSmil(nodePtr->children->children);
        }
        
        nodePtr->children = nodePtr->children->next;
    }
    
    if( tmpSmil ) {
        if( Root::initialState )
            tmpSmil->setHead( (*(Root::initialState->getContent().begin())).getSmil()->getHead() );
        
        c->setSmil(tmpSmil);
    }
    
	return c;
	*/
	return NULL;
}

Root SCXMLParser::parseRootNode(_xmlNode* nodePtr){
	_xmlAttr* nodeAttr = NULL;
	string name,stateId;
	string tmpName;

	if((nodeAttr = nodePtr->properties) == NULL)
    	throw SCXMLException();

	while(nodeAttr!=NULL){
		tmpName = (char*)nodeAttr->name;
		if( tmpName.compare("name") == 0)
			name = (char*)nodeAttr->children->content;
		else if( tmpName.compare("initial") == 0)
			stateId = (char*)nodeAttr->children->content;

		nodeAttr = nodeAttr->next;
	}
	Root root(name,stateId);

	return root;
}

State* SCXMLParser::parseStateNode(_xmlNode* nodePtr){
	_xmlAttr* nodeAttr = NULL;
	string id = "";
	string type=""; //aggiunto da francesco
	string tmpName;
	State* state = NULL;

	if((nodeAttr = nodePtr->properties) == NULL)
    	throw SCXMLException();

	while(nodeAttr!=NULL){
		tmpName = (char*)nodeAttr->name;
		if( tmpName.compare("id") == 0)
			id = (char*)nodeAttr->children->content;
		//Aggiunto da franscesco Inizio
		if( tmpName.compare("type") == 0)
			type = (char*)nodeAttr->children->content;

		//Aggiunto da francesco Fine
		nodeAttr = nodeAttr->next;
	}

	state = new State(id,type);
	nodePtr = nodePtr->children;
	state = parseStateSubTree(nodePtr, state);

	return state;
}


list<Content> SCXMLParser::parseDataModelNode(_xmlNode* nodePtr){
	string tmpNodeName="";
    Content*      tmpContent;
	list<Content> dataModel;

	nodePtr = nodePtr->children;
	while(nodePtr != NULL){
		tmpNodeName = (char*)nodePtr->name;
		if(tmpNodeName.compare("data")==0){
            tmpContent = parseDataNode(nodePtr);
            if( tmpContent )
                dataModel.push_back(*tmpContent);
		}
		nodePtr = nodePtr->next;
	}
	return dataModel;
}

Parallel* SCXMLParser::parseParallelNode(_xmlNode* nodePtr){
	_xmlAttr* nodeAttr = NULL;
	string tmpNodeName = "", tmpAttrName="",id="";

	Parallel* parallel=NULL;
	State* state=NULL;
	list<State*> states;

	if((nodeAttr = nodePtr->properties) == NULL)
    	throw SCXMLException();

	while(nodeAttr!=NULL){
		tmpAttrName = (char*)nodeAttr->name;
		if( tmpAttrName.compare("id") == 0)
			id = (char*)nodeAttr->children->content;
		nodeAttr = nodeAttr->next;
	}

	nodePtr = nodePtr->children;
	while(nodePtr != NULL){
		tmpNodeName = (char*)nodePtr->name;
		if( tmpNodeName.compare("state") == 0){
			state = parseStateNode(nodePtr);
			states.push_back(state);

		}
		nodePtr = nodePtr->next;
	}

	parallel = new Parallel(id,states);
	return parallel;
}

TmpTransition SCXMLParser::parseTransitionNode(_xmlNode* nodePtr){
	_xmlAttr* nodeAttr = NULL;
	string cond, event, target;
	string tmpName;

	if((nodeAttr = nodePtr->properties) == NULL)
		throw SCXMLException();

	while(nodeAttr!=NULL){
		tmpName = (char*)nodeAttr->name;

		if( tmpName.compare("cond") == 0)
			cond = (char*)nodeAttr->children->content;
		else if( tmpName.compare("event") == 0)
			event = (char*)nodeAttr->children->content;
		else if( tmpName.compare("target") == 0)
			target = (char*)nodeAttr->children->content;

		nodeAttr = nodeAttr->next;
	}
	TmpTransition tmpTransition(event,cond,target);
	return tmpTransition;
}

State* SCXMLParser::parseStateSubTree(_xmlNode* nodePtr, State* state){
	list<Content> dataModel;
	string tmpNodeName="", tmpNodeContent="";

	while(nodePtr != NULL){
		tmpNodeName = (char*)nodePtr->name;
		if( tmpNodeName.compare("parallel") == 0){
			state->setParallel(parseParallelNode(nodePtr));
		}
		else if(tmpNodeName.compare("datamodel") == 0){
			state->setContent(parseDataModelNode(nodePtr));
		}
		else if(tmpNodeName.compare("transition") == 0){
			state->addTmpTransition(parseTransitionNode(nodePtr));
		}
		else if(tmpNodeName.compare("onentry") == 0){
            if( nodePtr->children && nodePtr->children->content ) {
                tmpNodeContent = (char*)nodePtr->children->content;
                state->setOnentry(tmpNodeContent);
            }
		}
		else if(tmpNodeName.compare("onexit") == 0){
            if( nodePtr->children && nodePtr->children->content ) {
                tmpNodeContent = (char*)nodePtr->children->content;
                state->setOnexit(tmpNodeContent);
            }
		}
		else if(tmpNodeName.compare("state") == 0){
            allStatesRefs.push_back(this->parseStateNode(nodePtr));
            
			//state->addTmpTransition(parseTransitionNode(nodePtr));
		}
		nodePtr = nodePtr->next;
	}
	allStatesRefs.push_back(state);
	return state;
}

Root SCXMLParser::parseDocument(){
	Root root;
	State* tmpState = NULL;
	_xmlNode* nodePtr = NULL;
	string tmpNodeName="";

	list<State*> *tmpStates = new list<State*>();
	if(!rootNode)
		throw SCXMLException();

	root = parseRootNode(rootNode);

	nodePtr = rootNode->children;
	while(nodePtr != NULL){
		tmpNodeName = (char*)nodePtr->name;
		if( tmpNodeName.compare("state") == 0){
			tmpState = parseStateNode(nodePtr);
            root.addState(tmpState);
//			tmpStates->push_back(tmpState);
		}
		nodePtr = nodePtr->next;
	}
//	root.setStates(tmpStates);
    
	return root;
}
