using namespace std;

#include <iostream>
#include <string>

#include <list>
#include <vector>
#include <utility>
#include <memory> //allocate
#include <sstream>
#include <cerrno>
#include <time.h>
#include <signal.h>

#include "ofApp.h"
#include "Smil.h"
#include "Content.h"
#include "Controller.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc_c.h>

#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <libfreenect2/libfreenect2.hpp>
#include <libfreenect2/frame_listener_impl.h>
#include <libfreenect2/registration.h>
#include <libfreenect2/packet_pipeline.h>
#include <libfreenect2/logger.h>

using namespace cv;
using namespace std;

int isFinished=0;
int idFrame=0;
int initMaskValue=0;

bool protonect_shutdown=false;

list<int> ofApp::contInExe;


time_t seconds;
int lostingTimer = 0;
int inputEvent=0;
int timerStarted=0;

list<Controller*> ofApp::allController;
list<pair<TopLayout*, int> > ofApp::allWindows;
list<pair<State*, int> > ofApp::allUserStates;
list<pair<int, int> > ofApp::allTimer;

ofApp* ofApp::ofAppInstance;

Mat frameInit;


void sigint_handler(int s)
{
  protonect_shutdown = true;
}

Mat ofApp:: initMask(int client, Mat frame){

	list<pair<TopLayout*, int> >::iterator it;
	list<Mask*> tmpMask;
	list<Mask*>::iterator it2;

	Mat tmp;

	bool trovato=false;
	for(it=this->allWindows.begin();it!=this->allWindows.end();it++)
	{
		if((*it).second==client)
		{
			tmpMask=(*it).first->getChildrenMask();
			trovato=false;

			for(it2=tmpMask.begin();it2!= tmpMask.end();it2++)
			{
				trovato=true;

				tmp=imread((*it2)->getSrc(), IMREAD_GRAYSCALE );

				if(!tmp.data){
					cout<<"Maschera non trovata... verrà Aplicata Maschera di default"<<endl;
					Mat m(frame.rows,frame.cols, CV_8UC1, Scalar(255,255,255));
					return m;
				}
				break;
			}

			if(!trovato){
				Mat m(frame.rows,frame.cols, CV_8UC1, Scalar(255,255,255));
				return m;
			}
		}
	}

	return tmp;
}


Point ofApp::initPivot(int client ){

	list<pair<TopLayout*, int> >::iterator it;
	list<Mask*> tmpMask;
	list<Mask*>::iterator it2;
	Point pivot;

	Mat tmp;

	bool trovato=false;
	for(it=this->allWindows.begin();it!=this->allWindows.end();it++)
	{
		if((*it).second==client)
		{
			tmpMask=(*it).first->getChildrenMask();
			trovato=false;

			for(it2=tmpMask.begin();it2!= tmpMask.end();it2++)
			{

				trovato=true;
				pivot.x=(*it2)->getPivotXInt();
				pivot.y=(*it2)->getPivotYInt();
				return pivot;
			}

			if(!trovato){
				pivot.x=0;
				pivot.y=0;
				return pivot;
			}
		}
	}


}


Mat filtro(Mat src){
	Mat dst;

	bilateralFilter ( src, dst,9, 100,100 );
	return dst;
}


Mat initRectMask(int rows, int columns){

	Mat whiteRectangle;

	whiteRectangle = Mat::zeros(rows,columns,CV_8UC1);
	rectangle(whiteRectangle,Point(0,rows*0.3),Point(columns,rows),Scalar(255,255,255),-1);

	return whiteRectangle;
}


Mat preparationContours(Mat thresholdFrame,Mat circularMask){

	Mat black,outDoubleMask,rectMask;

	rectMask=initRectMask(circularMask.rows,circularMask.cols);

	black= Mat::zeros(circularMask.rows,circularMask.cols,CV_8UC1);
	thresholdFrame.copyTo(black,circularMask);

	black.copyTo(outDoubleMask,rectMask);

	return outDoubleMask;
}


