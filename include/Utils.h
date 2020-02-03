#ifndef _UTILS_
#define _UTILS_

#ifndef PI
#define PI 3.14149265
#define TO_RAD PI/180
#define TO_DEG 180/PI
#endif

//#include "ofMain.h"
//#include "ofxOpenCv.h"
#include <vector>
#include <iostream>

using namespace std;

class Utils {
public:
    template<typename T>
    static T                    min(T a, T b) {
        return std::min(a, b);
    }
    template<typename T>
    static T                    max(T a, T b) {
        if( a>b )
            return a;
        else
            return b;
//        return std::max(a, b);
    }
    
    static int                  indexOfMinValue(int, std::vector<float>);
    //static int                  OneNearestNeighborsIndex(ofPoint, ofxCvContourFinder);
    
   // static float                squaredEuclideanDistance(ofPoint, ofPoint);
   // static std::vector<float>   searchDistances(ofPoint, ofxCvContourFinder);
    
   // static float                angleBetweenVectors(ofPoint, ofPoint);
   // static float                angleBetween3Points(ofPoint, ofPoint, ofPoint);
   // static ofPoint              rotatePoint(double angle, ofPoint p, ofPoint center=ofPoint(0,0,0));
    
    static float                degreeToRadians(float);
    static float                radiansToDegree(float);
    
    static float                lowPass(float newValue, float oldValue, float smoothFactor, float threshold);
    static void                 swap(float*a, float*b);
    
   // static uchar*               rangeContrastStretch(IplImage* src, IplImage* dst, double minPixVal, double maxPixVal, double finalMinVal = 0, double finalMaxVal = 255);
};

#endif
