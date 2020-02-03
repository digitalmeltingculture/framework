/*
 * Root.h
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#ifndef ROOT_H_
#define ROOT_H_

class Root {
private:
    string name;
    string initial;
    list<State*>* states;
    
public:
    Root(){
        states = NULL;
        initial = "";
        name = "";
    }
    Root(string name, string initialStateId){
        this->name = name;
        this->initial = initialStateId;
        states = NULL;
    }
    virtual ~Root();
    friend ostream& operator<<(ostream& os, const Root& root);
    
    string getInitial(){
        return initial;
    }
    
    void setInitial(string initial) {
        this->initial = initial;
    }
    
    string getName(){
        return name;
    }
    
    void setName(string name) {
        this->name = name;
    }
    
    list<State*>* getStates(){
        return states;
    }
    
    void addState(State* newState) {
        if( !states )
            states = new list<State*>();
        
        states->push_back(newState);
        
        initialState = getInitialState();
    }
    
    void setStates(list<State*>* states) {
        this->states = states;
    }
    
    State* getInitialState();
    State* getStateById(string id);
    
    static State* initialState;
};

#endif /* ROOT_H_ */
