/*
 * Content.h
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#include<iostream>
#include<string>
#include<list>
#include<map>

#ifndef CONTENT_H_
#define CONTENT_H_

using namespace std;

//class Smil;

class Content {
//private:
    //Smil   *myContent;
    
    //static void* playContent_helper(void* );
    
public:
	Content();
	 /*Content(Smil* smil){
	 	this->myContent = smil;
	 }*/
    
	friend ostream& operator<<(ostream&, const Content&);

	virtual ~Content();

    void playContent();
    
	 /*Smil* getSmil(){
	 	return myContent;
	 }

	 void setSmil(Smil* smil) {
	 	this->myContent = smil;
	 }*/
};


#endif /* CONTENT_H_ */
