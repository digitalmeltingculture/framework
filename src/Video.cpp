//
//  Video.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "Video.h"

Video::Video() {
    client = Floor::getInstance("", 0, 0);
}

Video::~Video() {
}

void Video::execPlay() {
    int clientSock, pos;
    
    pos = ofApp::getClientFromLayout(window);
    if( pos==-1 ) {
        cerr << "client non trovato o layout non valido" << endl;
        return;
    }
    clientSock = client->getClientConnection(pos);
    
    std::cout << "play video on client "<<clientSock<<". Source: " << this->getSrc() << std::endl;
}
