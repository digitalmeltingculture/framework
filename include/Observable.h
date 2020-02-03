//
//  Observable.h
//  PaSt
//
//  Created by Dario Di Mauro on 04/09/14.
//
//

#ifndef PaSt_Observable_h
#define PaSt_Observable_h

#include "Observer.h"
#include "GestureType.h"

class Observer;

class Observable {
public:
    virtual void registerObserver(Observer*) = 0;
    virtual void notifyAll(GestureType_t*) = 0;
};


#endif
