/*
 * TmpTransition.h
 *
 *  Created on: Dec 1, 2014
 *      Author: prisca-davide
 */

#ifndef TMPTRANSITION_H_
#define TMPTRANSITION_H_

using namespace std;
class TmpTransition{
	private:
		string event;
		string cond;
		string target;

	public:
		TmpTransition(string,string);
		TmpTransition(string,string,string);

		string getEvent();
		string getCond();
		string getTarget();

		void setEvent(string);
		void setCond(string);
		void setTarget(string);
		friend ostream& operator<<(ostream&, const TmpTransition&);
};

#endif /* TMPTRANSITION_H_ */
