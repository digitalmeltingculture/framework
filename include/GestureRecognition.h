//
//  GestureRecognition.h
//  PaSt
//
//  Created by Dario Di Mauro on 04/09/14.
//
//

#ifndef PaSt_GestureRecognition_h
#define PaSt_GestureRecognition_h

#define NO_GESTURE "noGesture"
#define ARM_LEFT_TO_RIGHT "gestureLefttoRight"
#define ARM_RIGHT_TO_LEFT "gestureRighttoLeft"
#define ZOOM_IN "gestureZoomin"
#define ZOOM_OUT "gestureZoomout"
#define LEADER_IN "leaderIn"
#define LEADER_OUT "leaderOut"
#define LEADER_CHANGED "leaderChanged"
#define LEADER_IN_CLASS "leaderInClass"

#define UNDEFINED_HAND  0x10
#define LEFT_HAND       0x11
#define RIGHT_HAND      0x12
#define BOTH_HANDS      0x13
#define NO_HANDS        0x14
#define ORIENTATION_UNDEFINED 0x20
#define ORIENTATION_FRONT 0x21
#define ORIENTATION_REAR  0x22

//#include "ofMain.h"
//#include "ofxBlobsManager.h"
//#include "ofxOpenCv.h"

#include <utility>

#include "Utils.h"
#include "Observable.h"
#include "Observer.h"
#include "History.h"

#include "GestureType.h"
#include "Configuration.h"

#define  GR_RESIZE_WIDTH 640//320
#define  GR_RESIZE_HEIGHT 480//240

class GestureRecognition : public Observable {
public:
    static  GestureRecognition* getInstance();
    //void    updateImage(ofxCvGrayscaleImage newImage, ofPoint centroid = ofPoint(0));
    void    closeDatasetFile();
    void    resetImage();
    void    printHand(char versus);
    string  handToString(char versus);
    
    // Observer pattern
    virtual void registerObserver( Observer* obs) {
        GestureRecognition::myRegisterObserver(obs);
    }
    
    static string userMove;
    static string userCount;
    static string userNotEmpty;
    static string gesturePosition;
    static string gestureArea;
    
private:
    /*  ++++++++++++++  METHODS  ++++++++++++++  */
    GestureRecognition();
    // Gesture Recognition
    //vector<ofPoint*>	detectHands(ofxCvBlob);
    void                processIdle();
    //bool                calculateEllipse( vector <ofPoint>  & contour, CvBox2D32f & box, bool updateAngle = true );
    void                calculateMaxDistance();
    //void                manageSingleHandGesture(ofPoint);
    //void                manageTwoHandsGesture(vector<ofPoint*>, ofPoint);
   // bool                trackBrowseGesture();
    void                myRegisterObserver(Observer*);

    //char                analyzeHand(ofPoint *hand, ofPoint centroid, CvBox2D box, float minDistance, float bodyThickness);
    //vector<ofPoint*>    analyzeTwoHands(ofPoint *h1, ofPoint *h2, ofPoint centroid, CvBox2D box, float bodyThickness);
    //void                declareUndefined(ofPoint newPoint, char versus);
    //void                pushInHistory(ofPoint, char);
    //void                pushInHistoryTwoHands(ofPoint left, ofPoint right);
    void                createDataset(int label = 0);
    
    // Support functions
    //bool                fitEllipse( vector <ofPoint>  & contour, CvBox2D32f & box );
    
    // Observer pattern
    void                myNotifyAll(GestureType_t*);
    virtual void        notifyAll(GestureType_t* data) {
        GestureRecognition::myNotifyAll(data);
    }
    
    /*  ++++++++++++++  ATTRIBUTES  ++++++++++++++  */
    // Observer pattern
    vector<Observer*> observers;
    //CvBox2D box, leaderBox;

    static GestureRecognition* instance;
    
    //ofxCvGrayscaleImage   leaderImage;
    //ofxCvBlob             leaderBlob;
    int                   previousDetectedHands;
    
    // BODY DIRECTION DETECTION PARAMETER
    int     bodyMinorAxis;

    /* GESTURE RECOGNITION PARAMETERS */
    int		minDistanceToCentroid; 	// in pixel
    int		minAngularVariation; 	// In degree
    int		maxAngularVariation; 	// In degree
    int		minAngularVariation_2; 	// In degree
    int		maxAngularVariation_2; 	// In degree
    int		minTimeElapsed;		 	// In ms
    int		maxTimeElapsed;			// In ms

    /* GENERAL GESTURE RECOGNITION OBJECTS */
    double      startTrackingTime1, stopTrackingTime1;
    double      startTrackingTime2, stopTrackingTime2;
    //ofPoint     directionPoint1, directionPoint2;
    //ofPoint     oldLeaderPosition;
    //vector<ofPoint*>   handVect;
    //list<pair<ofPoint, ofPoint> > historyHands;
    History     *angleHistory,
                *centerX, *centerY;
    int         gesture_dataset_type;
    string      user_code;

    /* SINGLE HAND GESTURE */
    //ofPoint     startGesturePoint, endGesturePoint;
    char        versusGesture;
    char        orientationGesture;
    //list<ofPoint> undefinedHand;
    
    /* TWO HANDS GESTURE */
    float       minAngle, maxAngle, twoHandsAngle;
    double      minAngleTime, maxAngleTime;
    //list<pair<ofPoint, ofPoint> > undefinedTwoHands;
    /* END GESTURE RECOGNITION */
    
    ofstream myfile;
};

#endif