Mat processFrame(Mat frame,int *flag,double *min, double *max,double *avg,Point *pointMin, Point*pointMax,Mat mask) {

	Mat grayFrame, normalizedFrame, equalizedFrame, filteredFrame,thresholdFrame,equalizedFrameFilter;

	if(frame.type()!= 0){  //Video
		cvtColor( frame, grayFrame, CV_BGR2GRAY );
		normalize(grayFrame,normalizedFrame,0,255,NORM_MINMAX);
	}
	else{
		normalize(frame,normalizedFrame,0,255,NORM_MINMAX); //kinect
	}

   	equalizedFrameFilter =filtro(normalizedFrame);
   	equalizedFrameFilter.copyTo(filteredFrame,mask);

   	if((*flag)==0){
    	minMaxLoc(filteredFrame,min,max,pointMin,pointMax,mask);
    	*avg=(*min+*max)*0.5;

    	if((*min)==0) *min=5;
    	cout<<"Min="<<*min<<" Max= "<<*max<<endl;
    	cout<<"pointMin( "<<pointMin->x<<" , "<<pointMin->y<<" )"<<endl;
    	*flag=1;
    }

    threshold(filteredFrame,thresholdFrame,*min,255,CV_THRESH_BINARY_INV);

    return thresholdFrame;
}

string ofApp::isRoi(int client,int x, int y){

	list<pair<TopLayout*, int> >::iterator it;
	list<Roi*> cnt;
	list<Roi*>::iterator it2;
	int i;

	int left, top,width,height;

	for(it=allWindows.begin();it!= allWindows.end();it++){
		if((*it).second==client){
			cnt=(*it).first->getChildrenRoi();
			i=0;
			for(it2=cnt.begin();it2!=cnt.end();it2++){
				left=(*it2)->getLeftInt();
				top=(*it2)->getTopInt();
				width=(*it2)->getWidthInt();
				height=(*it2)->getHeightInt();

				if(((x<=(left+width))&&(x>=left))&&((y<=(top+height))&&(y>=top))){
					return (*it2)->getId();
				}
				i++;
			}
		}
	}

	return NO_ROI;
}
//-----------------------------------------------------------------
void ofApp::setListController(){

	Controller *c;

	for(int i=0;i<allWindows.size();i++){
		c=new Controller(i,currentState,&interactionManager);
		this->allController.push_back(c);
	}
}

//--------------------------------------------------------------

ofApp::ofApp(string tipo) {

	this->type=tipo;
    ofAppInstance = this;
    posClassHistory = new HistoryChina(70);
    posClassHistory->initHistoryBuffers();
    //contentEvent = NULL;
    //pthread_mutex_init(&contentMutex, NULL);
    conf = Configuration::getInstance();
}


ofApp::ofApp(string tipo,string src) {

	this->type=tipo;
	this->src=src;

    ofAppInstance = this;
    posClassHistory = new HistoryChina(70);
    posClassHistory->initHistoryBuffers();
    //contentEvent = NULL;
    //pthread_mutex_init(&contentMutex, NULL);

    conf = Configuration::getInstance();
}

ofApp::ofApp() {

    ofAppInstance = this;
    
    posClassHistory = new HistoryChina(70);
    posClassHistory->initHistoryBuffers();
    
    //contentEvent = NULL;
    //pthread_mutex_init(&contentMutex, NULL);

    conf = Configuration::getInstance();
}

