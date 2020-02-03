//
//  Image.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "Image.h"

Image::Image() {
    client = Floor::getInstance("", 0, 0);
}

Image::~Image() {
}

void Image::execPlay() {
    int clientSock, pos;
    
    pos = ofApp::getClientFromLayout(window);
    if( pos==-1 ) {
        cerr << "client non trovato o layout non valido" << endl;
        return;
    }
    clientSock = client->getClientConnection(pos);
    
    std::cout << "play image on client n."<<pos<<". Source: " << this->getSrc() << std::endl;
}
