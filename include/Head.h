/*
 * Head.h
 *
 *  Created on: 05/dic/2014
 *      Author: dariux881
 */

#ifndef HEAD_H_
#define HEAD_H_

#include <list>
#include "TopLayout.h"

using namespace std;

class Head {
public:
	Head();
	virtual ~Head();

	void addTopLayout(TopLayout* newTL ) {
		allWindows.push_back(newTL);
	}

	void setTopLayouts(list<TopLayout*> children) {
		allWindows = children;
	}

	list<TopLayout*> getTopLayouts() {
		return allWindows;
	}

	void printHead() {
		list<TopLayout*>::iterator tpIt;

		cout << "<head>\n\t<layout>" << endl;

		tpIt = allWindows.begin();
		(*tpIt)->getParent()->print();

		for(tpIt=allWindows.begin(); tpIt!=allWindows.end(); tpIt++) {
			(*tpIt)->print();
		}

		cout << "\t</layout>\n</head>" << endl;
	}

private:
	//Layouts
	list<TopLayout*> allWindows;
};

#endif /* HEAD_H_ */