//--------------------------------------------------------------
void ofApp::setRoot(Root root) {


	int flagClient=conf->clientDifferentMachines;    //flagClient = 0 client sulla stessa macchina
						 	 	 	 	 	 	 	 //flagClient = 1 client macchine differenti

	list<TopLayout*> tmpList;
    list<TopLayout*>::iterator it;
    Smil tmp;

    int *pids;

    interactionManager = root;

    list<State*>* lS= interactionManager.getStates();
    list<State*>::iterator it5;

    currentState = NULL;
    nextState = interactionManager.getInitialState();
    transitToNextState();
    
    tmp = *(*(currentState->getContent().begin())).getSmil();
    tmpList = tmp.getHead()->getTopLayouts();
    
    for( it=tmpList.begin(); it!=tmpList.end(); it++) {
    	allWindows.push_back(make_pair(*it, -1) );
    }
    
    cout << "starting " << allWindows.size() << " clients" << endl;

    if(flagClient==0){
    	pids = new int[allWindows.size()];

    	for(int i=0; i<allWindows.size(); i++ ) {
    		pids[i] = fork();
    		if( pids[i]==0 ) { //Figlio

    			const char* clientPath =conf->clientBasePath.c_str();

    			if(execl(clientPath, "client", NULL)==-1)
    				cout<<"ERRORE execl >>--> "<<strerror(errno)<<endl;
    		}
    	}
    }
}
//--------------------------------------------------------------
int ofApp::getClientFromLayout(TopLayout* layout) {
    list<pair<TopLayout*, int> >::iterator it;
    
    if( !layout ) {
        if( allWindows.size()!=0 )
            return (*allWindows.begin()).second;
    }
    else {
        for( it=allWindows.begin(); it!=allWindows.end(); it++ ) {
            if( (*it).first==layout )
                return (*it).second;
        }
    }
    return -1;
}
//--------------------------------------------------------------
void ofApp::setup() {

	myFloor = Floor::getInstance(conf->serverIp.c_str(), conf->serverPort, conf->serverPort2);


    requestedClients = allWindows.size();
    myFloor->setMaxNumberOfClients(requestedClients);
    clientsAreOk = false;


    setListController();
}

//--------------------------------------------------------------
/*void ofApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
    
    gRecog->closeDatasetFile();

#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}*/


//--------------------------------------------------------------
void ofApp::transitToNextState() {
    // Assumo che nextState sia stato già aggiornato
    if( currentState ) {
        onStateExit();
    }

    cout << "transit";

    if( currentState )
        cout << " from " << currentState->getId();
    if( nextState )
        cout << " to " << nextState->getId() ;

    cout << endl;

    currentState = nextState;
    processState();
}

//--------------------------------------------------------------
void ofApp::processState() {
    onStateEntry();

    // fai quello che devi fare
}

//--------------------------------------------------------------
void ofApp::onStateEntry() {
    blobFinding = true;

    if( clientsAreOk && currentState && !currentState->getOnentry().compare("loadContent") ) {
       	(*(currentState->getContent().begin())).playContent();
    }
}

//--------------------------------------------------------------
void ofApp::onStateExit() {

}
//--------------------------------------------------------------
void ofApp::update_new() {

	//list<pair<State*, int> >::iterator it;
	list<Controller*>::iterator it;

	pthread_t tid[allUserStates.size()];
	int i=0;
    long j=0;

    while(!checkClients());{
    	cout<< "connected: " << myFloor->getNumberOfClients() <<"/" << requestedClients<<endl;
    }


    for(it=this->allController.begin();it!=  this->allController.end();it++){
    	pthread_create(&tid[i],NULL,handleThreadUpdate,(void*)(*it));
		i++;
	}


    if(!type.compare(KINECT_360)){

    }
    else if(!type.compare(KINECT_ONE)){
    	isFinished = kinectOne();
    }
    else if(!type.compare(VIDEO)){
    	isFinished = video();
    }

	i=0;

	for( it=allController.begin(); it!=allController.end(); it++ ){
		pthread_join(tid[i],NULL);
		i++;
	}

	close(this->myFloor->getSock());
	vector<int> sock = this->myFloor->getClients();

	for(int j=0;j<sock.size();j++){
		close(sock[j]);
	}
}


