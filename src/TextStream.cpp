//
//  TextStream.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 05/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "TextStream.h"

TextStream::TextStream() {
    client = Floor::getInstance("", 0, 0);
}

TextStream::~TextStream() {
}

void TextStream::execPlay() {
    int clientSock, pos;
    
    pos = ofApp::getClientFromLayout(window);
    if( pos==-1 ) {
        cerr << "client non trovato o layout non valido" << endl;
        return;
    }
    clientSock = client->getClientConnection(pos);
    
    std::cout << "play textstream on client "<<clientSock<<". Source: " << this->getSrc() << std::endl;
}
