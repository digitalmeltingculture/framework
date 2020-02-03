/*
 * Floor.h
 *
 *  Created on: 21/mag/2014
 *      Author: dariux881
 */

#ifndef FLOOR_H_
#define FLOOR_H_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

#include "UserOnFloor.h"
#include <vector>

using namespace std;

void    *acceptClients( void* );
void    *Client (void * );
class Floor {
public:
    static Floor* instance;
    static Floor* getInstance(string ip_address, int port, int port2);
    int    getSock() {
        return sockfd;
    }
    int    getUsersSock() {
        return usersSockFd;
    }
    int    getClientConnection(int);
    int    getMaxNumberOfClients();
    void   setMaxNumberOfClients(int);
    void   addPeers( int );
    vector<int>   getClients() {
        return peers;
    }

	virtual ~Floor();

	bool sendContentParallelState(int window);
	void updatePeople( vector<UserOnFloor> );
    bool sendLayout(int sockfd, const char* layout );
    bool sendContent(int sockfd, const char* layout );
    bool sendStopRequest(int sockfd );
    bool sendParentela(int window,const char * value ) ;
    bool receivePlayResult(int sockfd );
    
    int  getNumberOfClients() {
        return peers.size();
    }
    bool isValidClientForUsers() {
        return clientForFloor!=-1;
    }

    void RequestClients(string ip_address, int portno);

    int    				sockfd;

private:




    Floor(string ip_address, int port, int usersPortNo);
	bool	openCommunicationForContent();
	bool	openCommunicationForUsers();

	bool	openCommunication(string, int, int*);
    static void* acceptUsersClient(void*);

	bool    writeShort(int sockfd, short msg);
	bool    writeFloat(int sockfd, float msg);
	bool    writeByte(int sockfd, char msg);
	bool    writeCharArray(int sockfd, const char* msg);
    short   readShort(int sockfd);
    bool    readBool(int sockfd);

	char*   numberToCharArray(int value);
	char*   numberToCharArray(float value);

    int    				/*sockfd,*/ portno, usersSockFd, usersPortNo;
    int                 clientForFloor;
    string 				ip_address;
    int         maxNumberOfPeers;
    vector<int> peers;
};

#endif /* FLOOR_H_ */
