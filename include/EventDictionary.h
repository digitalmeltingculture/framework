/*
 * EventDictionary.h
 *
 *  Created on: Nov 17, 2014
 *      Author: prisca-davide
 */

#ifndef EVENTDICTIONARY_H_
#define EVENTDICTIONARY_H_

class EventDictionary {

public:
	EventDictionary();
	virtual ~EventDictionary();

	//smil events. Shared among all applications
	static const string smilStart = "smil.start";
	static const string smilEnd = "smil.end";

	//eyecu events.
	static const string faceDetect = "face.detect";
	static const string watchStart = "watch.start";
	static const string watchMove = "watch.move";
	static const string watchEnd = "watch.end";
	static const string observeStart = "observe.start";

	//past events.
	static const string userMove = "userMove";
	static const string gestureL2R = "gestureLefttoright";
	static const string gestureR2L = "gestureRighttoleft";
	static const string gestureZoomIn = "gestureZoomin";
	static const string gestureZoomOut = "gestureZoomout";

};

#endif /* EVENTDICTIONARY_H_ */
