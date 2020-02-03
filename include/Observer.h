//
//  Observer.h
//  PaSt
//
//  Created by Dario Di Mauro on 04/09/14.
//
//

#ifndef PaSt_Observer_h
#define PaSt_Observer_h

#include <iostream>
#include "GestureType.h"

class Observer {
public:
    virtual void onObservableUpdate(GestureType_t*) = 0;
};

#endif
