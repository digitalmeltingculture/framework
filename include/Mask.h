#ifndef __smil_parser__Mask__
#define __smil_parser__Mask__

#include <iostream>
#include <string>
#include <list>
#include "TopLayout.h"
#include <sstream>

using namespace std;

class TopLayout;

class Mask {
public:
	Mask() {
        myTopLayout = NULL;
        parentMask =NULL;

        id="";
        src="";

        pivotX="";
        pivotY="";
        width="";
        height="";

    }

	//------Campi Convertiti -------


	const string& getId() const {
		return id;
	}

	void setId(const string& id) {
		this->id = id;
	}


	TopLayout*& getTopLayout() {
//        if( !myTopLayout )
//            return myTopLayout;

        /*if( parentRoi )
            return parentRoi->getTopLayout();
         */


		return myTopLayout;
	}

	void setTopLayout(TopLayout*& myTopLayout) {
		this->myTopLayout = myTopLayout;
	}


	const string& getSrc() const {
		return src;
	}

	void setSrc(const string& src) {
		this->src = src;
	}


	Mask*& getParentMask() {
		return parentMask;
	}

	void setParentMask(Mask*& parentMask) {
		this->parentMask = parentMask;
	}

	const list<Mask*>& getChildren() const {
		return children;
	}

	void setChildren(const list<Mask*>& children) {
		this->children = children;
	}

	void addChild(Mask* child) {
		children.push_back(child);
	}



	const string& getPivotX() const {
		return pivotX;
	}

	void setPivotX(const string& pivotX) {
		this->pivotX = pivotX;
	}


	const string& getPivotY() const {
			return pivotY;
	}

	void setPivotY(const string& pivotY) {
		this->pivotY = pivotY;
	}

	const string& getWidth() const {
		return this->width;
	}

	void setWidth(const string& width) {
		this->width = width;
	}


	const string& getHeight() const {
		return this->height;
	}

	void setHeight(const string& height) {
		this->height = height;
	}


	int getHeightInt() const {
		return this->heightInt;
	}

	void setHeightInt(int heightInt) {
		this->heightInt = heightInt;
	}


	int getWidthInt() const {
		return this->widthInt;
	}

	void setWidthInt(int widthInt) {
		this->widthInt = widthInt;
	}

	int getPivotXInt() {
		return this->pivotXInt;
	}

	void setPivotXInt(int pivotXInt) {
		this->pivotXInt = pivotXInt;
	}


	int getPivotYInt() {
		return this->pivotYInt;
	}

	void setPivotYInt(int pivotYInt) {
		this->pivotYInt = pivotYInt;
	}


    string toString();

	void print() {

		cout << "<mask id=\""<< id <<"\" src=\""<< src<<"\">\n";

		cout << "</mask>" << endl;
	}


private:
	TopLayout* myTopLayout;
	list<Mask*> children;
	Mask*    parentMask;

	string id;
	string src;


	string pivotX;
	string pivotY;
	string width;
	string height;

	int pivotXInt;
	int pivotYInt;
	int widthInt;
	int heightInt;



};

#endif /* defined(__smil_parser__Mask__) */
