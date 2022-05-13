#include "Movers.h"


Movers::Movers(void)
{
	BaseClass();
	//xSpeed = 0;
	//ySpeed = 0;
	speed = 0;
	ID = -1;
}

Movers::~Movers(void)
{
	//empty
}
bool Movers::collision()
{
	return false;
}

//As much of a pain as they are, these are some useful getters and setters

int Movers::getValueAtPosition() {
	return mapArray[(int)(x+16)/32][(int)(y+16)/32];
}

void Movers::setValueAtPosition(int value) {
	mapArray[(int)(x+16)/32][(int)(y+16)/32] = value;
}

void Movers::setState(int newState) {
	state = newState;
}

int Movers::getState() {
	return state;
}