/*
 * RootLayout.h
 *
 *  Created on: 13/dic/2014
 *      Author: dariux881
 */

#ifndef ROOTLAYOUT_H_
#define ROOTLAYOUT_H_

#include <iostream>
#include <string>
#include <list>

using namespace std;

class RootLayout {
public:
	RootLayout();
	virtual ~RootLayout();

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

	void print() {
		cout << "<root-layout id=\""<< id
						<<"\" title=\""<< title
						<<"\" bColor=\""<< backgroundColor
						<<"\" bOpacity=\""<< backgroundOpacity <<"\" />\n";
	}
    
    string toString();

private:
	string id;
	string title;

	string backgroundColor, backgroundOpacity;

	unsigned int width, height;
};

#endif /* ROOTLAYOUT_H_ */
