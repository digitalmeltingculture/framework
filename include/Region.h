//
//  Region.h
//  smil_parser
//
//  Created by Dario Di Mauro on 02/12/14.
//  Copyright (c) 2014 Dario Di Mauro. All rights reserved.
//

#ifndef __smil_parser__Region__
#define __smil_parser__Region__

#include <iostream>
#include <string>
#include <list>
#include "TopLayout.h"
#include <sstream>

using namespace std;

class TopLayout;

class Region {
public:
	Region() {
        myTopLayout = NULL;
        parentRegion = NULL;
        


        fit = "hidden";
        width = height = "auto";
        
        top="0";
        bottom="0";
        left="0";
        right="0";


        widthInt =0;
        heightInt=0;
        topInt=0;
        bottomInt=0;
        leftInt=0;
        rightInt=0;



        topServer=0;
        leftServer=0;

    }

	//------Campi Convertiti -------


	int getTopInt() {
		return topInt;
	}

	void setTopInt(int topInt) {
		this->topInt = topInt;
	}


	int getLeftInt() {
		return leftInt;
	}

	void setLeftInt(int leftInt) {
		this->leftInt = leftInt;
	}


	int getRightInt() {
		return rightInt;
	}

	void setRightInt(int rightInt) {
		this->rightInt = rightInt;
	}


	int getBottomInt() {
		return bottomInt;
	}

	void setBottomInt(int bottomInt) {
		this->bottomInt = bottomInt;
	}


	int getHeightInt() {
		return heightInt;
	}

	void setHeightInt(int heightInt) {
		this->heightInt = heightInt;
	}

	int getWidthInt() {
		return widthInt;
	}

	void setWidthInt(int widthInt) {
		this->widthInt = widthInt;
	}

	//-------------------------------------

	const string& getBackgroundColor() const {
		return backgroundColor;
	}

	void setBackgroundColor(const string& backgroundColor) {
		this->backgroundColor = backgroundColor;
	}

	const string& getBackgroundOpacity() const {
		return backgroundOpacity;
	}

	void setBackgroundOpacity(const string& backgroundOpacity) {
		this->backgroundOpacity = backgroundOpacity;
	}

	const string& getBottom() const {
		return bottom;
	}

	void setBottom(const string& bottom) {
		this->bottom = bottom;
	}

	const list<Region*>& getChildren() const {
		return children;
	}

	void setChildren(const list<Region*>& children) {
		this->children = children;
	}

	void addChild(Region* child) {
		children.push_back(child);
	}

	const string& getFit() const {
		return fit;
	}

	void setFit(const string& fit) {
		this->fit = fit;
	}

	const string& getId() const {
		return id;
	}

	void setId(const string& id) {
		this->id = id;
	}

	const string& getLeft() const {
		return left;
	}

	void setLeft(const string& left) {
		this->left = left;
	}

	TopLayout*& getTopLayout() {
//        if( !myTopLayout )
//            return myTopLayout;
        
        if( parentRegion )
            return parentRegion->getTopLayout();
        
        return myTopLayout;
	}

	void setTopLayout(TopLayout*& myTopLayout) {
		this->myTopLayout = myTopLayout;
	}

    Region*& getParentRegion() {
        return parentRegion;
    }
    
	void setParentRegion(Region*& parentRegion) {
		this->parentRegion = parentRegion;
	}
    
	const string& getRegionName() const {
		return regionName;
	}

	void setRegionName(const string& regionName) {
		this->regionName = regionName;
	}

	const string& getRight() const {
		return right;
	}

	void setRight(const string& right) {
		this->right = right;
	}

	const string& getShowBackground() const {
		return showBackground;
	}

	void setShowBackground(const string& showBackground) {
		this->showBackground = showBackground;
	}

	const string& getTitle() const {
		return title;
	}

	void setTitle(const string& title) {
		this->title = title;
	}

	const string& getTop() const {
		return top;
	}

	void setTop(const string& top) {
		this->top = top;
	}

	const string& getZIndex() const {
		return zIndex;
	}

	void setZIndex(const string& index) {
		zIndex = index;
	}

	string& getHeight() {
		return height;
	}

	void setHeight(const string& height) {
		this->height = height;
	}

	string& getWidth() {
		return width;
	}

	void setWidth(string& width) {
		this->width = width;
	}

    string toString();
    
	void print() {
		list<Region*>::iterator rIt;

		cout << "<region id=\""<< id
						<<"\" title=\""<< title
						<<"\" top=\""<< top
						<<"\" bottom=\""<< bottom
						<<"\" left=\""<< left
						<<"\" right=\""<< right <<"\">\n";

		for(rIt=children.begin(); rIt!=children.end(); rIt++) {
			(*rIt)->print();
		}

		cout << "</region>" << endl;
	}


private:
	TopLayout* myTopLayout;
	list<Region*> children;
    Region*    parentRegion;
    
    int widthInt, heightInt,topInt=0,bottomInt=0,leftInt=0,rightInt=0;
    long topServer=0, leftServer=0;

	string id;
	string title, regionName;

	string top, bottom, left, right;
	string width, height;
	string backgroundColor;
	string backgroundOpacity;

	string fit;
	string showBackground;
	string zIndex;
};

#endif /* defined(__smil_parser__Region__) */
