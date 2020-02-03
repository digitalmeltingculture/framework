#include "ofApp.h"

#include <libxml/xmlreader.h>
#include "TmpTransition.h"
#include "Transition.h"
#include "Content.h"
#include "State.h"
#include "Root.h"
#include "SCXMLParser.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

bool verificaInput(int argc, char** argv );

using namespace cv;
//========================================================================
int main(int argc, char** argv ){

	string s1,s2;

	if(!(verificaInput(argc,argv ))) return -1;

	Configuration* conf = Configuration::getInstance();
    Root root;
    SCXMLParser parser(conf->interactionFile);

    ofApp* realApp;
    
	root = parser.parse();
    
	if(argc==2){
		s1=argv[1];
		realApp = new ofApp(s1);
	}
	else if(argc==3){

		s1=argv[1];
		s2=argv[2];

		realApp = new ofApp(s1,s2);
	}

    realApp->setRoot(root);
    realApp->setup();

    realApp->update_new();

}


bool verificaInput(int argc, char** argv ){

	string s1,s2;

	if(argc==1){
		cout<<" ERRORE INSERIMENTO INPUT "<<endl;
		return false;
	}
	else if(argc==2){
		s1=argv[1];

		if((!s1.compare(KINECT_360))||(!s1.compare(KINECT_ONE))){
			return true;
		}
		else{
			cout<<" ERRORE INSERIMENTO INPUT "<<endl;
			return false;
		}
	}
	else if(argc==3){
		s1=argv[1];
		s2=argv[2];

		if(!s1.compare(VIDEO)){
			VideoCapture inputVideo(s2.c_str());
			if(!inputVideo.isOpened()){
				cout << "Errore Video Non trovato... " << endl;
					return false;
			}
			return true;
		}
		else
		{
			cout<<" ERRORE INSERIMENTO INPUT "<<endl;
			return false;
		}
	}
	else
	{
		cout<<" ERRORE INSERIMENTO INPUT "<<endl;
		return false;
	}

}

