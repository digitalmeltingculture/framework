/*
 * UserOnFloor.cpp
 *
 *  Created on: 21/mag/2014
 *      Author: dariux881
 */

#include "UserOnFloor.h"

UserOnFloor::UserOnFloor() {
	id = 0;
	leader = false;
	xPos = yPos = zPos = 0.0f;
}

UserOnFloor::UserOnFloor(short id, bool leader, float x, float y, float z) {
	this->id = id;
	this->leader = leader;
	xPos = x;
	yPos = y;
	zPos = z;
}

void UserOnFloor::setId(short id) {
	this->id = id;
}

void UserOnFloor::setLeader(bool value) {
	this->leader = value;
}

void UserOnFloor::setPosition(float x, float y, float z) {
	xPos = x;
	yPos = y;
	zPos = z;
}

short UserOnFloor::getId() {
	return id;
}

bool  UserOnFloor::isLeader() {
	return leader;
}

float UserOnFloor::getX() {
	return xPos;
}

float UserOnFloor::getY() {
	return yPos;
}

float UserOnFloor::getZ() {
	return zPos;
}

UserOnFloor::~UserOnFloor() {
}

