#pragma once

#include "BaseClass.h"

class Movers : public BaseClass
{
public: 
	Movers(void);
	~Movers(void);

	//int xSpeed, ySpeed;
	double speed;
	int ID;
	bool collision();
	int getValueAtPosition();

	void setValueAtPosition(int value);

	void setState(int newState);

	int getState();


};

