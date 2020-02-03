/*
 * TopLayout.h
 *
 *  Created on: 13/dic/2014
 *      Author: dariux881
 */

#ifndef TOPLAYOUT_H_
#define TOPLAYOUT_H_

#include <string>
#include <list>
#include "Region.h"
#include "Roi.h"
#include "Mask.h"
#include "RootLayout.h"
#include <sstream>

using namespace std;

class Region;
class Roi;
class Mask;

class TopLayout {
public:
	TopLayout();
	TopLayout(RootLayout*);
	virtual ~TopLayout();

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

	const string& getClose() const {
		return close;
	}

	void setClose(const string& close) {
		this->close = close;
	}

	unsigned int getHeight() const {
		return height;
	}

	void setHeight(unsigned int height) {
		this->height = height;
	}

	const string& getId() const {
		return id;
	}

	void setId(const string& id) {
		this->id = id;
	}

	const string& getName() const {
		return name;
	}

	void setName(const string& name) {
		this->name = name;
	}

	const string& getOpen() const {
		return open;
	}

	void setOpen(const string& open) {
		this->open = open;
	}

	const string& getTitle() const {
		return title;
	}

	void setTitle(const string& title) {
		this->title = title;
	}

	unsigned int getWidth() const {
		return width;
	}

	void setWidth(unsigned int width) {
		this->width = width;
	}

	const list<Region*>& getChildrenRegion() const {
		return childrenRegion;
	}

	void setChildrenRegion(const list<Region*>& children) {
		this->childrenRegion = children;
	}

	void addChildRegion(Region* newChild) {
		childrenRegion.push_back(newChild);
	}

	void addChildRoi(Roi* newChildRoi) {
		childrenRoi.push_back(newChildRoi);
	}

	void setChildren(const list<Roi*>& childrenRoi) {
		this->childrenRoi = childrenRoi;
	}

	RootLayout*& getParent() {
		return parent;
	}

	void setParent(RootLayout*& parent) {
		this->parent = parent;
	}

	const list<Roi*>& getChildrenRoi() const {
			return childrenRoi;
	}


	const list<Mask*>& getChildrenMask() const {
		return childrenMask;
	}

	void setChildrenMask(const list<Mask*>& children) {
		this->childrenMask = children;
	}

	void addChildMask(Mask* newChild) {
		childrenMask.push_back(newChild);
	}

	void print();
    string toString();
    
private:
    void constructor();
    
	list<Region*> childrenRegion;
	list<Roi*> childrenRoi;
	list<Mask*> childrenMask;

	RootLayout*	  parent;

	string id;
	string title, name;

	string backgroundColor, backgroundOpacity;

	unsigned int width, height;

	string open, close;
};

#endif /* TOPLAYOUT_H_ */
