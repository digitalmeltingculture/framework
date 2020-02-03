/*
 * State.h
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#ifndef STATE_H_
#define STATE_H_

class Parallel;
class Transition;

class State{
	private:
		string type ; //Aggiunto da francesco
		string id;
		list<Content> content;
		Parallel* parallel;
		string onentry;
		string onexit;
		list<TmpTransition> tmpTransitions;
		list<Transition*> transitions;
        list<State*> states;

	public:

        //---Inizio Blocco Aggiunto da francesco
        const string getType();

        void setType(const string type) ;

        State(string,string);
        //----Fine Blocco Aggiunto da francesco

		State(string, Parallel*);
		virtual ~State(){}
		list<Content> getContent();
		void setContent(const list<Content> content);
		const string getId();
		void setId(const string id);
		string getOnentry();
		void setOnentry(const string onEntryAction);
		string getOnexit();
		void setOnexit(const string onExitAction);
		Parallel* getParallel();
		void setParallel(Parallel* parallel);
		list<TmpTransition> getTmpTransitions();
		void setTmpTransitions(list<TmpTransition> tmpTransitions);
		void addTmpTransition(TmpTransition trans);
		list<Transition*> getTransitions();
		void setTransitions(list<Transition*> transitions);
		void addTransition(Transition* transition);
		bool isSubState(State*);
		friend ostream& operator<<(ostream& os, State& state);

};
#endif /* STATE_H_ */
