//
//  GestureType.h
//  PaSt
//
//  Created by Dario Di Mauro on 05/09/14.
//
//

#ifndef PaSt_GestureType_h
#define PaSt_GestureType_h

typedef struct GestureType {
public:
    string  type;
    int     x, y;
    long    ts;
    char    hand;
    char    orientation;
    //ofxCvGrayscaleImage *img;
    
} GestureType_t;

#endif
