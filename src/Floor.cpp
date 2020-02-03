/*
cz * Floor.cpp
 *
 *  Created on: 21/mag/2014
 *      Author: dariux881
 */

#include "Floor.h"
#include <errno.h>

Floor* Floor::instance = NULL;

Floor* Floor::getInstance(string ip_address, int port, int usersPortNo) {
    if( !instance )
        instance = new Floor(ip_address, port, usersPortNo);
    
    if( !instance->ip_address.compare("") ) {
        instance->ip_address = ip_address;
        instance->openCommunicationForContent();
    }
    if( instance->portno==0 ) {
        instance->portno = port;
        instance->openCommunicationForContent();
    }
    if( instance->usersPortNo==0 ) {
        instance->usersPortNo = usersPortNo;
        instance->openCommunicationForUsers();
    }
    
    return instance;
}

Floor::Floor(string ip_address, int port, int usersPortNo) {
    this->ip_address = ip_address;
    portno = port;
    this->usersPortNo = usersPortNo;
    sockfd = 0;
    clientForFloor = -1;
    
    maxNumberOfPeers = 10;

    openCommunicationForContent();
}

void Floor::setMaxNumberOfClients(int val) {
    maxNumberOfPeers = val;
}

int  Floor::getMaxNumberOfClients() {
    return maxNumberOfPeers;
}

bool	Floor::openCommunicationForContent() {

	if( !ip_address.compare("") || portno==0 // Ip or port are not valid
       || sockfd!=0 )                       // The communication is already open
        return false;
    
    if( openCommunication(ip_address, portno, &sockfd) ) {
        pthread_t tid;
        pthread_create( &tid, NULL, acceptClients, this );
    }
    
    return true;
}

bool	Floor::openCommunicationForUsers() {
    if( !ip_address.compare("") || usersPortNo==0 // Ip or port are not valid
       || usersSockFd!=0 )                        // The communication is already open
        return false;

    if( openCommunication(ip_address, usersPortNo, &usersSockFd) ) {
        pthread_t tid;
        pthread_create( &tid, NULL, acceptUsersClient, this );
    }
    
    return true;
}

bool Floor::openCommunication(string ip_address, int portno, int* sockfd) {
    struct sockaddr_in 	serv_addr;
    struct hostent 		*server;
    int    backlog = 5;
    
    cout << "Acception on " << ip_address << "::" << portno << endl;

    *sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if( *sockfd<0 ) {
    	cout<<"ERROR opening socket"<<endl;
    	return false;
    }

    server = gethostbyname(ip_address.c_str());

    if( server==NULL ) {
    	cout<<"ERROR no such host"<<endl;
    	return false;
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    memcpy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);


    /* SERVER PART */
    if (bind(*sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr_in)) == -1) {
    	//cout<<"ERROR bind"<<endl;
    	cout<<"ERRORE bind >--<"<<strerror(errno)<<endl;
    	return false;
    }

    if (listen(*sockfd, backlog) == -1) {
        cout<<"ERROR listen"<<endl;
    	return false;
    }

    return true;
}

void* acceptClients(void* params) {

	int flag=0;
	struct sockaddr_in tmpPeer;
    socklen_t peerSize;
    Floor*    floor = (Floor*)params;
    
    while( floor->getNumberOfClients() < floor->getMaxNumberOfClients() ) {
        int    cFd;
        
        peerSize = sizeof(struct sockaddr_in);
        
        cFd = accept(floor->getSock(), (struct sockaddr *) &tmpPeer, &peerSize);
        
        if( cFd==-1 ) {
            cout << "ERROR accept" << endl;
            flag=1;
           	break;
        }
        
        floor->addPeers(cFd);
    }
    
    if( flag==0) cout << "raggiunto limite massimo di utenti" << endl;
    //return;
}

void* Floor::acceptUsersClient(void* params) {

	struct sockaddr_in tmpPeer;
    socklen_t peerSize;
    Floor*    floor = (Floor*)params;
    
    cout << "accepting connections" << endl;
    
    peerSize = sizeof(struct sockaddr_in);
    
    floor->clientForFloor = accept(floor->getUsersSock(), (struct sockaddr *) &tmpPeer, &peerSize);
    
    if( floor->clientForFloor==-1 ) {
        cout << "ERROR accept" << endl;
    }
}