int ofApp::video(){

	Mat frame,thresholdFrame,mask, dst, q;
	double min,max,avg;
	Point pointMin,pointMax;

	bool stop,flagMask;
	int flagMinMax;
	char keyboard;

	VideoCapture inputVideo(this->src.c_str());
	if(!inputVideo.isOpened()){
	    cout << "(SERVER)Video Not FOUND... " << endl;
	    return -1;
	}

	idFrame=0;
	keyboard = 0;
	flagMinMax=0;
	stop=false;
	flagMask=false;


	mask=initMask(initMaskValue,frame);

	while( keyboard != 'q' && keyboard != 27 )
	{
		if(!stop)
	    {
			if(!inputVideo.read(frame)) {
	    	    cout << "(Server)Fine Video..." << endl;
	    	    break;
	    	}
	    }

		thresholdFrame=processFrame(frame,&flagMinMax,&min,&max,&avg,&pointMin,&pointMax,mask);
		dst = preparationContours(thresholdFrame,mask);

		dst.copyTo(frameInit);

		frame.copyTo(q);
		drawRoi(q);

		imshow("q",q);
		imshow("FrameThreshold",frameInit);

	    idFrame++;
	    keyboard=waitKey(70);

	    if(keyboard=='s')stop=true;
	    if(keyboard=='r')stop=false;
	}

	return 2;

}

int ofApp::kinectOne(){

	Mat rgbmat,depthmat,src,thresholdFrame;
	Mat ris(0,0,CV_8UC1);
	Mat equa(0,0,CV_8UC1);
	Mat dst,contoursFrame,mask,src2;

	double min1,max1,avg,min,max;
	Point pointMin,pointMax;
	char key=0;
	int flagMinMax;
	bool stop,flagMask;

	string serial;

	idFrame=0;

	libfreenect2::Freenect2 freenect2;
	libfreenect2::Freenect2Device *dev = NULL;
	libfreenect2::PacketPipeline *pipeline = NULL;

	if(freenect2.enumerateDevices() == 0)
	{
	   cout << "no device connected!" << endl;
	   return -1;
	}

	serial = freenect2.getDefaultDeviceSerialNumber();
	cout <<"SERIAL: "<<serial<<endl;

	if(pipeline) dev = freenect2.openDevice(serial, pipeline);
	else 		 dev = freenect2.openDevice(serial);


	if(dev == 0){
		cout << "failure opening device!"<<endl;
	    return -1;
	}

	signal(SIGINT, sigint_handler);
	protonect_shutdown = false;


	libfreenect2::SyncMultiFrameListener listener(libfreenect2::Frame::Color |
	                                                  libfreenect2::Frame::Depth |
	                                                  libfreenect2::Frame::Ir);
	libfreenect2::FrameMap frames;

	dev->setColorFrameListener(&listener);
	dev->setIrAndDepthFrameListener(&listener);
	dev->start();

	cout<<"device serial: "<< dev->getSerialNumber()<<endl;
	cout<<"device firmware: "<< dev->getFirmwareVersion()<<endl;

	libfreenect2::Registration* registration = new libfreenect2::Registration(dev->getIrCameraParams(), dev->getColorCameraParams());

	flagMask=false;
	flagMinMax=0;
	stop=false;

	mask=initMask(initMaskValue,equa);

	while( key != 'q' && key != 27 )
	{
		listener.waitForNewFrame(frames);
	    libfreenect2::Frame *rgb = frames[libfreenect2::Frame::Color];
	    libfreenect2::Frame *ir = frames[libfreenect2::Frame::Ir];
	    libfreenect2::Frame *depth = frames[libfreenect2::Frame::Depth];

	    Mat(rgb->height, rgb->width, CV_8UC4, rgb->data).copyTo(rgbmat);
	    Mat(depth->height, depth->width, CV_32FC1, depth->data).copyTo(depthmat);

	    minMaxLoc(depthmat,&min,&max);
		depthmat.convertTo(ris,CV_8UC1,255.0/(max-min),-min*255.0/(max-min));
		equalizeHist(ris,equa);

		equa.copyTo(src2);

	    drawRoi(src2);

		thresholdFrame=processFrame(equa,&flagMinMax,&min1,&max1,&avg,&pointMin,&pointMax,mask);

		dst = preparationContours(thresholdFrame,mask);
		dst.copyTo(frameInit);

		imshow("dst",dst);
		imshow("src2",src2);
		imshow("mask",mask);

		idFrame++;
		key = waitKey(30);

		listener.release(frames);
	}

	dev->stop();
	dev->close();

	delete registration;

	return 2;
}


