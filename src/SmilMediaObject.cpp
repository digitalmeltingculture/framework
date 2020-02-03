//
//  MediaObject.cpp
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#include "SmilMediaObject.h"

void SmilMediaObject::setFps(int fps){
	this->fps=fps;
}
int SmilMediaObject::getFps(){
	return this->fps;
}


void SmilMediaObject::setLoop(string loop){
	this->loop=loop;
}
string SmilMediaObject::getLoop(){
	return this->loop;
}

void SmilMediaObject::setPlayer(string player) {
    this->player = player;
}
string SmilMediaObject::getPlayer() {
    return this->player;
}


void SmilMediaObject::setId(string id) {
    this->id = id;
}

void SmilMediaObject::setTitle(string title) {
    this->title = title;
}

void SmilMediaObject::setBegin(long begin) {
    this->begin = begin;
}

void SmilMediaObject::setDur(long dur) {
    this->dur = dur;
}

void SmilMediaObject::setEnd(long end) {
    this->end = end;
}

void SmilMediaObject::setClipBegin(string begin) {
    this->clipBegin = begin;
}

void SmilMediaObject::setClipEnd(string end) {
    this->clipEnd = end;
}

void SmilMediaObject::setSrc(string src) {
    this->src = src;
}

void SmilMediaObject::setType(string type) {
    this->type = type;
}

void SmilMediaObject::setAlt(string alt) {
    this->alt = alt;
}

void SmilMediaObject::setRegion(Region* region) {
    this->region = region;
}

void SmilMediaObject::setWindowName(string name) {
    this->windowName = name;
}

void SmilMediaObject::setWindow(TopLayout* window) {
    this->window = window;
}

string SmilMediaObject::getId() {
    return id;
}

string SmilMediaObject::getTitle() {
    return title;
}

long SmilMediaObject::getBegin() {
    return begin;
}

long SmilMediaObject::getDur() {
    return dur;
}

long SmilMediaObject::getEnd() {
    return end;
}

string SmilMediaObject::getClipBegin() {
    return clipBegin;
}

string SmilMediaObject::getClipEnd() {
    return clipEnd;
}

string SmilMediaObject::getSrc() {
    return src;
}

string SmilMediaObject::getAlt() {
    return alt;
}

string SmilMediaObject::getType() {
    return type;
}

Region* SmilMediaObject::getRegion() {
    return region;
}

void SmilMediaObject::setBackgroundClient(string name) {
    this->backgroundClient = name;
}

string SmilMediaObject::getBackgroundClient() {
    return backgroundClient;
}



