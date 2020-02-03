
#ifndef __smil_parser__Roi__
#define __smil_parser__Roi__

#include <iostream>
#include <string>
#include <list>
#include "TopLayout.h"
#include <sstream>

using namespace std;

class TopLayout;

class Roi {
public:
	Roi() {
        myTopLayout = NULL;
        parentRoi = NULL;

        width ="0";
        height = "0";

        top="0";
        left="0";


        widthInt =0;
        heightInt=0;
        topInt=0;
        leftInt=0;
    }

	//------Campi Convertiti -------


	int getTopInt() {
		return topInt;
	}

	void setTopLong(int topInt) {
		this->topInt = topInt;
	}


	int getLeftInt() {
		return leftInt;
	}

	void setLeftLong(int leftInt) {
		this->leftInt = leftInt;
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

	void setWidthInt(long widthInt) {
		this->widthInt = widthInt;
	}

	//-------------------------------------

	const list<Roi*>& getChildren() const {
		return children;
	}

	void setChildren(const list<Roi*>& children) {
		this->children = children;
	}

	void addChild(Roi* child) {
		children.push_back(child);
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

        if( parentRoi )
            return parentRoi->getTopLayout();

        return myTopLayout;
	}

	void setTopLayout(TopLayout*& myTopLayout) {
		this->myTopLayout = myTopLayout;
	}

	Roi*& getParentRoi() {
        return parentRoi;
    }

	void setParentRoi(Roi*& parentRoi) {
		this->parentRoi = parentRoi;
	}

	const string& getRoiName() const {
		return roiName;
	}

	void setRoiName(const string& roiName) {
		this->roiName = roiName;
	}

	const string& getTop() const {
		return top;
	}

	void setTop(const string& top) {
		this->top = top;
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
		list<Roi*>::iterator rIt;

		cout << "<roi id=\""<< id
						<<"\" title=\""<< title
						<<"\" top=\""<< top
						<<"\" left=\""<< left<<"\">\n";

		for(rIt=children.begin(); rIt!=children.end(); rIt++) {
			(*rIt)->print();
		}

		cout << "</roi>" << endl;
	}


private:
	TopLayout* myTopLayout;
	list<Roi*> children;
    Roi*    parentRoi;

    int widthInt, heightInt,topInt=0,leftInt=0;

	string id;
	string title, roiName;

	string top, left;
	string width, height;

};

#endif /* defined(__smil_parser__Roi__) */