vector <Point> ofApp::costruisciCentroide(Mat frame,Point pivot){

	vector<vector<Point>> contours;
	vector<Point> minPoint;
	vector<Point>hierarchy;
	vector<Point> kl;
	Mat dis =Mat::zeros( frame.size(), CV_8UC3 );
	double area;
	Mat contorni;

	frame.copyTo(contorni);

	findContours( contorni, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	/*for( int i = 0; i< contours.size(); i++ ) //Disegna i contorni sulll'imagine
	{
		Scalar color = Scalar(255 ,255 , 255);

		area=contourArea(contours[i]);
		if(area>50){
		drawContours( dis, contours, i, color, 1, 8, hierarchy, 0, Point() );
		}
	}*/

	minPoint=drawContursInit(contours,pivot);

	kl=drawContursFinal(frame,minPoint,contours,pivot);

	/*for( int i = 0; i< kl.size(); i++ ) //Disegna i contorni sulll'imagine
	{
		Scalar color = Scalar(255 ,255 , 255);
		circle(dis,kl[i],2,color, -1);

	}

	imshow("dis",dis);*/


	return kl;
}


vector<Point> ofApp::drawContursInit(vector<vector<Point>>contours,Point pivot ){

	double area;
	Mat contorni,src;
	Point centroidBlob;

	vector<Point> P,minPoint(contours.size());
	vector<Vec4i> hierarchy;

	Point h(pivot.x,pivot.y);
	Scalar color;
	int min=295;

	for( int i = 0; i< contours.size(); i++ )
	{
		P=contours[i];
		for(int j=0;j<P.size();j++){
			double res = cv::norm(P[j]-pivot);
			if(min>res){
				min=res;
				h.x=P[j].x;
				h.y=P[j].y;
			}
		}
		minPoint[i].x=h.x;
		minPoint[i].y=h.y;
	}

	return minPoint;
}

vector<Point> ofApp::drawContursFinal(Mat frame, vector<Point> minPoint,vector<vector<Point>>contours,Point pivot){

	Mat est=Mat::zeros(50,50,CV_8UC1);
	Point centroidBlob;
	double area;
	int x1,y1,w,a;
	int val=50,valMez=25;
	vector<Point> vectorPoint;

	for (int i=0;i<contours.size();i++){

		w=val;
		a=val;

		area=contourArea(contours[i]);

		if(area>50){

			est = tagliaMat(frame,&a,&w, &x1,&y1,minPoint[i],valMez, val,pivot);
			centroidBlob=contorni2(est);

			centroidBlob.x=centroidBlob.x+x1;
			centroidBlob.y=centroidBlob.y+y1;

			vectorPoint.push_back(centroidBlob);
		}
	}

	return vectorPoint;
}

Mat ofApp::tagliaMat(Mat frame,int *a, int *w, int *x1,int *y1,Point minPoint,int valMez,int val,Point pivot){

	Mat est;

	if(minPoint.x <= pivot.x && minPoint.y>= pivot.y){
		*x1=minPoint.x-valMez;
		*y1=minPoint.y;

		if((*y1)+(*a)>frame.rows){
			*a=frame.rows-(*y1);
		}

		if((*x1)<0){
			(*x1)=0;
		}
	}

	if(minPoint.x > pivot.x && minPoint.y> pivot.y){
		*x1=minPoint.x-valMez;
		*y1=minPoint.y;

		if((*x1)+(*w)>frame.cols){
			*w=frame.cols-(*x1);
		}
		if((*y1)+(*a)>frame.rows){
			*a=frame.rows-(*y1);
		}
	}

	if(minPoint.x > pivot.x && minPoint.y< pivot.y){
		*x1=minPoint.x;
		*y1=minPoint.y-valMez;

		if((*x1)+(*w)>frame.cols){
			*w=frame.cols-(*x1);
		}

		if((*y1)<0){
			*y1=0;
		}
	}

	if(minPoint.x < pivot.x && minPoint.y< pivot.y){
		(*x1)=minPoint.x-valMez;
		(*y1)=minPoint.y-val;

		if((*x1)<0){
			(*x1)=0;
		}

		if((*y1) < 0){
			(*y1)=0;
		}
	}

	return est=frame(Rect((*x1),(*y1),(*w),(*a)));
}


Point ofApp::contorni2(Mat frame){

	double area;
	Mat contorni,src;
	Mat drawing = Mat::zeros(frame.size(), CV_8UC3 );

	Point poi;
	vector<vector<Point> > contours;
	vector<Point> P;
	vector<Vec4i> hierarchy;

	vector<Point> tmp;

	frame.copyTo(contorni);
	frame.copyTo(src);

	findContours( contorni, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);


	vector<Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ ){
		mu[i] = moments( contours[i], false );
	}


	vector<Point2f> mc( contours.size() );
	for( int i = 0; i < contours.size(); i++ ){
		mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
	}

	int q=0;
	for( int i = 0; i< contours.size(); i++ )
	{
		Scalar color = Scalar(255 ,255 , 255);

		area=contourArea(contours[i]);

		if(area>50){
			drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, Point() );

			poi.x=mc[i].x;
			poi.y=mc[i].y;
			circle( drawing, mc[i], 2, color, -1, 8, 0 );
		}
	}
	return poi ;
}


