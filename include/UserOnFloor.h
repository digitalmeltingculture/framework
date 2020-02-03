/*
 * UserOnFloor.h
 *
 *  Created on: 21/mag/2014
 *      Author: dariux881
 */

#ifndef USERONFLOOR_H_
#define USERONFLOOR_H_

class UserOnFloor {
public:
	UserOnFloor();
	UserOnFloor(short, bool, float, float, float);

	void setId(short);
	void setLeader(bool);
	void setPosition(float, float, float);

	short getId();
	bool  isLeader();
	float getX();
	float getY();
	float getZ();

	virtual ~UserOnFloor();

private:
	short	id;
	bool	leader;
	float	xPos, yPos, zPos;

};

#endif /* USERONFLOOR_H_ */
