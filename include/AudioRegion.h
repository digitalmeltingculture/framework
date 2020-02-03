/*
 * AudioRegion.h
 *
 *  Created on: 13/dic/2014
 *      Author: dariux881
 */

#ifndef AUDIOREGION_H_
#define AUDIOREGION_H_

#include <string>

using namespace std;

class AudioRegion {
public:
	AudioRegion();
	virtual ~AudioRegion();

	const string& getId() const {
		return id;
	}

	void setId(const string& id) {
		this->id = id;
	}

	const string& getSoundLevel() const {
		return soundLevel;
	}

	void setSoundLevel(const string& soundLevel) {
		this->soundLevel = soundLevel;
	}

	const string& getTitle() const {
		return title;
	}

	void setTitle(const string& title) {
		this->title = title;
	}
    
    string toString();

private:
	string id;
	string title;

	string soundLevel;
};

#endif /* AUDIOREGION_H_ */
