//
//  Audio.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "Audio.h"
#include <cstdlib>
#include <stdio.h>

Audio::Audio() {
    client = Floor::getInstance("", 0, 0);
}

Audio::~Audio() {
    
}

void Audio::execPlay() {
    int clientSock, pos;
    
    pos = ofApp::getClientFromLayout(window);
    if( pos==-1 ) {
        cerr << "client non trovato o layout non valido" << endl;
        return;
    }
    clientSock = client->getClientConnection(pos);
    
    std::cout << "play audio on client "<<clientSock<<". Source: " << this->getSrc() << std::endl;
}


