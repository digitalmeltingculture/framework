/*
 * scxmlParser.h
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#ifndef SCXMLPARSER_H_
#define SCXMLPARSER_H_

class SCXMLParser{
private:
    string src;
    string name;
    _xmlNode *rootNode;
    list<State*> allStatesRefs;
    
    void buildTransitions(list<State*> allStatesRefs);
    State* getStateFromId(list<State*> states, string id);
    
public:
    SCXMLParser(string);
    
    string getName(){
        return name;
    }
    
    void setName(string name) {
        this->name = name;
    }
    
    _xmlNode* getRootNode(){
        return rootNode;
    }
    
    void setRootNode(_xmlNode* rootNode) {
        this->rootNode = rootNode;
    }
    
    void setSrc(string src) {
        this->src = src;
    }
    
    string getSrc(){
        return src;
    }
    
    Content *parseDataNode(_xmlNode*);
    list<Content> parseDataModelNode(_xmlNode*);
    TmpTransition parseTransitionNode(_xmlNode*);
    
    State* parseStateNode(_xmlNode*);
    Parallel* parseParallelNode(_xmlNode*);
    
    State* parseStateSubTree(_xmlNode*, State*);
    Root parseRootNode(_xmlNode*);
    Root parseDocument();
    Root parse();
    
    list<State*> getAllStatesRefs(){return allStatesRefs;}
};



#endif /* SCXMLPARSER_H_ */
