#pragma once

#include "Movers.h"
#include "Enemies.h"
#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 4
#define TYPE 0
#define STATUS 1
#define TIMER 2
class Player : public Movers
{
public:
	Player(void);
	~Player(void);

	bool canMove(int, int = -1, int = -1);
	int currentMove;
	int queuedMove;

	////elemental powers
	bool findWater;
	int ability[4][3];
	int waterX, waterY;

	//Functions to use powers. 
	//NOTE: Effect for metal power is missing!!

	//Powers that only affect player
	void destroyBlock(int x, int y);
	void putRock(int x, int y);
	void putWater(int x, int y);
	void doAbility(int, Enemies elemental[], int&);
	void checkSpell(Enemies elemental[]);
	bool findIntersection(int checkX, int checkY);
	void movement();

};

