/*
 * Transition.h
 *
 *  Created on: Dec 1, 2014
 *      Author: prisca-davide
 */

#ifndef TRANSITION_H_
#define TRANSITION_H_

class State;
class Transition{
	private:
		string event;
		string cond;
		State* target;

	public:
		Transition(string,State*);
		Transition(string,string,State*);

		string getEvent();
		string getCond();
		State* getTarget();

		void setEvent(string);
		void setCond(string);
		void setTarget(State*);
		friend ostream& operator<<(ostream&, Transition&);
};



#endif /* TRANSITION_H_ */
