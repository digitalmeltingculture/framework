/*
 * Configuration.h
 *
 *  Created on: 06/giu/2014
 *      Author: dariux881
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>

using namespace std;

class Configuration {
public:
	static Configuration* getInstance();
	virtual ~Configuration();
    
    int minAngularVariation 	= 70; 	// In degree
    int maxAngularVariation 	= 170; 	// In degree
    int minAngularVariation_2   = 30;   // In degree
    int maxAngularVariation_2   = 60;   // In degree
    int minTimeElapsed 			= 600; 	// In ms
    int maxTimeElapsed 			= 2000;	// In ms
    
    int R = 100;
    int rainDrops = 100;
    int r = 20;
    int T = 110;
    
    int depthThreshold = 100;
    int kinectHeight = 350;
    
    int x_offset = 0;
    int y_offset = 0;
    int width_offset = 640;
    int height_offset = 480;
    
    string interactionFile;
    string clientBasePath;
    string serverIp;
    int serverPort, serverPort2;
    int clientDifferentMachines;

  private:
	Configuration();

	static Configuration* instance;
};

#endif /* CONFIGURATION_H_ */
