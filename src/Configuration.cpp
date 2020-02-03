/*
 * Configuration.cpp
 *
 *  Created on: 06/giu/2014
 *      Author: dariux881
 */

#include "Configuration.h"

Configuration* Configuration::instance = NULL;

Configuration* Configuration::getInstance() {
	if( !instance )
		instance = new Configuration();

	return instance;
}

Configuration::Configuration() {
    ifstream configFile;
    string name;
    string value;
    

    serverIp    = "127.0.0.1";
    serverPort  = 9645;
    serverPort2 = 9646;
    interactionFile = "./res/provaD.scxml";
    clientBasePath = "/home/franky/Scrivania/eclipse/workspace/CFinal/bin/client";
    clientDifferentMachines=0;

    configFile.open( string(string(getcwd(NULL, 0)) + "/data/table.conf").c_str() );
    if( !configFile.is_open() )
        cout <<"(SERVER) FILE CONFIGURATION NOT FOUND" << endl;
    else
    {
    	do{
    		configFile>>name>>value;
    		istringstream ss(value);
        
    		if( !name.compare("serverIp")) {
    			ss >> serverIp;
    		}
    		else if( !name.compare("serverPort")) {
    			ss >> serverPort;
    		}
    		else if( !name.compare("serverPort2")) {
    			ss >> serverPort2;
    		}
    		else if( !name.compare("clientDifferentMachines") ) {
    		    ss >> clientDifferentMachines;
    		}
    		else if( !name.compare("interactionFile") ) {
    			ss >> interactionFile;
    		}
    		else if( !name.compare("clientBasePath") ) {
    			ss >> clientBasePath;
    		}
    	}while( configFile.good() );
    }
}


Configuration::~Configuration() {
}