int   Floor::getClientConnection(int pos) {
    if( pos>peers.size() )
        return -1;
    
    return peers[pos];
}

void  Floor::addPeers( int newPeer ) {
    peers.push_back(newPeer);
}

bool Floor::writeCharArray(int sockfd, const char* msg) {
	int n;
    short size;

    if( !msg || !(size=strlen(msg)) ) {
        return true;
    }
    
    n = write(sockfd, &size, sizeof(short));
	n = write(sockfd, msg, strlen(msg));

	if( n<0 ) {
		//cout<<"ERROR writing char array on socket"<<endl;
		return false;
	}

	return true;
}

bool Floor::writeShort(int sockfd, short msg) {
	int n;

	n = write(sockfd, &msg, sizeof(short));

	if( n<0 ) {
		//cout<<"ERROR writing short on socket"<<endl;
		return false;
	}

	return true;
}

bool    Floor::writeFloat(int sockfd, float msg) {
	int n;

	n = write(sockfd, &msg, sizeof(float));

	if( n<0 ) {
		//cout<<"ERROR writing float on socket"<<endl;
		return false;
	}

	return true;
}

bool    Floor::writeByte(int sockfd, char msg) {
	int n;
	
    n = write(sockfd, &msg, sizeof(char));

	if( n<0 ) {
		//cout<<"ERROR writing byte on socket"<<endl;
		return false;
	}

	return true;

}

short   Floor::readShort(int sockfd) {
    short val;
    
    read(sockfd, &val, sizeof(short));
   // cout<<"(Server)readShort val >>--> "<<val<<endl;
    return val;
}

bool    Floor::readBool(int sockfd) {
    char byte;
    
    int f=read(sockfd, &byte, 1);

    if( byte==0x00 )
        return false;
    else
        return true;
}

void Floor::updatePeople(vector<UserOnFloor> users) {
	int i, n;

	n = users.size();

    if( clientForFloor<0 ) {
        cout << "clientForFloor not valid" << endl;
        return; //false;
    }

    // Intero per identificare un tipo di comunicazione per utente
    writeShort( clientForFloor, (short)1 );
    
	// Scrivo il numero delle persone
	writeShort( clientForFloor, (short)n );

	for( i=0; i<n; i++ ) {
		// Scrivo leader/no leader
		if( users[i].isLeader() )
			writeByte(clientForFloor, 0x01);
		else
			writeByte(clientForFloor, 0x00);

		// Scrivo le 3 posizioni
		writeFloat( clientForFloor, users[i].getX() );
		writeFloat( clientForFloor, 1.0f - users[i].getY() );
	}
}

bool Floor::sendLayout(int window, const char *layout) {

	if( window>peers.size() )
        return false;

    writeShort(peers[window], 0);
    writeCharArray(peers[window], "window_layout");
    writeCharArray(peers[window], layout);


    short f= (readShort(peers[window]) == 1);
    return f;

    //return readBool(peers[window]);
}

bool Floor::sendParentela(int window,const char * value ) {
	  if( window>peers.size() )
	        return false;

	  	writeShort(peers[window], 0);
	    writeCharArray(peers[window], "flag_parentela");
	    writeCharArray(peers[window], value);

	    return (readShort(peers[window]) == 1);
}

bool Floor::sendContentParallelState(int window ) {
	  if( window>peers.size() )
	        return false;

	  	writeShort(peers[window], 0);
	    writeCharArray(peers[window], "window_parallel");

	   // return readBool(peers[window]);
	    return (readShort(peers[window]) == 1);
}

bool Floor::sendContent(int window, const char* content ) {
    if( window>peers.size() )
        return false;
    
    writeShort(peers[window], 0);
    writeCharArray(peers[window], "window_content");
    writeCharArray(peers[window], content);
    
   // return readBool(peers[window]);
    return (readShort(peers[window]) == 1);
}

bool Floor::sendStopRequest( int window ) {
	if( window>peers.size() )
        return false;

    writeShort(peers[window], 0);
    writeCharArray(peers[window], "content_stop");

    //return readBool(peers[window]);
    return (readShort(peers[window]) == 1);
}

bool Floor::receivePlayResult(int window ) {
    if( window>peers.size() )
        return false;
    //return readBool(peers[window]);
    return readShort(peers[window]);
}

Floor::~Floor() {

}

