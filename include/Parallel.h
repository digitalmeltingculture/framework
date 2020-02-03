/*
 * Parallel.h
 *
 *  Created on: Nov 30, 2014
 *      Author: prisca-davide
 */
#include <list>
#ifndef PARALLEL_H_
#define PARALLEL_H_

class State;

class Parallel {
	private:
		string id;
		list<State*> states;

	public:
		Parallel();
		Parallel(string);
		Parallel(string, list<State*>);

		virtual ~Parallel();

		string getId();
		void setId(const string);

		list<State*> getStates();
		void setStates(list<State*> states);

		friend ostream& operator<<(ostream&, Parallel&);
};


#endif /* PARALLEL_H_ */