void* ofApp::handleThreadUpdate(void *params) {

	Controller * c=(Controller*)params;

	int i,idFrameThread=-1;

	Point pivot;
	bool roi;
	vector<Point> kl;

	string clientString=std::to_string(c->getId());
	int timerInattivo=RESET;

	int threadTimeFlag=RESET;
	pivot=ofAppInstance->initPivot(c->getId());


	while(isFinished==0)
	{
		if(frameInit.data)
		{
			inputEvent=NO_PEOPLE;
			kl=ofAppInstance->costruisciCentroide(frameInit,pivot);

			if(kl.size()<=0)
			{
				inputEvent=NO_PEOPLE;
				if(!(timerInattivo))
				{
					seconds = ofAppInstance->startTimer();
					timerInattivo = SET;
				}

				if (ofAppInstance->isTimerFinished(seconds,TIME_REFRESH))
				{
					timerInattivo=RESET;
					c->changeState(NOT_PEOPLE_IN,NO_CONDITION);
				}
				ofAppInstance->analyzeTransitionUsers_thread(c,clientString,Point(0,0),&threadTimeFlag);
			}
			else
			{
				inputEvent=DETECT_PEOPLE;
				timerInattivo=RESET;

				if(idFrameThread!= idFrame)
				{
					idFrameThread=idFrame;
					roi=false;

					i=0;
					while(i<kl.size())
					{
						if((ofAppInstance->isRoi(c->getId(),kl[i].x,kl[i].y)).compare(NO_ROI)){
							ofAppInstance->analyzeTransitionUsers_thread(c,clientString,kl[i],&threadTimeFlag);
							roi=true;
							break;
						}
						i++;
					}
					if(!roi){
						ofAppInstance->analyzeTransitionUsers_thread(c,clientString,kl[0],&threadTimeFlag);
					}
				}
			}
		}
	}
}


