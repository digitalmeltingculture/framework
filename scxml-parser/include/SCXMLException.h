/*
 * SCXMLException.h
 *
 *  Created on: Nov 30, 2014
 *      Author: prisca-davide
 */

#include<iostream>
#include<string>
#include<list>
#include<map>

#ifndef SCXMLEXCEPTION_H_
#define SCXMLEXCEPTION_H_


class SCXMLException: public std::exception{
	virtual const char* what() const throw(){
	return "SCXML Exception";
	}
};

#endif /* SCXMLEXCEPTION_H_ */
