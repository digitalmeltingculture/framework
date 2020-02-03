
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Root;
class Floor;
class State;

class Controller {

	public:

		void sendContentParallel() ;

		void analyzeTransitionUser();
		void transitToNextState();
		void processState();
		void onStateEntry();
		void onStateExit();

		Controller(int id,State*,Root *);
		void changeState(string,string);

		void setCurrentState(State* currentState);
		State* getCurrentState();
		void setNextState(State* nextState);
		State* getNextState();
		void setParallel(bool parallel);
		bool getParallel();
		int getId();
		void setId(int id);
		void setFloor(Floor* );
		Floor* getFloor();
		void gestioneRelazioneSottostati();
		void gestioneParentela();
		void setParentela(int );
		int getParentela();

		void sendFlagParentela();


	private:
		State* currentState;
		State* nextState;
		State* precedentState;
		bool parallel=false;
		int parentela=0;
		int id;
		Floor* myFloor;
		Root* initState;

};

#endif