void ofApp::analyzeTransitionUsers_thread(Controller *c,string clientString, CvPoint pos,int *tTime) {

	string roiSelected="";
	int posClass=-1;

	if(! c->getCurrentState()->getType().compare(ST_IDLE))
	{
		c->changeState(NO_EVENT,clientString);
	}
	else if(! c->getCurrentState()->getType().compare(ST_INTRO))
	{
		if(inputEvent==DETECT_PEOPLE) c->changeState(PEOPLE_IN,clientString);
	}
	else if(!c->getCurrentState()->getType().compare(ST_MENU) )
	{
		roiSelected=isRoi(c->getId(),pos.x, pos.y);

		if(roiSelected.compare(NO_ROI))
		{
			*tTime = RESET;
			c->changeState(TRANSIT_ON_ROI,roiSelected);
		}
		else{
			*tTime = RESET;
		}
	}
	else if(!c->getCurrentState()->getType().compare(ST_PREVIEW ) )
	{
		if(!(*tTime)){
			posClassHistory->initHistoryBuffers();
			*tTime = SET;
		}

		roiSelected=isRoi(c->getId(),pos.x, pos.y);
		if(roiSelected.compare(NO_ROI))
		{
			posClassHistory->updateHistoryBuffers(atoi(roiSelected.c_str()));

			posClass = posClassHistory->getEyeCenterWeightedAvgValue(0.8f);

			if(posClass==atoi(roiSelected.c_str())){
				posClassHistory->initHistoryBuffers();
				c->changeState(ACTIVATED_ROI,NO_CONDITION);
				*tTime = RESET;
			}
		}
		else
		{
			*tTime = RESET;
			c->changeState(EXIT_ROI,NO_CONDITION);
			//posClassHistory->initHistoryBuffers();
		}

	}
}

time_t ofApp::startTimer(void){
	//inizializzo tempo a ora attuale
	return time (NULL);
}

int  ofApp::isTimerFinished(time_t firstValue, int maxValue){
	time_t theseSeconds;

	/*
	 * calcolo ora attuale
	 * faccio differenza dei secondi
	 * e memorizzo in losting timer.
	 */
	theseSeconds = time(NULL);
	lostingTimer = theseSeconds - firstValue;
	//printf("\nTIMER: %d. MAX =%d\n", lostingTimer, maxValue);
	//Timer finished
	if(lostingTimer >= maxValue)
		return 1;

	return 0;
}


//--------------------------------------------------------------
bool ofApp::checkClients() {


	if( !clientsAreOk && myFloor->getNumberOfClients()==requestedClients ) {
    	setupClients();
        clientsAreOk = true;
        
    }
    else if( clientsAreOk && myFloor->getNumberOfClients()!=requestedClients ) {
        clientsAreOk = false;
    }

    return clientsAreOk;
}

//--------------------------------------------------------------
void ofApp::onContentEvent(string event) {
    
}

//--------------------------------------------------------------
void ofApp::setupClients() {

	int clients;
	bool flag;

    list<pair<TopLayout*, int> >::iterator it;
    
    clients = myFloor->getNumberOfClients();

    it = allWindows.begin();
    
    for( int i=0; i<clients; i++ ) {
        (*it).second = i;
        
        myFloor->sendLayout(i, (*it).first->toString().c_str() );

        it++;
    }
    
    // Notify clients?!
}
//-----------------------------------------------------------------------
void ofApp::drawRoi(Mat img){


	list<pair<TopLayout*, int> >::iterator it;
	list<Roi*> cnt;
	list<Roi*>::iterator it2;
	int i;

	int left, top,width,height;

	for(it=allWindows.begin();it!= allWindows.end();it++){
		cnt=(*it).first->getChildrenRoi();
		i=0;
		for(it2=cnt.begin();it2!=cnt.end();it2++){
			left=(*it2)->getLeftInt();
			top=(*it2)->getTopInt();
			width=(*it2)->getWidthInt();
			height=(*it2)->getHeightInt();

			rectangle(img,Point(left,top),Point((left+width),(top+height)) ,Scalar(255,255,255),2);
		}
	}
}

