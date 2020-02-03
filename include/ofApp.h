#pragma once

//#define _USE_KINECT_

//#include "ofMain.h"
//#include "ofxBlobsManager.h"
#include "Floor.h"
#include "Utils.h"
//#include "ofxOpenCv.h"
#include "UserOnFloor.h"
#include "Configuration.h"
#include "historyChina.h"
#include "Parallel.h"
#include "Observer.h"
#include "Observable.h"
#include "GestureType.h"

#include "GestureRecognition.h"
#include "Floor.h"
#include "Configuration.h"
#include <vector>

#include "TmpTransition.h"
#include "Controller.h"
#include "Transition.h"
#include "Content.h"
#include "State.h"
#include "Root.h"
#include "TopLayout.h"
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include "Constants.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>


#ifdef _USE_KINECT_
	#include "ofxKinect.h"
#else
	#include "kinect.h"
#endif

#define  RESIZE_WIDTH 640
#define  RESIZE_HEIGHT 480

#define  WINDOW_SIZE RESIZE_HEIGHT
#define  MAX_USER_NUMBER 6
#define  GESTURE_TIMER 2000;

#define NO_LEADER -1
#define REG1 1
#define REG2 2
#define REG3 3
#define REG4 4
#define REG5 5

using namespace std;
using namespace cv;

class ofApp : /*public ofBaseApp,*/ public Observer {

public:

	static list<int> contInExe;


	string type;
	string src;


	ofApp(string s);
	ofApp(string s1,string s2);
	Mat initMask(int,Mat);
	Point initPivot(int client );
	int kinectOne();
	int video();
	static Mat provaDivisione(int,Mat);
	static vector<Point> drawContoursMat1(Mat frame);
	static vector<Point> drawContoursMat2(Mat frame);

	vector<Point> drawContursInit(vector<vector<Point>>,Point);
	void drawRoi(Mat img);
	vector<Point> drawContursFinal(Mat frame, vector<Point> minPoint,vector<vector<Point>>contours,Point);
	Mat tagliaMat(Mat frame,int *a, int *w, int* x1,int *y1,Point minPoint,int valMez,int val,Point);
	vector <Point> costruisciCentroide(Mat frame,Point);


	static Point contorni2(Mat frame);

	void setListController();

	void setStateThread();
	string convertIntToString(int val);
	 static void* handleTransition(void*);

	 //void analyzeTransitionUsers_thread(State * cState,int client,string clientString,CvPoint pos,int *threadTime);
	 void analyzeTransitionUsers_thread(Controller *,string ,CvPoint ,int *);
	 void changeStateUsingEvent(string c,State *cState);
	 void changeStateUsingEventAndCondition(string c,string cond,State*cState);
	 void transitToNextState(State* cState,State* nState);
	 void processState(State * cState);
	 void onStateEntry(State *cstate) ;

	 static list<pair<int, int> > allTimer;
	 void setFlagTimer();
	 static void updateState(State* cState,int cClient);
	 static void updateFlagTimer(int cClient,int val);
	 static int extractFlagTimer(int client);
	 static State* extractState(int client);
	 void update_new() ;
	 static void* handleThreadUpdate(void*);


	 int convetStringToInt(string val,bool * );
	 int calcoloPercentuale(int  , int );
	 void printAllRegions();
	 string isRoi(int client,int x, int y);



	time_t startTimer(void);
	int isTimerFinished(time_t firstValue, int maxValue);
	ofApp();
    void setRoot(Root root);
    static int  getClientFromLayout(TopLayout* layout);
    
    void setup();
    void update();
    void draw();
    void exit();

    void onContentEvent(string event);
    
    void keyPressed(int key);
    void windowResized(int w, int h);

    virtual void onObservableUpdate(GestureType_t* data) {
        //ofApp::myOnObservableUpdate(data);
    }
    
    static ofApp* ofAppInstance;
    static vector<TopLayout*,int> windows;

    void closeSoket(){
    	close(this->myFloor->sockfd);
    }


private:
    bool        isFloorFull();
    int         detectPeople();
    void        fillLeaderWindow(int, int);
    //ofPoint     detectHeadCentroid(int, int);
    
    // Manage clients
    bool        checkClients();
    void        setupClients();
    
    // Gesture detection and condition evaluation
    void        myOnObservableUpdate(GestureType_t* data);
    void        analyzeTransition(GestureType_t*);
    void        analyzeTransitionUsers();
    bool        isConditionValid(string, GestureType_t*);
    bool        checkUserCount(string);
    bool        checkGesturePosition(string, int x, int y);
    bool        checkGestureArea(string, int x, int y);

    int         getPositionClass(CvPoint p, CvRect r);
    
    // Manage transitions
    void        processState();
    void        transitToNextState();
    void        onStateEntry();
    void        onStateExit();
    
    int         leaderId, leaderBlobPos;
    double      minThresholdValue, maxThresholdValue, thresholdValue;
    History     *centroidX, *centroidY;
    HistoryChina  *posClassHistory;
    
    uint        previousUserNumber;
    int         kinectAngle;

#ifdef _USE_KINECT_
    ofxKinect kinect;

	#ifdef USE_TWO_KINECTS
		ofxKinect kinect2;
	#endif
#else
    Kinect kinect;
#endif

    vector<UserOnFloor> users;
    string     gestureMsg, clientMsg, msg;

    Root        interactionManager;

    static list<pair<TopLayout*, int> > allWindows;
    static list<pair<State*, int> > allUserStates;
    static list<Controller* > allController;


    State       *currentState, *nextState,*currentState2, *nextState2;
    string      prevGestureType;
    int         requestedClients;
    bool        clientsAreOk;
    double      lastGestureTimestamp;
    
    int x_offset, y_offset, width_offset, height_offset;
    int         frameRate;
    
    GestureRecognition* gRecog;
    Floor* myFloor;
    Configuration *conf;
    GestureType_t* contentEvent;
    
    pthread_mutex_t contentMutex;
    
    bool gestureInteractionEnabled;
    bool bLearnBakground;
    bool blobFinding;
    bool showFloor;
};
