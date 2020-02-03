//
//  Content.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "Container.h"
#include <cstdlib>
#include <pthread.h>
#include <map>
#include <string>

Container* Container::containerInstance;

//--------------------------------------------------------------------
Container::Container(string type) {
    if(!type.compare("par") && !type.compare("seq") && !type.compare("excl") ) {
    	cout << "errore nel tipo richiesto ["<<type<<"]" << endl;
        return;
    }
    this->type = type;
    
    contents.clear();

    region = NULL;
    begin = dur = end = 0;
    repeat = 0;
    hard_sync = false;
}

//--------------------------------------------------------------------
void Container::execContent() {

	//cout<<"(Server) Sono in ExecContent"<<endl;

	int tmpRepeat = repeat;
	list<pair<void*, string> >::iterator compIt;
    
	if( !type.compare("seq") ) {
		do {
	        list<pair<void*, string> >::iterator compIt;

	        for( compIt = contents.begin(); compIt!=contents.end(); compIt++) {
                if( !((*compIt).second).compare("MediaObject") ) {
                    playInSeq("MediaObject", (SmilMediaObject*)(*compIt).first);
                }
            }
            
			tmpRepeat--;
            
            ofApp::ofAppInstance->onContentEvent("contentFinished");
            
		} while( tmpRepeat>0 );
	}
	else if( !type.compare("par") ) {
		int fgtr=0;
		do {
			playInPar("Container", this);
			ofApp::ofAppInstance->onContentEvent("contentFinished");
			tmpRepeat--;


		} while( tmpRepeat>0 );
	}
	else { // excl
		do {

			tmpRepeat--;
		} while( tmpRepeat>0 );
	}
}

//--------------------------------------------------------------------
void Container::execStop() {
}

//--------------------------------------------------------------------
void Container::playInPar(string type, void* object) {
	pthread_t tid;

	//cout<<"(Server)Sono in playInPar"<<endl;

    if( !type.compare("Container") ) {
//        if( pthread_create(&tid, NULL, playContainerInPar, object) ) {
//            cerr << "errore creazione thread container\n";
//            exit(-1);
//        }
//

        playContainerInPar(object);
    }
    else {
    	if( pthread_create(&tid, NULL, playContentInPar, object) ) {
            cerr << "errore creazione thread container\n";
            exit(-1);
        }
        pthread_join( tid, NULL );
    }
}

//--------------------------------------------------------------------
void Container::playInSeq(string type, void* object) {
	pthread_t tid;
    
	//cout<<"(Server) Sono in playInSeq"<<endl;
    if( !type.compare("Container") ) {
        //if( pthread_create(&tid, NULL, playContainerInPar, object) ) {
            //cerr << "errore creazione thread container\n";
        //}
        //cerr << "c'è un container in seq\n";
    }
    else {
        playContentInSeq(object);
        
//        if( pthread_create(&tid, NULL, playContentInSeq, object) ) {
//            cerr << "errore creazione thread container\n";
//        }
    }
    
//    pthread_join( tid, NULL );
}

//--------------------------------------------------------------------
void* Container::playContentInSeq(void* params) {
    //cout<<"(Server) Sono in PlaycontentInSeq"<<endl;

    string singleContent;
    TopLayout* tmpLayout;
    int    clientIdx;
    sender_params sp;
    
    singleContent = ((SmilMediaObject*)params)->toString();
    tmpLayout = ((SmilMediaObject*)params)->getWindow();
    
    clientIdx = ofApp::getClientFromLayout(tmpLayout);
    
    sp.myFloor = Floor::getInstance("", 0, 0);
    sp.window  = clientIdx;
    sp.content = containerInstance->buildXMLRootElement("seq",singleContent,"seq");
    //sp.content = singleContent;
    
    // chiedo la terminazione di un eventuale contenuto attivo
    sp.myFloor->sendStopRequest(sp.window);

    sendContent_helper(&sp);
}

//--------------------------------------------------------------------
void* Container::playContentInPar(void* params) {

	//cout<<"(Server) playContentInPar "<<endl;
	string singleContent;
    TopLayout* tmpLayout;
    int    clientIdx;
    sender_params sp;
    pthread_t tid;
    
    singleContent = ((SmilMediaObject*)params)->toString();
    tmpLayout = ((SmilMediaObject*)params)->getWindow();
    
    clientIdx = ofApp::getClientFromLayout(tmpLayout);
    

    sp.myFloor = Floor::getInstance("", 0, 0);
    sp.window  = clientIdx;
    sp.content = buildXMLRootElement("par",singleContent,"par");
    // sp.content = singleContent;
    
    sendContent_helper(&sp);
}

//--------------------------------------------------------------------
void* Container::playContainerInPar(void* params) {

	//cout<<"(Server) sono in playContainerInPar"<<endl;
	map<int, string> allConts;
    map<int, string>::iterator conIt;
	list<pair<void*, string> >::iterator compIt;
    Floor* myFloor = Floor::getInstance("", 0, 0);
    string singleContent;
    TopLayout* tmpLayout;
    stringstream *tmpSS;
    Container*   mySelf;

    int clientIdx;
    int d;
    
    mySelf = (Container*)params;
    
    for( compIt = mySelf->contents.begin(); compIt!=mySelf->contents.end(); compIt++) {


    	if( !((*compIt).second).compare("MediaObject") ) {
            singleContent = ((SmilMediaObject*)(*compIt).first)->toString();
            tmpLayout = ((SmilMediaObject*)(*compIt).first)->getWindow();
            
            clientIdx = ofApp::getClientFromLayout(tmpLayout);

            tmpSS = new stringstream(singleContent);
            
            if( (conIt=allConts.find(clientIdx))!=allConts.end() ) {
                *tmpSS << conIt->second;
            }
            
            *tmpSS << singleContent;


            allConts[clientIdx] = tmpSS->str();
        }
    }

    vector<pthread_t> threads(allConts.size());
    int i=0;
    for( conIt=allConts.begin(); conIt!=allConts.end(); conIt++) {
        sender_params *sp = new sender_params();

        sp->myFloor = myFloor;
        sp->window  = conIt->first;
        sp->content = buildXMLRootElement("par",conIt->second,"par");

        if( pthread_create(&threads[i], NULL, sendContent_helper, sp) ) {
        		cerr << "play container in par. error create thread" << endl;
        }
        i++;
    }
    
    for( i=0; i<threads.size(); i++ ) {
        pthread_join( threads[i], NULL);
    }
}

//--------------------------------------------------------------------
void* Container::sendContent_helper(void *sp) {

	//cout<<"\n\n\nFinestra= "<<((sender_params*)sp)->window <<endl;
	//cout<<"Contenuto inviato= "<<((sender_params*)sp)->content.c_str()<<endl;
	((sender_params*)sp)->myFloor->sendContent(((sender_params*)sp)->window, ((sender_params*)sp)->content.c_str());
	short result = ((sender_params*)sp)->myFloor->receivePlayResult(((sender_params*)sp)->window);

	//cout<<"(Server)(sendContent_helper) Risultato PlayContent >>-->"<<result<<endl;

}


string Container::buildXMLRootElement(string rootName, string body,string term){
	string nodeEntry	= "<"+rootName+">";
	string nodeExit		= "</"+term+">";

	return nodeEntry+body+nodeExit;
}

